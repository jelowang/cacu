
#include "RecastAlloc.h"
#include "RecastAssert.h"
#include "DetourAssert.h"
#include "DetourNavMeshBuilder.h"
#include "DetourCommon.h"
#include "DetourTileCacheBuilder.h"
#include "fastlz.h"
#include "H3NavMesh.h"
#include "H3DrawPrimitives.h"

static const int TILECACHESET_MAGIC = 'T' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'TSET';
static const int TILECACHESET_VERSION = 1;

static float gCacheBuildTimeMs = 0;
static int gCacheCompressedSize = 0;
static int gCacheRawSize = 0;
static int gCacheLayerCount = 0;
static int gCacheBuildMemUsage = 0;

//	jelo add start
static float gAgentRadius = 0 ;
static float gAgentHeight = 0;
static float gAgentClimb = 0 ;
static float gAgentSpos[3]={0};
static float gAgentEpos[3]={0};
//	jelo add end

/*
brief : These are just sample areas to use consistent values across the samples.
		The use should specify these base on his needs.
*/
enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP,
};

enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED = 0x10,	// Disabled polygon
	SAMPLE_POLYFLAGS_ALL = 0xffff	    // All abilities.
};

struct BoundsItem
{
	float bmin[2];
	float bmax[2];
	int i;
};

struct TileCacheData
{
	unsigned char* data;
	int dataSize;
};

struct TileCacheSetHeader
{
	int magic;
	int version;
	int numTiles;
	dtNavMeshParams meshParams;
	dtTileCacheParams cacheParams;
};

struct TileCacheTileHeader
{
	dtCompressedTileRef tileRef;
	int dataSize;
};

struct FastLZCompressor : public dtTileCacheCompressor
{
	virtual int maxCompressedSize(const int bufferSize)
	{
		return (int)(bufferSize* 1.05f);
	}

	virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
		unsigned char* compressed, const int /*maxCompressedSize*/, int* compressedSize)
	{
		*compressedSize = fastlz_compress((const void *const)buffer, bufferSize, compressed);
		return DT_SUCCESS;
	}

	virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
		unsigned char* buffer, const int maxBufferSize, int* bufferSize)
	{
		*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
		return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
	}
};

struct LinearAllocator : public dtTileCacheAlloc
{
	unsigned char* buffer;
	size_t capacity;
	size_t top;
	size_t high;

	LinearAllocator(const size_t cap) : buffer(0), capacity(0), top(0), high(0)
	{
		resize(cap);
	}

	~LinearAllocator()
	{
		dtFree(buffer);
	}

	void resize(const size_t cap)
	{
		if (buffer) dtFree(buffer);
		buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
		capacity = cap;
	}

	virtual void reset()
	{
		high = dtMax(high, top);
		top = 0;
	}

	virtual void* alloc(const size_t size)
	{
		if (!buffer)
			return 0;
		if (top + size > capacity)
			return 0;
		unsigned char* mem = &buffer[top];
		top += size;
		return mem;
	}

	virtual void free(void* /*ptr*/)
	{
		// Empty
	}
};

struct RasterizationContext
{
	RasterizationContext() :
	solid(0),
	triareas(0),
	lset(0),
	chf(0),
	ntiles(0)
	{
		memset(tiles, 0, sizeof(TileCacheData)*MAX_LAYERS);
	}

	~RasterizationContext()
	{
		rcFreeHeightField(solid);
		delete[] triareas;
		rcFreeHeightfieldLayerSet(lset);
		rcFreeCompactHeightfield(chf);
		for (int i = 0; i < MAX_LAYERS; ++i)
		{
			dtFree(tiles[i].data);
			tiles[i].data = 0;
		}
	}

	rcHeightfield* solid;
	unsigned char* triareas;
	rcHeightfieldLayerSet* lset;
	rcCompactHeightfield* chf;
	TileCacheData tiles[MAX_LAYERS];
	int ntiles;
};

struct MeshProcess : public dtTileCacheMeshProcess
{
	InputGeom* m_geom;

	inline MeshProcess() : m_geom(0)
	{
	}

	inline void init(void* geom)
	{
		m_geom = (InputGeom*)geom;
	}

	virtual void process(struct dtNavMeshCreateParams* params,
		unsigned char* polyAreas, unsigned short* polyFlags)
	{
		// Update poly flags from areas.
		for (int i = 0; i < params->polyCount; ++i)
		{

			if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
				polyAreas[i] = SAMPLE_POLYAREA_GROUND;

			if (polyAreas[i] == SAMPLE_POLYAREA_GROUND ||
				polyAreas[i] == SAMPLE_POLYAREA_GRASS ||
				polyAreas[i] == SAMPLE_POLYAREA_ROAD)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_WALK;
			}
			else if (polyAreas[i] == SAMPLE_POLYAREA_WATER)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_SWIM;
			}
			else if (polyAreas[i] == SAMPLE_POLYAREA_DOOR)
			{
				polyFlags[i] = SAMPLE_POLYFLAGS_WALK | SAMPLE_POLYFLAGS_DOOR;
			}
		}

		// Pass in off-mesh connections.
		if (m_geom)
		{

			params->offMeshConVerts = m_geom->getOffMeshConnectionVerts();
			params->offMeshConRad = m_geom->getOffMeshConnectionRads();
			params->offMeshConDir = m_geom->getOffMeshConnectionDirs();
			params->offMeshConAreas = m_geom->getOffMeshConnectionAreas();
			params->offMeshConFlags = m_geom->getOffMeshConnectionFlags();
			params->offMeshConUserID = m_geom->getOffMeshConnectionId();
			params->offMeshConCount = m_geom->getOffMeshConnectionCount();

		}
	}
};

//	jelo add start
// Returns a random number [0..1)
static float frand()
{
	//	return ((float)(rand() & 0xffff)/(float)0xffff);
	return (float)rand() / (float)RAND_MAX;
}
//	jelo add end

static inline bool inRange(const float* v1, const float* v2, const float r, const float h)
{
	const float dx = v2[0] - v1[0];
	const float dy = v2[1] - v1[1];
	const float dz = v2[2] - v1[2];
	return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
}

static inline unsigned int duRGBA(int r, int g, int b, int a)
{
	return ((unsigned int)r) | ((unsigned int)g << 8) | ((unsigned int)b << 16) | ((unsigned int)a << 24);
}

static inline unsigned int duRGBAf(float fr, float fg, float fb, float fa)
{
	unsigned char r = (unsigned char)(fr*255.0f);
	unsigned char g = (unsigned char)(fg*255.0f);
	unsigned char b = (unsigned char)(fb*255.0f);
	unsigned char a = (unsigned char)(fa*255.0f);
	return duRGBA(r, g, b, a);
}

static inline bool checkOverlapRect(const float amin[2], const float amax[2], const float bmin[2], const float bmax[2])
{
	bool overlap = true;
	overlap = (amin[0] > bmax[0] || amax[0] < bmin[0]) ? false : overlap;
	overlap = (amin[1] > bmax[1] || amax[1] < bmin[1]) ? false : overlap;
	return overlap;
}

static float distancePtLine2d(const float* pt, const float* p, const float* q)
{
	float pqx = q[0] - p[0];
	float pqz = q[2] - p[2];
	float dx = pt[0] - p[0];
	float dz = pt[2] - p[2];
	float d = pqx*pqx + pqz*pqz;
	float t = pqx*dx + pqz*dz;
	if (d != 0) t /= d;
	dx = p[0] + t*pqx - pt[0];
	dz = p[2] + t*pqz - pt[2];
	return dx*dx + dz*dz;
}

static int calcLayerBufferSize(const int gridWidth, const int gridHeight)
{
	const int headerSize = dtAlign4(sizeof(dtTileCacheLayerHeader));
	const int gridSize = gridWidth * gridHeight;
	return headerSize + gridSize * 4;
}

static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
							   const float minTargetDist, const dtPolyRef* path, const int pathSize,
							   float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
							   float* outPoints, int* outPointCount)
{
	outPoints = 0;
	outPointCount = 0;

	// Find steer target.
	static const int MAX_STEER_POINTS = 3;
	float steerPath[MAX_STEER_POINTS * 3];
	unsigned char steerPathFlags[MAX_STEER_POINTS];
	dtPolyRef steerPathPolys[MAX_STEER_POINTS];
	int nsteerPath = 0;
	navQuery->findStraightPath(startPos, endPos, path, pathSize,
		steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
	if (!nsteerPath)
		return false;

	if (outPoints && outPointCount)
	{
		*outPointCount = nsteerPath;
		for (int i = 0; i < nsteerPath; ++i)
			dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
	}


	// Find vertex far enough to steer to.
	int ns = 0;
	while (ns < nsteerPath)
	{
		// Stop at Off-Mesh link or when point is further than slop away.
		if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
			!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
			break;
		ns++;
	}
	// Failed to find good point to steer to.
	if (ns >= nsteerPath)
		return false;

	dtVcopy(steerPos, &steerPath[ns * 3]);
	steerPos[1] = startPos[1];
	steerPosFlag = steerPathFlags[ns];
	steerPosRef = steerPathPolys[ns];

	return true;
}

static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath, const dtPolyRef* visited, const int nvisited)
{
	int furthestPath = -1;
	int furthestVisited = -1;

	// Find furthest common polygon.
	for (int i = npath - 1; i >= 0; --i)
	{
		bool found = false;
		for (int j = nvisited - 1; j >= 0; --j)
		{
			if (path[i] == visited[j])
			{
				furthestPath = i;
				furthestVisited = j;
				found = true;
			}
		}
		if (found)
			break;
	}

	// If no intersection found just return current path. 
	if (furthestPath == -1 || furthestVisited == -1)
		return npath;

	// Concatenate paths.	

	// Adjust beginning of the buffer to include the visited.
	const int req = nvisited - furthestVisited;
	const int orig = rcMin(furthestPath + 1, npath);
	int size = rcMax(0, npath - orig);
	if (req + size > maxPath)
		size = maxPath - req;
	if (size)
		memmove(path + req, path + orig, size*sizeof(dtPolyRef));

	// Store visited
	for (int i = 0; i < req; ++i)
		path[i] = visited[(nvisited - 1) - i];

	return req + size;
}

/*
 brief : This function checks if the path has a small U-turn, that is,
		 a polygon further in the path is adjacent to the first polygon
		 in the path. If that happens, a shortcut is taken.
		 This can happen if the target (T) location is at tile boundary,
		 and we're (S) approaching it parallel to the tile edge.
		 The choice at the vertex can be arbitrary, 
		 +---+---+
		 |:::|:::|
		 +-S-+-T-+
		 |:::|   | <-- the step can end up in here, resulting U-turn path.
		 +---+---+
*/
static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
{
	if (npath < 3)
		return npath;

	// Get connected polygons
	static const int maxNeis = 16;
	dtPolyRef neis[maxNeis];
	int nneis = 0;

	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
		return npath;

	for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
	{
		const dtLink* link = &tile->links[k];
		if (link->ref != 0)
		{
			if (nneis < maxNeis)
				neis[nneis++] = link->ref;
		}
	}

	// If any of the neighbour polygons is within the next few polygons
	// in the path, short cut to that polygon directly.
	static const int maxLookAhead = 6;
	int cut = 0;
	for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
		for (int j = 0; j < nneis; j++)
		{
			if (path[i] == neis[j]) {
				cut = i;
				break;
			}
		}
	}
	if (cut > 1)
	{
		int offset = cut - 1;
		npath -= offset;
		for (int i = 1; i < npath; i++)
			path[i] = path[i + offset];
	}

	return npath;
}

static void drawPolyonBoundaries(H3DrawPrimitivesType type, const dtMeshTile* tile, const unsigned int col, const float linew, bool inner)
{
	static const float thr = 0.01f*0.01f;

	H3DrawPrimitives::begin(H3DrawPrimitives_Line);

	for (int i = 0; i < tile->header->polyCount; ++i)
	{
		const dtPoly* p = &tile->polys[i];

		if (p->getType() == DT_POLYTYPE_OFFMESH_CONNECTION) continue;

		const dtPolyDetail* pd = &tile->detailMeshes[i];

		for (int j = 0, nj = (int)p->vertCount; j < nj; ++j)
		{
			unsigned int c = col;
			if (inner)
			{
				if (p->neis[j] == 0) continue;
				if (p->neis[j] & DT_EXT_LINK)
				{
					bool con = false;
					for (unsigned int k = p->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
					{
						if (tile->links[k].edge == j)
						{
							con = true;
							break;
						}
					}
					if (con)
						c = duRGBA(255, 255, 255, 48);
					else
						c = duRGBA(0, 0, 0, 48);
				}
				else
					c = duRGBA(0, 48, 64, 32);
			}
			else
			{
				if (p->neis[j] != 0) continue;
			}

			const float* v0 = &tile->verts[p->verts[j] * 3];
			const float* v1 = &tile->verts[p->verts[(j + 1) % nj] * 3];

			// Draw detail mesh edges which align with the actual poly edge.
			// This is really slow.

			for (int k = 0; k < pd->triCount; ++k)
			{
				const unsigned char* t = &tile->detailTris[(pd->triBase + k) * 4];
				const float* tv[3];
				for (int m = 0; m < 3; ++m)
				{
					if (t[m] < p->vertCount)
						tv[m] = &tile->verts[p->verts[t[m]] * 3];
					else
						tv[m] = &tile->detailVerts[(pd->vertBase + (t[m] - p->vertCount)) * 3];
				}
				for (int m = 0, n = 2; m < 3; n = m++)
				{
					if (((t[3] >> (n * 2)) & 0x3) == 0) continue;	// Skip inner detail edges.
					if (distancePtLine2d(tv[n], v0, v1) < thr &&
						distancePtLine2d(tv[m], v0, v1) < thr)
					{
						//dd->vertex(tv[n], c);
						//dd->vertex(tv[m], c);
						H3DrawPrimitives::addVertex(tv[n][0], tv[n][1], tv[n][2]);
						H3DrawPrimitives::addVertex(tv[m][0], tv[m][1], tv[m][2]);
					}
				}
			}
		}
	}
	H3DrawPrimitives::end();
}

void duDebugDrawNavMeshPoly ( const dtNavMesh& mesh, dtPolyRef ref, const unsigned int col )
{

	const dtMeshTile* tile = 0;
	const dtPoly* poly = 0;
	if ( dtStatusFailed ( mesh.getTileAndPolyByRef ( ref, &tile, &poly ) ) )
		return;

	H3DrawPrimitives::setDepthTestEnable ( true );

	const unsigned int c = (col & 0x00ffffff) | (64 << 24);
	const unsigned int ip = (unsigned int)(poly - tile->polys);

	if ( poly->getType ( ) == DT_POLYTYPE_OFFMESH_CONNECTION )
	{
		dtOffMeshConnection* con = &tile->offMeshCons[ip - tile->header->offMeshBase];
		 
		// Connection arc.
		H3DrawPrimitives::drawArc ( con->pos[0], con->pos[1], con->pos[2], con->pos[3], con->pos[4], con->pos[5], 0.25f,
			(con->flags & 1) ? 0.6f : 0, 0.6f );

	}
	else
	{
		const dtPolyDetail* pd = &tile->detailMeshes[ip];

		H3DrawPrimitives::begin ( H3DrawPrimitives_Triangle );
		for ( int i = 0; i < pd->triCount; ++i )
		{
			const unsigned char* t = &tile->detailTris[(pd->triBase + i) * 4];
			for ( int j = 0; j < 3; ++j )
			{
				if ( t[j] < poly->vertCount ) {
					float* tv = &tile->verts[poly->verts[t[j]] * 3] ;
					H3DrawPrimitives::addVertex ( tv[0], tv[1], tv[2] ) ;
				} else {
					float* tv = &tile->detailVerts[(pd->vertBase + t[j] - poly->vertCount) * 3];
					H3DrawPrimitives::addVertex ( tv[0], tv[1], tv[2] );
				}
			}
		}
		H3DrawPrimitives::end ( );
	}

	H3DrawPrimitives::setDepthTestEnable ( true );

}

static void drawMeshTile(H3DrawPrimitivesType type, const dtNavMesh& mesh, const dtNavMeshQuery* query, const dtMeshTile* tile, unsigned char flags)
{
	dtPolyRef base = mesh.getPolyRefBase(tile);

	int tileNum = mesh.decodePolyIdTile(base);

	H3DrawPrimitives::setDepthTestEnable(true);

	H3DrawPrimitives::begin(type);
	H3DrawPrimitives::setColor ( 0, 0, 100, 100.0 / 255.0 );

	for (int i = 0; i < tile->header->polyCount; ++i)
	{
		const dtPoly* p = &tile->polys[i];
		if (p->getType() == DT_POLYTYPE_OFFMESH_CONNECTION)	// Skip off-mesh links.
			continue;

		const dtPolyDetail* pd = &tile->detailMeshes[i];

		
		unsigned int col;
		if (query && query->isInClosedList(base | (dtPolyRef)i))
			H3DrawPrimitives::setColor ( 255.0f / 255.0f, 196.0f / 255.0f, 0.0f / 255.0f, 64.0f / 255.0f );
		//else
		{
			//if (flags & DU_DRAWNAVMESH_COLOR_TILES)
			{
				//col = duIntToCol(tileNum, 128);
			}
			//else
			{
			//	if (p->getArea() == 0) // Treat zero area type as default.
			//		col = duRGBA(0, 192, 255, 64);
				//else
				//	col = duIntToCol(p->getArea(), 64);
			}
		}
		

		H3Vector3f a; 
		H3Vector3f b;
		H3Vector3f c;
		for (int j = 0; j < pd->triCount; ++j)
		{
			const unsigned char* t = &tile->detailTris[(pd->triBase + j) * 4];
			int k = 0;
			for (int k = 0; k < 3; ++k)
			{
				//if (t[k] < p->vertCount)
				//	dd->vertex(&tile->verts[p->verts[t[k]] * 3], col);
				//else
				//	dd->vertex(&tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3], col);

				if (t[k] < p->vertCount) {
					float* table = &tile->verts[p->verts[t[k]] * 3];
					H3DrawPrimitives::addVertex(table[0], table[1], table[2]);
				} else {
					float* table = &tile->detailVerts[(pd->vertBase + t[k] - p->vertCount) * 3];
					H3DrawPrimitives::addVertex(table[0], table[1], table[2]);
				}

			}
		}
	}
	H3DrawPrimitives::end();

#if 1
	// Draw inter poly boundaries
	H3DrawPrimitives::setColor ( 0, 48.0f / 255.0f, 64.0f / 255.0f, 100.0f / 255.0 );
	drawPolyonBoundaries ( H3DrawPrimitives_Line, tile, duRGBA ( 0, 48, 64, 32 ), 1.5f, true );

	// Draw outer poly boundaries
	H3DrawPrimitives::setColor ( 0, 48.0f/255.0f, 64.0f / 255.0f, 255.0f / 255.0 );
	drawPolyonBoundaries ( H3DrawPrimitives_Line, tile, duRGBA ( 0, 48, 64, 220 ), 2.5f, false );

	//if (flags & DU_DRAWNAVMESH_OFFMESHCONS)
	if (1)
	{
		H3DrawPrimitives::begin(H3DrawPrimitives_Line);
		for (int i = 0; i < tile->header->polyCount; ++i)
		{
			const dtPoly* p = &tile->polys[i];
			if (p->getType() != DT_POLYTYPE_OFFMESH_CONNECTION)	// Skip regular polys.
				continue;

			unsigned int col, col2;
			if (query && query->isInClosedList(base | (dtPolyRef)i))				
				H3DrawPrimitives::setColor ( 255.0f / 255.0f, 196.0f / 255.0f, 0.0f / 255.0f, 220.0f / 255.0f );
			//else
			//	col = duDarkenCol(duIntToCol(p->getArea(), 220));

			const dtOffMeshConnection* con = &tile->offMeshCons[i - tile->header->offMeshBase];
			const float* va = &tile->verts[p->verts[0] * 3];
			const float* vb = &tile->verts[p->verts[1] * 3];

			// Check to see if start and end end-points have links.
			bool startSet = false;
			bool endSet = false;
			for (unsigned int k = p->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
			{
				if (tile->links[k].edge == 0)
					startSet = true;
				if (tile->links[k].edge == 1)
					endSet = true;
			}

			H3DrawPrimitives::setColor ( 255.0f / 255.0f , 0, 0, 255.0 / 255.0 );
			// End points and their on-mesh locations.
			H3DrawPrimitives::addVertex(va[0], va[1], va[2]);
			H3DrawPrimitives::addVertex(con->pos[0], con->pos[1], con->pos[2]);			
			H3DrawPrimitives::setColor ( 220.0f / 255.0f, 32.0f / 255.0f, 16.0f / 255.0f, 196.0f / 255.0f );
			H3DrawPrimitives::drawCircle( con->pos[0], con->pos[1] + 0.1f, con->pos[2], con->rad );

			H3DrawPrimitives::addVertex(vb[0], vb[1], vb[2]);
			H3DrawPrimitives::addVertex(con->pos[3], con->pos[4], con->pos[5]);
			H3DrawPrimitives::setColor ( 220.0f / 255.0f, 32.0f / 255.0f, 16.0f / 255.0f, 196.0 / 255.0 );
			H3DrawPrimitives::drawCircle(con->pos[3], con->pos[4] + 0.1f, con->pos[5], con->rad );

			// End point vertices.
			H3DrawPrimitives::setColor ( 0, 48 / 255.0f, 64 / 255.0f, 196.0 / 255.0 );
			H3DrawPrimitives::addVertex(con->pos[0], con->pos[1], con->pos[2]);
			H3DrawPrimitives::addVertex(con->pos[0], con->pos[1] + 0.2f, con->pos[2]);

			H3DrawPrimitives::setColor ( 0, 48 / 255.0f, 64 / 255.0f, 196.0 / 255.0 );
			H3DrawPrimitives::addVertex(con->pos[3], con->pos[4], con->pos[5]);
			H3DrawPrimitives::setColor ( 0, 48 / 255.0f, 64 / 255.0f, 196.0 / 255.0 );
			H3DrawPrimitives::addVertex(con->pos[3], con->pos[4] + 0.2f, con->pos[5]);

			// Connection arc.			
			H3DrawPrimitives::drawArc ( con->pos[0], con->pos[1], con->pos[2], con->pos[3], con->pos[4], con->pos[5], 0.25f,
				(con->flags & 1) ? 0.6f : 0, 0.6f );
		}
		
		H3DrawPrimitives::end();
	}

	const unsigned int vcol = duRGBA ( 0, 0, 0, 196 / 255 );
	H3DrawPrimitives::begin(H3DrawPrimitives_Point);
	for (int i = 0; i < tile->header->vertCount; ++i)
	{
		const float* v = &tile->verts[i * 3];
		H3DrawPrimitives::addVertex(v[0], v[1], v[2]);
	}

	H3DrawPrimitives::end();
	H3DrawPrimitives::setDepthTestEnable(true);
#endif
}

static int rcGetChunksOverlappingRect(const rcChunkyTriMesh* cm, float bmin[2], float bmax[2], int* ids, const int maxIds)
{
	// Traverse tree
	int i = 0;
	int n = 0;
	while (i < cm->nnodes)
	{
		const rcChunkyTriMeshNode* node = &cm->nodes[i];
		const bool overlap = checkOverlapRect(bmin, bmax, node->bmin, node->bmax);
		const bool isLeafNode = node->i >= 0;

		if (isLeafNode && overlap)
		{
			if (n < maxIds)
			{
				ids[n] = i;
				n++;
			}
		}

		if (overlap || isLeafNode)
			i++;
		else
		{
			const int escapeIndex = -node->i;
			i += escapeIndex;
		}
	}

	return n;
}

static int rasterizeTileLayers(BuildContext* ctx, InputGeom* geom, const int tx, const int ty, const rcConfig& cfg, TileCacheData* tiles, const int maxTiles , H3Object* object )
{
	if (!geom || !object || !geom->getChunkyMesh())
	//if (!geom || !geom->getMesh() || !geom->getChunkyMesh())
	{
		//ctx->log(RC_LOG_ERROR, "buildTile: Input mesh is not specified.");
		iLog("buildTile: Input mesh is not specified.");
		return 0;
	}

	FastLZCompressor comp;
	RasterizationContext rc;

	//const float* verts = geom->getMesh()->getVerts();
	//const int nverts = geom->getMesh()->getVertCount();

	int nverts = 0;

	H3Mesh* mesh = (H3Mesh*)object->getMesh();
	int meshTotal = mesh->meshGroupList.size();

	H3MeshGroup* meshGroup;
	int index;
	int vecIdx;
	float vec;
	float* vecterArray = (float*)malloc(2048 * sizeof(float));
	for (int count = 0; count < meshTotal; count++) {

		meshGroup = mesh->getMeshGroup(count);
		index = meshGroup->face.length;         //vertexCount

		for (int walker = 0; walker < index; walker +=3) {
			if (walker==0)
				vecIdx = meshGroup->face.data[walker];
			else
				vecIdx = meshGroup->face.data[walker-3];
			vec = meshGroup->quad[vecIdx].vertex.x;
			vecterArray[walker] = vec;
			vec = meshGroup->quad[vecIdx].vertex.z;
			vecterArray[walker+1] = vec;
			vec = meshGroup->quad[vecIdx].vertex.y;
			vecterArray[walker+2] = vec;
			nverts += 3;
		}
	}
	const float* verts = vecterArray;


	const rcChunkyTriMesh* chunkyMesh = geom->getChunkyMesh();

	// Tile bounds.
	const float tcs = cfg.tileSize * cfg.cs;

	rcConfig tcfg;
	memcpy(&tcfg, &cfg, sizeof(tcfg));

	tcfg.bmin[0] = cfg.bmin[0] + tx*tcs;
	tcfg.bmin[1] = cfg.bmin[1];
	tcfg.bmin[2] = cfg.bmin[2] + ty*tcs;
	tcfg.bmax[0] = cfg.bmin[0] + (tx + 1)*tcs;
	tcfg.bmax[1] = cfg.bmax[1];
	tcfg.bmax[2] = cfg.bmin[2] + (ty + 1)*tcs;
	tcfg.bmin[0] -= tcfg.borderSize*tcfg.cs;
	tcfg.bmin[2] -= tcfg.borderSize*tcfg.cs;
	tcfg.bmax[0] += tcfg.borderSize*tcfg.cs;
	tcfg.bmax[2] += tcfg.borderSize*tcfg.cs;

	// Allocate voxel heightfield where we rasterize our input data to.
	rc.solid = rcAllocHeightfield();
	if (!rc.solid)
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		iLog("buildNavigation: Out of memory 'solid'.");
		return 0;
	}
	if (!rcCreateHeightfield(ctx, *rc.solid, tcfg.width, tcfg.height, tcfg.bmin, tcfg.bmax, tcfg.cs, tcfg.ch))
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		iLog("buildNavigation: Could not create solid heightfield.");
		return 0;
	}

	// Allocate array that can hold triangle flags.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	rc.triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
	if (!rc.triareas)
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", chunkyMesh->maxTrisPerChunk);
		iLog("buildNavigation: Out of memory 'm_triareas'");
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = tcfg.bmin[0];
	tbmin[1] = tcfg.bmin[2];
	tbmax[0] = tcfg.bmax[0];
	tbmax[1] = tcfg.bmax[2];
	int cid[512];// TODO: Make grow when returning too many items.
	const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
	if (!ncid)
	{
		return 0; // empty
	}

	for (int i = 0; i < ncid; ++i)
	{
		const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
		const int* tris = &chunkyMesh->tris[node.i * 3];
		const int ntris = node.n;

		memset(rc.triareas, 0, ntris*sizeof(unsigned char));
		rcMarkWalkableTriangles(ctx, tcfg.walkableSlopeAngle,
			verts, nverts, tris, ntris, rc.triareas);

		if (!rcRasterizeTriangles(ctx, verts, nverts, tris, rc.triareas, ntris, *rc.solid, tcfg.walkableClimb))
			return 0;
	}

	// Once all geometry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(ctx, tcfg.walkableClimb, *rc.solid);
	rcFilterLedgeSpans(ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid);
	rcFilterWalkableLowHeightSpans(ctx, tcfg.walkableHeight, *rc.solid);


	rc.chf = rcAllocCompactHeightfield();
	if (!rc.chf)
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		iLog("buildNavigation: Out of memory 'chf'.");
		return 0;
	}
	if (!rcBuildCompactHeightfield(ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid, *rc.chf))
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		iLog("buildNavigation: Could not build compact data.");
		return 0;
	}

	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(ctx, tcfg.walkableRadius, *rc.chf))
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		iLog("buildNavigation: Could not erode.");
		return 0;
	}

	// (Optional) Mark areas.
	const ConvexVolume* vols = geom->getConvexVolumes();
	for (int i = 0; i < geom->getConvexVolumeCount(); ++i)
	{
		rcMarkConvexPolyArea(ctx, vols[i].verts, vols[i].nverts,
			vols[i].hmin, vols[i].hmax,
			(unsigned char)vols[i].area, *rc.chf);
	}

	rc.lset = rcAllocHeightfieldLayerSet();
	if (!rc.lset)
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'lset'.");
		iLog("buildNavigation: Out of memory 'lset'.");
		return 0;
	}
	if (!rcBuildHeightfieldLayers(ctx, *rc.chf, tcfg.borderSize, tcfg.walkableHeight, *rc.lset))
	{
		//ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build heighfield layers.");
		iLog("buildNavigation: Could not build heighfield layers.");
		return 0;
	}

	rc.ntiles = 0;
	for (int i = 0; i < rcMin(rc.lset->nlayers, MAX_LAYERS); ++i)
	{
		TileCacheData* tile = &rc.tiles[rc.ntiles++];
		const rcHeightfieldLayer* layer = &rc.lset->layers[i];

		// Store header
		dtTileCacheLayerHeader header;
		header.magic = DT_TILECACHE_MAGIC;
		header.version = DT_TILECACHE_VERSION;

		// Tile layer location in the navmesh.
		header.tx = tx;
		header.ty = ty;
		header.tlayer = i;
		dtVcopy(header.bmin, layer->bmin);
		dtVcopy(header.bmax, layer->bmax);

		// Tile info.
		header.width = (unsigned char)layer->width;
		header.height = (unsigned char)layer->height;
		header.minx = (unsigned char)layer->minx;
		header.maxx = (unsigned char)layer->maxx;
		header.miny = (unsigned char)layer->miny;
		header.maxy = (unsigned char)layer->maxy;
		header.hmin = (unsigned short)layer->hmin;
		header.hmax = (unsigned short)layer->hmax;

		dtStatus status = dtBuildTileCacheLayer(&comp, &header, layer->heights, layer->areas, layer->cons,
			&tile->data, &tile->dataSize);
		if (dtStatusFailed(status))
		{
			return 0;
		}
	}

	// Transfer ownsership of tile data from build context to the caller.
	int n = 0;
	for (int i = 0; i < rcMin(rc.ntiles, maxTiles); ++i)
	{
		tiles[n++] = rc.tiles[i];
		rc.tiles[i].data = 0;
		rc.tiles[i].dataSize = 0;
	}

	return n;
}

static int rasterizeTileLayers(BuildContext* ctx, InputGeom* geom,const int tx, const int ty,rcConfig& cfg,TileCacheData* tiles,const int maxTiles)
{
	if (!geom || !geom->getMesh() || !geom->getChunkyMesh())
	{
		ctx->log(RC_LOG_ERROR, "buildTile: Input mesh is not specified.");
		return 0;
	}

	FastLZCompressor comp;
	RasterizationContext rc;

	const float* verts = geom->getMesh()->getVerts();
	const int nverts = geom->getMesh()->getVertCount();
	const rcChunkyTriMesh* chunkyMesh = geom->getChunkyMesh();

	// Tile bounds.
	const float tcs = cfg.tileSize * cfg.cs;

	rcConfig tcfg;

	cfg.bmin[0] = geom->m_meshBMin[0];
	cfg.bmin[1] = geom->m_meshBMin[1];
	cfg.bmin[2] = geom->m_meshBMin[2];
	cfg.bmax[0] = geom->m_meshBMax[0];
	cfg.bmax[1] = geom->m_meshBMax[1];
	cfg.bmax[2] = geom->m_meshBMax[2];

	memcpy(&tcfg, &cfg, sizeof(tcfg));

	tcfg.bmin[0] = cfg.bmin[0] + tx*tcs;
	tcfg.bmin[1] = cfg.bmin[1];
	tcfg.bmin[2] = cfg.bmin[2] + ty*tcs;
	tcfg.bmax[0] = cfg.bmin[0] + (tx + 1)*tcs;
	tcfg.bmax[1] = cfg.bmax[1];
	tcfg.bmax[2] = cfg.bmin[2] + (ty + 1)*tcs;
	tcfg.bmin[0] -= tcfg.borderSize*tcfg.cs;
	tcfg.bmin[2] -= tcfg.borderSize*tcfg.cs;
	tcfg.bmax[0] += tcfg.borderSize*tcfg.cs;
	tcfg.bmax[2] += tcfg.borderSize*tcfg.cs;

	// Allocate voxel heightfield where we rasterize our input data to.
	rc.solid = rcAllocHeightfield();
	if (!rc.solid)
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'solid'.");
		return 0;
	}
	if (!rcCreateHeightfield(ctx, *rc.solid, tcfg.width, tcfg.height, tcfg.bmin, tcfg.bmax, tcfg.cs, tcfg.ch))
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Could not create solid heightfield.");
		return 0;
	}

	// Allocate array that can hold triangle flags.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	rc.triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
	if (!rc.triareas)
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'm_triareas' (%d).", chunkyMesh->maxTrisPerChunk);
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = tcfg.bmin[0];
	tbmin[1] = tcfg.bmin[2];
	tbmax[0] = tcfg.bmax[0];
	tbmax[1] = tcfg.bmax[2];
	int cid[512];// TODO: Make grow when returning too many items.
	const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
	if (!ncid)
	{
		return 0; // empty
	}

	for (int i = 0; i < ncid; ++i)
	{
		const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
		const int* tris = &chunkyMesh->tris[node.i * 3];
		const int ntris = node.n;

		memset(rc.triareas, 0, ntris*sizeof(unsigned char));
		rcMarkWalkableTriangles(ctx, tcfg.walkableSlopeAngle,
			verts, nverts, tris, ntris, rc.triareas);

		if (!rcRasterizeTriangles(ctx, verts, nverts, tris, rc.triareas, ntris, *rc.solid, tcfg.walkableClimb))
			return 0;
	}

	// Once all geometry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(ctx, tcfg.walkableClimb, *rc.solid);
	rcFilterLedgeSpans(ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid);
	rcFilterWalkableLowHeightSpans(ctx, tcfg.walkableHeight, *rc.solid);


	rc.chf = rcAllocCompactHeightfield();
	if (!rc.chf)
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'chf'.");
		return 0;
	}
	if (!rcBuildCompactHeightfield(ctx, tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid, *rc.chf))
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build compact data.");
		return 0;
	}

	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(ctx, tcfg.walkableRadius, *rc.chf))
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Could not erode.");
		return 0;
	}

	// (Optional) Mark areas.
	const ConvexVolume* vols = geom->getConvexVolumes();
	for (int i = 0; i < geom->getConvexVolumeCount(); ++i)
	{
		rcMarkConvexPolyArea(ctx, vols[i].verts, vols[i].nverts,
			vols[i].hmin, vols[i].hmax,
			(unsigned char)vols[i].area, *rc.chf);
	}

	rc.lset = rcAllocHeightfieldLayerSet();
	if (!rc.lset)
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Out of memory 'lset'.");
		return 0;
	}
	if (!rcBuildHeightfieldLayers(ctx, *rc.chf, tcfg.borderSize, tcfg.walkableHeight, *rc.lset))
	{
		ctx->log(RC_LOG_ERROR, "buildNavigation: Could not build heighfield layers.");
		return 0;
	}

	rc.ntiles = 0;
	for (int i = 0; i < rcMin(rc.lset->nlayers, MAX_LAYERS); ++i)
	{
		TileCacheData* tile = &rc.tiles[rc.ntiles++];
		const rcHeightfieldLayer* layer = &rc.lset->layers[i];

		// Store header
		dtTileCacheLayerHeader header;
		header.magic = DT_TILECACHE_MAGIC;
		header.version = DT_TILECACHE_VERSION;

		// Tile layer location in the navmesh.
		header.tx = tx;
		header.ty = ty;
		header.tlayer = i;
		dtVcopy(header.bmin, layer->bmin);
		dtVcopy(header.bmax, layer->bmax);

		// Tile info.
		header.width = (unsigned char)layer->width;
		header.height = (unsigned char)layer->height;
		header.minx = (unsigned char)layer->minx;
		header.maxx = (unsigned char)layer->maxx;
		header.miny = (unsigned char)layer->miny;
		header.maxy = (unsigned char)layer->maxy;
		header.hmin = (unsigned short)layer->hmin;
		header.hmax = (unsigned short)layer->hmax;

		dtStatus status = dtBuildTileCacheLayer(&comp, &header, layer->heights, layer->areas, layer->cons,
			&tile->data, &tile->dataSize);
		if (dtStatusFailed(status))
		{
			return 0;
		}
	}

	// Transfer ownsership of tile data from build context to the caller.
	int n = 0;
	for (int i = 0; i < rcMin(rc.ntiles, maxTiles); ++i)
	{
		tiles[n++] = rc.tiles[i];
		rc.tiles[i].data = 0;
		rc.tiles[i].dataSize = 0;
	}

	return n;
}

static int compareItemX(const void* va, const void* vb)
{
	const BoundsItem* a = (const BoundsItem*)va;
	const BoundsItem* b = (const BoundsItem*)vb;
	if (a->bmin[0] < b->bmin[0])
		return -1;
	if (a->bmin[0] > b->bmin[0])
		return 1;
	return 0;
}

static int compareItemY(const void* va, const void* vb)
{
	const BoundsItem* a = (const BoundsItem*)va;
	const BoundsItem* b = (const BoundsItem*)vb;
	if (a->bmin[1] < b->bmin[1])
		return -1;
	if (a->bmin[1] > b->bmin[1])
		return 1;
	return 0;
}

static void calcExtends(const BoundsItem* items, const int /*nitems*/, const int imin, const int imax, float* bmin, float* bmax)
{
	bmin[0] = items[imin].bmin[0];
	bmin[1] = items[imin].bmin[1];

	bmax[0] = items[imin].bmax[0];
	bmax[1] = items[imin].bmax[1];

	for (int i = imin + 1; i < imax; ++i)
	{
		const BoundsItem& it = items[i];
		if (it.bmin[0] < bmin[0]) bmin[0] = it.bmin[0];
		if (it.bmin[1] < bmin[1]) bmin[1] = it.bmin[1];

		if (it.bmax[0] > bmax[0]) bmax[0] = it.bmax[0];
		if (it.bmax[1] > bmax[1]) bmax[1] = it.bmax[1];
	}
}

inline int longestAxis(float x, float y)
{
	return y > x ? 1 : 0;
}

static void subdivide(BoundsItem* items, int nitems, int imin, int imax, int trisPerChunk, int& curNode, rcChunkyTriMeshNode* nodes, const int maxNodes, int& curTri, int* outTris, const int* inTris)
{
	int inum = imax - imin;
	int icur = curNode;

	if (curNode > maxNodes)
		return;

	rcChunkyTriMeshNode& node = nodes[curNode++];

	if (inum <= trisPerChunk)
	{
		// Leaf
		calcExtends(items, nitems, imin, imax, node.bmin, node.bmax);

		// Copy triangles.
		node.i = curTri;
		node.n = inum;

		for (int i = imin; i < imax; ++i)
		{
			const int* src = &inTris[items[i].i * 3];
			int* dst = &outTris[curTri * 3];
			curTri++;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
		}
	}
	else
	{
		// Split
		calcExtends(items, nitems, imin, imax, node.bmin, node.bmax);

		int	axis = longestAxis(node.bmax[0] - node.bmin[0],
			node.bmax[1] - node.bmin[1]);

		if (axis == 0)
		{
			// Sort along x-axis
			qsort(items + imin, static_cast<size_t>(inum), sizeof(BoundsItem), compareItemX);
		}
		else if (axis == 1)
		{
			// Sort along y-axis
			qsort(items + imin, static_cast<size_t>(inum), sizeof(BoundsItem), compareItemY);
		}

		int isplit = imin + inum / 2;

		// Left
		subdivide(items, nitems, imin, isplit, trisPerChunk, curNode, nodes, maxNodes, curTri, outTris, inTris);
		// Right
		subdivide(items, nitems, isplit, imax, trisPerChunk, curNode, nodes, maxNodes, curTri, outTris, inTris);

		int iescape = curNode - icur;
		// Negative index means escape.
		node.i = -iescape;
	}
}

bool rcCreateChunkyTriMesh(const float* verts, const int* tris, int ntris, int trisPerChunk, rcChunkyTriMesh* cm)
{
	int nchunks = (ntris + trisPerChunk - 1) / trisPerChunk;

	cm->nodes = new rcChunkyTriMeshNode[nchunks * 4];
	if (!cm->nodes)
		return false;

	cm->tris = new int[ntris * 3];
	if (!cm->tris)
		return false;

	cm->ntris = ntris;

	// Build tree
	BoundsItem* items = new BoundsItem[ntris];
	if (!items)
		return false;

	for (int i = 0; i < ntris; i++)
	{
		const int* t = &tris[i * 3];
		BoundsItem& it = items[i];
		it.i = i;
		// Calc triangle XZ bounds.
		it.bmin[0] = it.bmax[0] = verts[t[0] * 3 + 0];
		it.bmin[1] = it.bmax[1] = verts[t[0] * 3 + 2];
		for (int j = 1; j < 3; ++j)
		{
			const float* v = &verts[t[j] * 3];
			if (v[0] < it.bmin[0]) it.bmin[0] = v[0];
			if (v[2] < it.bmin[1]) it.bmin[1] = v[2];

			if (v[0] > it.bmax[0]) it.bmax[0] = v[0];
			if (v[2] > it.bmax[1]) it.bmax[1] = v[2];
		}
	}

	int curTri = 0;
	int curNode = 0;
	subdivide(items, ntris, 0, ntris, trisPerChunk, curNode, cm->nodes, nchunks * 4, curTri, cm->tris, tris);

	delete[] items;

	cm->nnodes = curNode;

	// Calc max tris per node.
	cm->maxTrisPerChunk = 0;
	for (int i = 0; i < cm->nnodes; ++i)
	{
		rcChunkyTriMeshNode& node = cm->nodes[i];
		const bool isLeaf = node.i >= 0;
		if (!isLeaf) continue;
		if (node.n > cm->maxTrisPerChunk)
			cm->maxTrisPerChunk = node.n;
	}

	return true;
}

BuildContext::BuildContext() :
m_messageCount(0),
m_textPoolSize(0)
{
	memset(m_messages, 0, sizeof(char*)* MAX_MESSAGES);

	resetTimers();
}

// Virtual functions for custom implementations.
void BuildContext::doResetLog()
{
	m_messageCount = 0;
	m_textPoolSize = 0;
}

void BuildContext::doLog(const rcLogCategory category, const char* msg, const int len)
{
	if (!len) return;
	if (m_messageCount >= MAX_MESSAGES)
		return;
	char* dst = &m_textPool[m_textPoolSize];
	int n = TEXT_POOL_SIZE - m_textPoolSize;
	if (n < 2)
		return;
	char* cat = dst;
	char* text = dst + 1;
	const int maxtext = n - 1;
	// Store category
	*cat = (char)category;
	// Store message
	const int count = rcMin(len + 1, maxtext);
	memcpy(text, msg, count);
	text[count - 1] = '\0';
	m_textPoolSize += 1 + count;
	m_messages[m_messageCount++] = dst;
}

void BuildContext::doResetTimers()
{
	for (int i = 0; i < RC_MAX_TIMERS; ++i)
		m_accTime[i] = -1;
}

void BuildContext::doStartTimer(const rcTimerLabel label)
{
}

void BuildContext::doStopTimer(const rcTimerLabel label)
{
}

int BuildContext::doGetAccumulatedTime(const rcTimerLabel label) const
{
	return 1;
}

void BuildContext::dumpLog(const char* format, ...)
{
	// Print header.
	va_list ap;
	va_start(ap, format);
	vprintf(format, ap);
	va_end(ap);
	printf("\n");

	// Print messages
	const int TAB_STOPS[4] = { 28, 36, 44, 52 };
	for (int i = 0; i < m_messageCount; ++i)
	{
		const char* msg = m_messages[i] + 1;
		int n = 0;
		while (*msg)
		{
			if (*msg == '\t')
			{
				int count = 1;
				for (int j = 0; j < 4; ++j)
				{
					if (n < TAB_STOPS[j])
					{
						count = TAB_STOPS[j] - n;
						break;
					}
				}
				while (--count)
				{
					putchar(' ');
					n++;
				}
			}
			else
			{
				putchar(*msg);
				n++;
			}
			msg++;
		}
		putchar('\n');
	}
}

int BuildContext::getLogCount() const
{
	return m_messageCount;
}

const char* BuildContext::getLogText(const int i) const
{
	return m_messages[i] + 1;
}

/*
brief :
*/
H3NavMesh::H3NavMesh()
{
	this->m_navQuery = nullptr;
	this->m_navMesh = nullptr;
	this->m_tileCache = nullptr;
	this->m_talloc = nullptr;
	this->m_tcomp = nullptr;
	this->m_tmproc = nullptr;
	this->m_polyPickExt[3] = 0;
	this->m_smoothPath[MAX_SMOOTH * 3] = 0;
	this->m_nsmoothPath = 0;
	this->m_npolys = 0;
	this->m_polys[MAX_POLYS] = 0;

	this->m_ctx = new BuildContext();
	this->m_geom = new InputGeom();

	//	nav mesh
	m_polyPickExt[0] = 2;
	m_polyPickExt[1] = 4;
	m_polyPickExt[2] = 2;
	m_filter.setIncludeFlags(SAMPLE_POLYFLAGS_ALL ^ SAMPLE_POLYFLAGS_DISABLED);
	m_filter.setExcludeFlags(0);

	m_filter.setAreaCost(SAMPLE_POLYAREA_GROUND, 1.0f);
	m_filter.setAreaCost(SAMPLE_POLYAREA_WATER, 10.0f);
	m_filter.setAreaCost(SAMPLE_POLYAREA_ROAD, 1.0f);
	m_filter.setAreaCost(SAMPLE_POLYAREA_DOOR, 1.0f);
	m_filter.setAreaCost(SAMPLE_POLYAREA_GRASS, 2.0f);
	m_filter.setAreaCost(SAMPLE_POLYAREA_JUMP, 1.5f);

	m_talloc = new LinearAllocator(32000);
	m_tcomp = new FastLZCompressor;
	m_tmproc = new MeshProcess;
	m_navQuery = dtAllocNavMeshQuery();
}

/*
brief :
*/
H3NavMesh::~H3NavMesh()
{
	delete m_talloc ;
	delete m_tcomp ;
	delete m_tmproc ;

	dtFreeNavMeshQuery ( m_navQuery ) ;
	dtFreeTileCache ( m_tileCache ) ;
	dtFreeNavMesh ( m_navMesh ) ;
}

/*
brief :
*/
void H3NavMesh::load(iS8* path)
{

	void* fp = SCHalFileOpen(path, "rb");
	if (!fp) {
		iLog("load file error : %s\n", path);
		return;
	}

	// Read header.
	TileCacheSetHeader header;
	SCHalFileRead(fp, &header, sizeof(TileCacheSetHeader));

	if (header.magic != TILECACHESET_MAGIC)
	{
		SCHalFileClose(fp);
		iLog("load file error : %s\n", path);
		return;
	}
	if (header.version != TILECACHESET_VERSION)
	{
		SCHalFileClose(fp);
		iLog("load file error : %s\n", path);
		return;
	}

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{		
		SCHalFileClose(fp);
		iLog("load file error : %s\n", path);
		return;
	}
	dtStatus status = m_navMesh->init(&header.meshParams);
	if (dtStatusFailed(status))
	{
		dtFreeNavMesh ( m_navMesh );
		SCHalFileClose(fp);
		iLog("load file error : %s\n", path);
		return;
	}

	m_tileCache = dtAllocTileCache();
	if (!m_tileCache)
	{
		dtFreeNavMesh ( m_navMesh );
		SCHalFileClose(fp);
		iLog("load file error : %s\n", path);
		return;
	}
	status = m_tileCache->init(&header.cacheParams, m_talloc, m_tcomp, m_tmproc);
	if (dtStatusFailed(status))
	{
		dtFreeTileCache ( m_tileCache );
		dtFreeNavMesh ( m_navMesh );
		SCHalFileClose(fp);
		iLog("load file error : %s\n", path);
		return;
	}

	// Read tiles.
	for (int i = 0; i < header.numTiles; ++i)
	{
		TileCacheTileHeader tileHeader;
		SCHalFileRead(fp, &tileHeader, sizeof(tileHeader));
		if (!tileHeader.tileRef || !tileHeader.dataSize)
			break;

		unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
		if (!data) break;
		memset(data, 0, tileHeader.dataSize);
		SCHalFileRead(fp, data, tileHeader.dataSize);

		dtCompressedTileRef tile = 0;
		m_tileCache->addTile(data, tileHeader.dataSize, DT_COMPRESSEDTILE_FREE_DATA, &tile);

		if (tile)
			m_tileCache->buildNavMeshTile(tile, m_navMesh);
	}

	SCHalFileClose(fp);

	m_navQuery->init(m_navMesh, 2048);
}

/*
brief :
*/
void H3NavMesh::generatePath(H3Vector3f spos, H3Vector3f epos)
{
	float m_spos[3];
	float m_epos[3];

	m_spos[0] = spos.x;
	m_spos[1] = spos.y;
	m_spos[2] = spos.z;

	m_epos[0] = epos.x;
	m_epos[1] = epos.y;
	m_epos[2] = epos.z;

	m_navQuery->findNearestPoly(m_spos, m_polyPickExt, &m_filter, &m_startRef, 0);

	m_navQuery->findNearestPoly(m_epos, m_polyPickExt, &m_filter, &m_endRef, 0);

	m_navQuery->findPath(m_startRef, m_endRef, m_spos, m_epos, &m_filter, m_polys, &m_npolys, MAX_POLYS);

	if (m_npolys)
	{
		// Iterate over the path to find smooth path on the detail mesh surface.
		dtPolyRef polys[MAX_POLYS];
		memcpy(polys, m_polys, sizeof(dtPolyRef)*m_npolys);
		int npolys = m_npolys;

		float iterPos[3], targetPos[3];
		m_navQuery->closestPointOnPoly(m_startRef, m_spos, iterPos, 0);
		m_navQuery->closestPointOnPoly(polys[npolys - 1], m_epos, targetPos, 0);

		static const float STEP_SIZE = 0.5f;
		static const float SLOP = 0.01f;

		m_nsmoothPath = 0;

		dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
		m_nsmoothPath++;

		// Move towards target a small advancement at a time until target reached or
		// when ran out of memory to store the path.
		while (npolys && m_nsmoothPath < MAX_SMOOTH)
		{
			// Find location to steer towards.
			float steerPos[3];
			unsigned char steerPosFlag;
			dtPolyRef steerPosRef;

			if (!getSteerTarget(m_navQuery, iterPos, targetPos, SLOP,
				polys, npolys, steerPos, steerPosFlag, steerPosRef, 0, 0))
				break;

			bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
			bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

			// Find movement delta.
			float delta[3], len;
			dtVsub(delta, steerPos, iterPos);
			len = dtMathSqrtf(dtVdot(delta, delta));
			// If the steer target is end of path or off-mesh link, do not move past the location.
			if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
				len = 1;
			else
				len = STEP_SIZE / len;
			float moveTgt[3];
			dtVmad(moveTgt, iterPos, delta, len);

			// Move
			float result[3];
			dtPolyRef visited[16];
			int nvisited = 0;
			m_navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &m_filter,
				result, visited, &nvisited, 16);

			npolys = fixupCorridor(polys, npolys, MAX_POLYS, visited, nvisited);
			npolys = fixupShortcuts(polys, npolys, m_navQuery);

			float h = 0;
			m_navQuery->getPolyHeight(polys[0], result, &h);
			result[1] = h;
			dtVcopy(iterPos, result);

			// Handle end of path and off-mesh links when close enough.
			if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
			{
				// Reached end of path.
				dtVcopy(iterPos, targetPos);
				if (m_nsmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
					m_nsmoothPath++;
				}
				break;
			}
			else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
			{
				// Reached off-mesh connection.
				float startPos[3], endPos[3];

				// Advance the path up to and over the off-mesh connection.
				dtPolyRef prevRef = 0, polyRef = polys[0];
				int npos = 0;
				while (npos < npolys && polyRef != steerPosRef)
				{
					prevRef = polyRef;
					polyRef = polys[npos];
					npos++;
				}
				for (int i = npos; i < npolys; ++i)
					polys[i - npos] = polys[i];
				npolys -= npos;

				// Handle the connection.
				dtStatus status = m_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
				if (dtStatusSucceed(status))
				{
					if (m_nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
						m_nsmoothPath++;
						// Hack to make the dotted path not visible during off-mesh connection.
						if (m_nsmoothPath & 1)
						{
							dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
							m_nsmoothPath++;
						}
					}
					// Move position at the other side of the off-mesh link.
					dtVcopy(iterPos, endPos);
					float eh = 0.0f;
					m_navQuery->getPolyHeight(polys[0], iterPos, &eh);
					iterPos[1] = eh;
				}
			}

			// Store results.
			if (m_nsmoothPath < MAX_SMOOTH)
			{
				dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
				m_nsmoothPath++;
			}
		}
	} else {
		m_nsmoothPath = 0;
	}

}

//	jelo add start
/*
brief :
*/
void H3NavMesh::generateRandomPath () {
	m_navQuery->findRandomPoint ( &m_filter, frand, &m_startRef, gAgentSpos );
	m_navQuery->findRandomPointAroundCircle ( m_startRef, gAgentSpos, 1000.0f, &m_filter, frand, &m_endRef, gAgentEpos );
	generatePath ( H3Vector3f ( gAgentSpos[0], gAgentSpos[1], gAgentSpos[2] ), H3Vector3f ( gAgentEpos[0], gAgentEpos[1], gAgentEpos[2] ) );
}
//	jelo add end

/*
brief : example
iF32* path = 0 ;
iU32 totallVertex = 0 ;
generatePath ( ... , ... ) ;
getPath ( &path , &totallVertex ) ;
*/
void H3NavMesh::getPath(float** path, int* totallVertex)
{
	*path = m_smoothPath;
	*totallVertex = m_nsmoothPath;
}

void H3NavMesh::save(iS8* path)
{
#if 0
	//if (!m_tileCache) return;

	FILE* fp = fopen(path, "wb+");
	if (!fp)
		return;

	// Store header.
	TileCacheSetHeader header;
	header.magic = TILECACHESET_MAGIC;
	header.version = TILECACHESET_VERSION;
	header.numTiles = 0;
	for (int i = 0; i < m_tileCache->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = m_tileCache->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;
		header.numTiles++;
	}
	memcpy(&header.cacheParams, m_tileCache->getParams(), sizeof(dtTileCacheParams));
	memcpy(&header.meshParams, m_navMesh->getParams(), sizeof(dtNavMeshParams));
	fwrite(&header, sizeof(TileCacheSetHeader), 1, fp);

	// Store tiles.
	for (int i = 0; i < m_tileCache->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = m_tileCache->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;

		TileCacheTileHeader tileHeader;
		tileHeader.tileRef = m_tileCache->getTileRef(tile);
		tileHeader.dataSize = tile->dataSize;
		fwrite(&tileHeader, sizeof(tileHeader), 1, fp);

		fwrite(tile->data, tile->dataSize, 1, fp);
	}

	fclose(fp);
#endif
	void* fp = SCHalFileOpen(path, "wb+");
	if (!fp) {
		iLog("save file error : %s\n", path);
		return;
	}

	// Store header.
	TileCacheSetHeader header;
	header.magic = TILECACHESET_MAGIC;
	header.version = TILECACHESET_VERSION;
	header.numTiles = 0;
	for (int i = 0; i < m_tileCache->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = m_tileCache->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;
		header.numTiles++;
	}
	memcpy(&header.cacheParams, m_tileCache->getParams(), sizeof(dtTileCacheParams));
	memcpy(&header.meshParams, m_navMesh->getParams(), sizeof(dtNavMeshParams));
	SCHalFileWrite(fp, &header, sizeof(TileCacheSetHeader));

	// Store tiles.
	for (int i = 0; i < m_tileCache->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = m_tileCache->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;

		TileCacheTileHeader tileHeader;
		tileHeader.tileRef = m_tileCache->getTileRef(tile);
		tileHeader.dataSize = tile->dataSize;
		SCHalFileWrite(fp, &tileHeader, sizeof(tileHeader));

		SCHalFileWrite(fp, tile->data, tile->dataSize);
	}

	SCHalFileClose(fp);

}
//
static void drawAgent ( const float* pos, float r, float h, float c, float red, float green, float blue, float alpha )
{

	H3DrawPrimitives::setDepthTestEnable ( true );

	// Agent dimensions.	
	H3DrawPrimitives::setColor ( red / 255.0f, green / 255.0f, blue / 255.0f, alpha / 255.0f );
	H3DrawPrimitives::drawCylinderWire ( pos[0] - r, pos[1] + 0.02f, pos[2] - r, pos[0] + r, pos[1] + h, pos[2] + r );

	H3DrawPrimitives::setColor ( 0 , 0 , 0 , 64.0f / 255.0f )  ;
	H3DrawPrimitives::drawCircle ( pos[0], pos[1] + c, pos[2], r );

	H3DrawPrimitives::setColor ( 0, 0, 0, 196.0f / 255.0f );
	H3DrawPrimitives::begin ( H3DrawPrimitives_Line );
	H3DrawPrimitives::addVertex (pos[0], pos[1] - c, pos[2] );
	H3DrawPrimitives::addVertex ( pos[0], pos[1] + c, pos[2]  );
	H3DrawPrimitives::addVertex ( pos[0] - r / 2, pos[1] + 0.02f, pos[2] );
	H3DrawPrimitives::addVertex ( pos[0] + r / 2, pos[1] + 0.02f, pos[2] );
	H3DrawPrimitives::addVertex ( pos[0], pos[1] + 0.02f, pos[2] - r / 2 );
	H3DrawPrimitives::addVertex ( pos[0], pos[1] + 0.02f, pos[2] + r / 2 );
	H3DrawPrimitives::end ( );

	H3DrawPrimitives::setDepthTestEnable ( true );
}
//
//	jelo add start
/*
biref :
*/
void H3NavMesh::addDebugValue ( iF32 agentRadius, iF32 agentHeight, iF32 agentClimb ) {

	gAgentRadius = agentRadius;
	gAgentHeight = agentHeight;
	gAgentClimb = agentClimb;

}
//	jelo add end

void H3NavMesh::debugDraw()
{
	if ( nullptr == m_navMesh)
		return;

	unsigned char flag=0;
	const dtNavMesh* navMesh = m_navMesh;
	for (int i = 0; i < (*m_navMesh).getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = navMesh->getTile(i);
		if (!tile->header) continue;
		drawMeshTile(H3DrawPrimitives_Triangle, (*navMesh), 0, tile, flag);
	}

	//	jelo add start
	H3DrawPrimitives::setColor ( 255.0f / 255.0f, 0 / 255.0f, 0 / 255.0f, 255.0f / 255.0f );
	duDebugDrawNavMeshPoly ( *m_navMesh, m_startRef, 0 );
	H3DrawPrimitives::setColor ( 0 / 255.0f, 0 / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f );
	duDebugDrawNavMeshPoly ( *m_navMesh, m_endRef, 0 );

	if ( m_npolys )
	{
		H3DrawPrimitives::setColor ( 0.0f, 1.0f, 0, 1.0f );
		for ( int i = 0; i < m_npolys; ++i )
		{
			if ( m_polys[i] == m_startRef || m_polys[i] == m_endRef )
				continue;
			duDebugDrawNavMeshPoly ( *m_navMesh, m_polys[i], 0 );
		}
	}
	 
	if ( m_nsmoothPath )
	{
		H3DrawPrimitives::setDepthTestEnable ( false );
		H3DrawPrimitives::setColor ( 1.0f , 0 , 0 , 1.0f ) ;
		H3DrawPrimitives::begin ( H3DrawPrimitives_Line );
		//for ( int i = 0; i < m_nsmoothPath-1; i++ ){
		//	H3DrawPrimitives::addVertex ( m_smoothPath[i * 3], m_smoothPath[i * 3 + 1] + 0.1f, m_smoothPath[i * 3 + 2] );
		//}

		float* testDrawLineArray ;
		int testVeretxTotal = 0 ;
		getPath ( &testDrawLineArray, &testVeretxTotal );
		for ( iS32 walker = 0; walker < testVeretxTotal * 3 - 6; walker += 6 ) {
			H3Vector3f testPointA , testPointB;
			testPointA.x = testDrawLineArray[walker];
			testPointA.y = testDrawLineArray[walker + 1];
			testPointA.z = testDrawLineArray[walker + 2];
			testPointB.x = testDrawLineArray[walker + 3];
			testPointB.y = testDrawLineArray[walker + 4];
			testPointB.z = testDrawLineArray[walker + 5];
			H3DrawPrimitives::setColor ( 255.0f, 0, 0, 255.0f / 255 );
			H3DrawPrimitives::drawLine(testPointA, testPointB);
		}

		H3DrawPrimitives::end ( );
		H3DrawPrimitives::setDepthTestEnable ( true );
	}
	H3DrawPrimitives::setDepthTestEnable ( true );
	if ( gAgentRadius ) {
		drawAgent ( gAgentSpos, gAgentRadius, gAgentHeight, gAgentClimb, 128.0f, 25.0f, 0.0f, 192.0f );
	} if ( gAgentRadius ) {
		drawAgent ( gAgentEpos, gAgentRadius, gAgentHeight, gAgentClimb, 51.0f, 102.0f, 0.0f, 129.0f );
	}
	H3DrawPrimitives::setDepthTestEnable ( true );
	//	jelo add end

}

bool H3NavMesh::regenerateinit(const iS8* path)
{
	if (m_geom->m_mesh)
	{
		delete m_geom->m_chunkyMesh;
		m_geom->m_chunkyMesh = 0;
		delete m_geom->m_mesh;
		m_geom->m_mesh = 0;
	}
	m_geom->m_offMeshConCount = 0;
	m_geom->m_volumeCount = 0;

	m_geom->m_mesh = new rcMeshLoaderObj;
	if (!m_geom->m_mesh)
	{
		iLog("loadMesh: Out of memory 'm_mesh'.\n");
		delete m_geom->m_mesh;
		return false;
	}
	if (!m_geom->m_mesh->load(path))
	{
		iLog("buildTiledNavigation: Could not load '%s'\n", path);
		delete m_geom->m_mesh;
		return false;
	}

	rcCalcBounds(m_geom->m_mesh->getVerts(), m_geom->m_mesh->getVertCount(), m_geom->m_meshBMin, m_geom->m_meshBMax);

	m_geom->m_chunkyMesh = new rcChunkyTriMesh;
	if (!m_geom->m_chunkyMesh)
	{
		iLog("buildTiledNavigation: Out of memory 'm_chunkyMesh'.\n");
		delete m_geom->m_mesh;
		delete m_geom->m_chunkyMesh;
		return false;
	}
	if (!rcCreateChunkyTriMesh(m_geom->m_mesh->getVerts(), m_geom->m_mesh->getTris(), m_geom->m_mesh->getTriCount(), 256, m_geom->m_chunkyMesh))
	{
		iLog("buildTiledNavigation: Failed to build chunky mesh.\n");
		delete m_geom->m_mesh;
		delete m_geom->m_chunkyMesh;
		return false;
	}

	return true;
}

/*
biref :
*/
void H3NavMesh::regenerate(rcConfig* cfg, dtTileCacheParams* tcparams, dtNavMeshParams* params, H3Object* object, const iS8* path)
{
	regenerateinit ( path );

	dtStatus status;

	m_tmproc->init(m_geom);

	// Init cache
	const float* bmin = m_geom->getNavMeshBoundsMin();
	const float* bmax = m_geom->getNavMeshBoundsMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, cfg->cs, &gw, &gh);
	int ts = (int)cfg->tileSize;
	int tw = (gw + ts - 1) / ts;
	int th = (gh + ts - 1) / ts;
	tcparams->maxTiles = tw*th*EXPECTED_LAYERS_PER_TILE;

	// Max tiles and max polys affect how the tile IDs are caculated.
	// There are 22 bits available for identifying a tile and a polygon.
	int tileBits = rcMin((int)dtIlog2(dtNextPow2(tw*th*EXPECTED_LAYERS_PER_TILE)), 14);
	if (tileBits > 14) tileBits = 14;
	int polyBits = 22 - tileBits;
	int m_maxTiles = 1 << tileBits;
	int m_maxPolysPerTile = 1 << polyBits;
	int gridSize = tw*th;

	dtFreeTileCache(m_tileCache);

	m_tileCache = dtAllocTileCache();

	if (!m_tileCache)
	{
		iLog("buildTiledNavigation: Could not allocate tile cache.\n");
		return ;
	}
	status = m_tileCache->init(tcparams, m_talloc, m_tcomp, m_tmproc);
	if (dtStatusFailed(status))
	{
		iLog("buildTiledNavigation: Could not init tile cache.\n");
		return ;
	}

	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh)
	{
		iLog("buildTiledNavigation: Could not allocate navmesh.\n");
		return ;
	}

	rcVcopy(params->orig, bmin);
	params->tileWidth = cfg->tileSize * cfg->cs;
	params->tileHeight = cfg->tileSize * cfg->cs;
	params->maxTiles = m_maxTiles;
	params->maxPolys = m_maxPolysPerTile;
		  
	status = m_navMesh->init(params);
	if (dtStatusFailed(status))
	{
		iLog("buildTiledNavigation: Could not init navmesh.\n");
		return ;
	}

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status))
	{
		iLog("buildTiledNavigation: Could not init Detour navmesh query\n");
		return ;
	}


	// Preprocess tiles.

	m_ctx->resetTimers();

	gCacheLayerCount = 0;
	gCacheCompressedSize = 0;
	gCacheRawSize = 0;

	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			TileCacheData tiles[MAX_LAYERS];
			memset(tiles, 0, sizeof(tiles));
			int ntiles = rasterizeTileLayers(m_ctx, m_geom, x, y, *cfg, tiles, MAX_LAYERS);

			for (int i = 0; i < ntiles; ++i)
			{
				TileCacheData* tile = &tiles[i];
				status = m_tileCache->addTile(tile->data, tile->dataSize, DT_COMPRESSEDTILE_FREE_DATA, 0);
				if (dtStatusFailed(status))
				{
					dtFree(tile->data);
					tile->data = 0;
					continue;
				}

				gCacheLayerCount++;
				gCacheCompressedSize += tile->dataSize;
				gCacheRawSize += calcLayerBufferSize(tcparams->width, tcparams->height);
			}
		}
	}

	// Build initial meshes
	m_ctx->startTimer(RC_TIMER_TOTAL);
	for (int y = 0; y < th; ++y)
	for (int x = 0; x < tw; ++x)
		m_tileCache->buildNavMeshTilesAt(x, y, m_navMesh);
	m_ctx->stopTimer(RC_TIMER_TOTAL);

	gCacheBuildTimeMs = m_ctx->getAccumulatedTime(RC_TIMER_TOTAL) / 1000.0f;
	gCacheBuildMemUsage = m_talloc->high;


	const dtNavMesh* nav = m_navMesh;
	int navmeshMemUsage = 0;
	for (int i = 0; i < nav->getMaxTiles(); ++i)
	{
		const dtMeshTile* tile = nav->getTile(i);
		if (tile->header)
			navmeshMemUsage += tile->dataSize;
	}
	printf("navmeshMemUsage = %.1f kB", navmeshMemUsage / 1024.0f);

	iS8 saveFilePath[256];

	iS8 fileName[128];
	int walker = 0;
	int count = 0;
	while (path[walker] != '.')
	{

		if (path[walker] == '/' || count != 0) {
			fileName[count] = path[walker + 1];
			count++;
		}
		walker++;
	}

	fileName[count] = '\0';

	sprintf(saveFilePath, "models/%snav", fileName);
	save(saveFilePath);

	//if (m_tool)
	//	m_tool->init(this);
	//initToolStates(this);

	return ;

}

/*
biref :
*/
void H3NavMesh::cleanData()
{
	m_nsmoothPath = 0;
	m_npolys = 0;
	m_smoothPath[MAX_SMOOTH * 3] = 0;
	m_polys[MAX_POLYS] = 0;
	m_navMesh = nullptr;
}