
/*

+	H3NavMesh.h
+	QDL

+	hello3d
+	realtime 3d engine


+	(C) Quantum Dynamics Lab.
	FACEGOOD.
+

*/

#ifndef H3NavMesh_H
#define H3NavMesh_H

#include "H3Std.h"
#include "H3Vector.h"
#include "H3AABB.h"
#include "H3Mesh.h"
#include "Recast.h"
#include "DetourTileCache.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "MeshLoaderObj.h"

#define MAX_POLYS 256
#define MAX_SMOOTH 2048
#define MAX_LAYERS 32
#define MAX_CONVEXVOL_PTS 12
#define EXPECTED_LAYERS_PER_TILE 4 

struct rcChunkyTriMeshNode
{
	float bmin[2];
	float bmax[2];
	int i;
	int n;
};

struct rcChunkyTriMesh
{
	inline rcChunkyTriMesh() : nodes(0), nnodes(0), tris(0), ntris(0), maxTrisPerChunk(0) {};
	inline ~rcChunkyTriMesh() { delete[] nodes; delete[] tris; }

	rcChunkyTriMeshNode* nodes;
	int nnodes;
	int* tris;
	int ntris;
	int maxTrisPerChunk;

private:
	// Explicitly disabled copy constructor and copy assignment operator.
	rcChunkyTriMesh(const rcChunkyTriMesh&);
	rcChunkyTriMesh& operator=(const rcChunkyTriMesh&);
};

struct BuildSettings
{
	// Cell size in world units
	float cellSize;
	// Cell height in world units
	float cellHeight;
	// Agent height in world units
	float agentHeight;
	// Agent radius in world units
	float agentRadius;
	// Agent max climb in world units
	float agentMaxClimb;
	// Agent max slope in degrees
	float agentMaxSlope;
	// Region minimum size in voxels.
	// regionMinSize = sqrt(regionMinArea)
	float regionMinSize;
	// Region merge size in voxels.
	// regionMergeSize = sqrt(regionMergeArea)
	float regionMergeSize;
	// Edge max length in world units
	float edgeMaxLen;
	// Edge max error in voxels
	float edgeMaxError;
	float vertsPerPoly;
	// Detail sample distance in voxels
	float detailSampleDist;
	// Detail sample max error in voxel heights.
	float detailSampleMaxError;
	// Partition type, see SamplePartitionType
	int partitionType;
	// Bounds of the area to mesh
	float navMeshBMin[3];
	float navMeshBMax[3];
	// Size of the tiles in voxels
	float tileSize;
};

struct ConvexVolume
{
	float verts[MAX_CONVEXVOL_PTS * 3];
	float hmin, hmax;
	int nverts;
	int area;
};

class InputGeom
{
public:
	rcChunkyTriMesh* m_chunkyMesh;

	rcMeshLoaderObj* m_mesh;
	float m_meshBMin[3], m_meshBMax[3];
	BuildSettings m_buildSettings;
	bool m_hasBuildSettings;
	/// @name Off-Mesh connections.
	///@{
	static const int MAX_OFFMESH_CONNECTIONS = 256;
	float m_offMeshConVerts[MAX_OFFMESH_CONNECTIONS * 3 * 2];
	float m_offMeshConRads[MAX_OFFMESH_CONNECTIONS];
	unsigned char m_offMeshConDirs[MAX_OFFMESH_CONNECTIONS];
	unsigned char m_offMeshConAreas[MAX_OFFMESH_CONNECTIONS];
	unsigned short m_offMeshConFlags[MAX_OFFMESH_CONNECTIONS];
	unsigned int m_offMeshConId[MAX_OFFMESH_CONNECTIONS];
	int m_offMeshConCount;
	///@}

	/// @name Convex Volumes.
	///@{
	static const int MAX_VOLUMES = 256;
	ConvexVolume m_volumes[MAX_VOLUMES];
	int m_volumeCount;
	///@}

	bool loadMesh(class rcContext* ctx, const std::string& filepath);
	bool loadGeomSet(class rcContext* ctx, const std::string& filepath);
public:
	InputGeom::InputGeom() :
		m_chunkyMesh(0),
		m_mesh(0),
		m_hasBuildSettings(false),
		m_offMeshConCount(0),
		m_volumeCount(0)
	{
	}

	InputGeom::~InputGeom()
	{
		delete m_chunkyMesh;
		delete m_mesh;
	}


	bool load(class rcContext* ctx, const std::string& filepath);
	bool saveGeomSet(const BuildSettings* settings);

	/// Method to return static mesh data.
	const rcMeshLoaderObj* getMesh() const { return m_mesh; }
	const float* getMeshBoundsMin() const { return m_meshBMin; }
	const float* getMeshBoundsMax() const { return m_meshBMax; }
	const float* getNavMeshBoundsMin() const {
		return m_hasBuildSettings ? m_buildSettings.navMeshBMin : m_meshBMin;
	}
	const float* getNavMeshBoundsMax() const {
		return m_hasBuildSettings ? m_buildSettings.navMeshBMax : m_meshBMax;
	}
	const rcChunkyTriMesh* getChunkyMesh() const { return m_chunkyMesh; }
	const BuildSettings* getBuildSettings() const { return m_hasBuildSettings ? &m_buildSettings : 0; }
	bool raycastMesh(float* src, float* dst, float& tmin);

	/// @name Off-Mesh connections.
	///@{
	int getOffMeshConnectionCount() const { return m_offMeshConCount; }
	const float* getOffMeshConnectionVerts() const { return m_offMeshConVerts; }
	const float* getOffMeshConnectionRads() const { return m_offMeshConRads; }
	const unsigned char* getOffMeshConnectionDirs() const { return m_offMeshConDirs; }
	const unsigned char* getOffMeshConnectionAreas() const { return m_offMeshConAreas; }
	const unsigned short* getOffMeshConnectionFlags() const { return m_offMeshConFlags; }
	const unsigned int* getOffMeshConnectionId() const { return m_offMeshConId; }
	void addOffMeshConnection(const float* spos, const float* epos, const float rad,
		unsigned char bidir, unsigned char area, unsigned short flags);
	void deleteOffMeshConnection(int i);
	void drawOffMeshConnections(struct duDebugDraw* dd, bool hilight = false);
	///@}

	/// @name Box Volumes.
	///@{
	int getConvexVolumeCount() const { return m_volumeCount; }
	const ConvexVolume* getConvexVolumes() const { return m_volumes; }
	void addConvexVolume(const float* verts, const int nverts,
		const float minh, const float maxh, unsigned char area);
	void deleteConvexVolume(int i);
	void drawConvexVolumes(struct duDebugDraw* dd, bool hilight = false);
	///@}

private:
	// Explicitly disabled copy constructor and copy assignment operator.
	InputGeom(const InputGeom&);
	InputGeom& operator=(const InputGeom&);
};

class BuildContext : public rcContext
{
	//TimeVal m_startTime[RC_MAX_TIMERS];
	//TimeVal m_accTime[RC_MAX_TIMERS];

	long  m_startTime[RC_MAX_TIMERS];
	long m_accTime[RC_MAX_TIMERS];

	static const int MAX_MESSAGES = 1000;
	const char* m_messages[MAX_MESSAGES];
	int m_messageCount;
	static const int TEXT_POOL_SIZE = 8000;
	char m_textPool[TEXT_POOL_SIZE];
	int m_textPoolSize;

public:
	BuildContext();

	/// Dumps the log to stdout.
	void dumpLog(const char* format, ...);
	/// Returns number of log messages.
	int getLogCount() const;
	/// Returns log message text.
	const char* getLogText(const int i) const;

protected:
	/// Virtual functions for custom implementations.
	///@{
	virtual void doResetLog();
	virtual void doLog(const rcLogCategory category, const char* msg, const int len);
	virtual void doResetTimers();
	virtual void doStartTimer(const rcTimerLabel label);
	virtual void doStopTimer(const rcTimerLabel label);
	virtual int doGetAccumulatedTime(const rcTimerLabel label) const;
	///@}
};

class H3NavMesh : virtual public H3Memory
{
public:
	
	H3NavMesh();
	~H3NavMesh();
	
	/*
	 brief :
	*/
	void load( iS8* path ) ;

	/*
	 brief :
	*/
	void generatePath(H3Vector3f spos, H3Vector3f epos);

	/*
	 brief : 
	*/
	void generateRandomPath () ;

	/*
	brief : example  
			iF32* path = 0 ;
			iU32 totallVertex = 0 ;
			generatePath ( ... , ... ) ;
			getPath ( &path , &totallVertex ) ;
	*/
	void getPath ( float** path , int* totallVertex ) ;

	/*
	brief :
	*/
	void save(iS8* path);

	//	jelo add start
	/*
	biref :
	*/
	void addDebugValue ( iF32 agentRadius, iF32 agentHeight, iF32 agentClimb );
	//	jelo add end

	/*
	biref :
	*/
	void debugDraw () ;

	/*
	 biref :
	*/
	void regenerate(rcConfig* cfg, dtTileCacheParams* tcparams, dtNavMeshParams* params, H3Object* object,const iS8* path = 0 );

	/*
	 biref :
	*/
	void cleanData ();

private:

	bool regenerateinit(const iS8* path);

	dtNavMeshQuery* m_navQuery;
	dtNavMesh* m_navMesh;
	dtTileCache* m_tileCache;
	struct LinearAllocator* m_talloc;
	struct FastLZCompressor* m_tcomp;
	struct MeshProcess* m_tmproc ;
	float m_polyPickExt[3];
	dtQueryFilter m_filter;
	dtPolyRef m_startRef;
	dtPolyRef m_endRef;
	float m_smoothPath[MAX_SMOOTH * 3];
	int m_nsmoothPath;
	int m_npolys;
	dtPolyRef m_polys[MAX_POLYS];
	rcMeshLoaderObj grcMeshLoaderObj;
	BuildContext* m_ctx;
	InputGeom* m_geom;
};

#endif