
/*

	+	H3DrawPrimitives.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Std.h"
#include "H3Gl.h"
#include "H3DrawPrimitives.h"
#include "H3ShaderCache.h"

static H3Shader* gShader = 0 ;
static H3Vector3f gColor = H3Vector3f(0.0,0.0,0.0) ;
static iF32 gAlpha = 1.0f ;
static iS32 gColorLocation = -1 ;   
static iS32 gAlphaLocation = -1 ; 
static iU32 gVao = 0 ;
static iU32 gPositionVbo = 0 ;

static bool depthTestEnable = false ;
static iF32* vertexsArray = 0 ;
static iU32 vertexsArrayTotall = 0 ;
static iU32 vertexsArrayCapacity = 0 ;
static iU32 gPrimitivesType = H3DrawPrimitives_Point ;  

#define VERTEXS_ARRAY_DEFUALT_CAPACITY 1 * 3

static inline void evalArc ( const float x0, const float y0, const float z0,
	const float dx, const float dy, const float dz,
	const float h, const float u, float* res )
{
	res[0] = x0 + dx * u;
	res[1] = y0 + dy * u + h * (1 - (u * 2 - 1)*(u * 2 - 1));
	res[2] = z0 + dz * u;
}

static inline void vcross ( float* dest, const float* v1, const float* v2 )
{
	dest[0] = v1[1] * v2[2] - v1[2] * v2[1];
	dest[1] = v1[2] * v2[0] - v1[0] * v2[2];
	dest[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

static inline void vnormalize ( float* v )
{
	float d = 1.0f / sqrtf ( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] );
	v[0] *= d;
	v[1] *= d;
	v[2] *= d;
}

static inline void vsub ( float* dest, const float* v1, const float* v2 )
{
	dest[0] = v1[0] - v2[0];
	dest[1] = v1[1] - v2[1];
	dest[2] = v1[2] - v2[2];
}

static inline float vdistSqr ( const float* v1, const float* v2 )
{
	const float x = v1[0] - v2[0];
	const float y = v1[1] - v2[1];
	const float z = v1[2] - v2[2];
	return x*x + y*y + z*z;
}

static void appendArrowHead ( const float* p, const float* q, const float s )
{
	const float eps = 0.001f;
	if ( vdistSqr ( p, q ) < eps*eps ) return;
	float ax[3], ay[3] = { 0, 1, 0 }, az[3];
	vsub ( az, q, p );
	vnormalize ( az );
	vcross ( ax, ay, az );
	vcross ( ay, az, ax );
	vnormalize ( ay );

	H3DrawPrimitives::addVertex ( p[0], p[1], p[2] );
	H3DrawPrimitives::addVertex ( p[0] + az[0] * s + ax[0] * s / 3, p[1] + az[1] * s + ax[1] * s / 3, p[2] + az[2] * s + ax[2] * s / 3 );

	H3DrawPrimitives::addVertex ( p[0], p[1], p[2] );
	H3DrawPrimitives::addVertex ( p[0] + az[0] * s - ax[0] * s / 3, p[1] + az[1] * s - ax[1] * s / 3, p[2] + az[2] * s - ax[2] * s / 3 );

}

static void clearn() {
	vertexsArrayTotall = 0;
}

static void resize(iU32 size) {

	iF32* mem = 0 ; 

	if (0 == vertexsArray) {
		vertexsArray = (iF32*)MMC_MALLOC(size * sizeof(iF32));
		if (0 == vertexsArray) iLog("FE:not enough memory H3DrawPrimitives::resize");
		else vertexsArrayCapacity = size ;
		return;
	}

	iF32* oldmem = vertexsArray;

	mem  = (iF32*)MMC_MALLOC(size * sizeof(iF32));

	if (0 == mem) {
		iLog("FE:not enough memory H3DrawPrimitives::resize");
		return;
	}

	memcpy(mem, vertexsArray, vertexsArrayTotall) ;	
	MMC_FREE(oldmem);
	vertexsArray = mem;

	vertexsArrayCapacity = size;

}

void H3DrawPrimitives::init()
{
	h3glGenVertexArrays ( 1 , &gVao ) ;
	h3glGenBuffers ( 1 , &gPositionVbo ) ;
	gShader = H3ShaderCache::getInstance()->getShader ( H3_SHADER_CACHE_PRIMITIVES ) ;
	gColorLocation = gShader->getUniformLocation ( "color" ) ;
	gAlphaLocation = gShader->getUniformLocation ( "alpha" ) ;

	depthTestEnable = true;
	vertexsArray = nullptr;
	vertexsArrayTotall = 0;
	vertexsArrayCapacity = VERTEXS_ARRAY_DEFUALT_CAPACITY;
	resize(VERTEXS_ARRAY_DEFUALT_CAPACITY);

} 

void H3DrawPrimitives::release()
{
	h3glDeleteVertexArrays ( 1 , &gVao ) ;
	h3glDeleteBuffers ( 1 , &gPositionVbo ) ;

	if (nullptr != vertexsArray)
		MMC_FREE(vertexsArray);
} 

void H3DrawPrimitives::setColor(iF32 r, iF32 g, iF32 b, iF32 a)
{ 
	gColor.init ( r , g , b ) ;
	gAlpha = a ;
}

void H3DrawPrimitives::addVertex(iF32 x, iF32 y, iF32 z) {
	
	if ( nullptr == vertexsArray )
		return;

	if (vertexsArrayTotall >= vertexsArrayCapacity) {
		resize(vertexsArrayTotall+VERTEXS_ARRAY_DEFUALT_CAPACITY) ;
	}

	if (vertexsArrayTotall < vertexsArrayCapacity) {
		vertexsArray[vertexsArrayTotall] = x ;
		vertexsArray[vertexsArrayTotall+1] = y ;
		vertexsArray[vertexsArrayTotall+2] = z ;
		vertexsArrayTotall = vertexsArrayTotall + 3 ;
	}

}

void H3DrawPrimitives::drawCircle ( iF32 x, iF32 y, iF32 z, iF32 r ) {

	static const int NUM_SEG = 40;
	static float dir[40 * 2];
	static bool init = false;

	begin ( H3DrawPrimitives_Line ) ;

	if ( !init )
	{
		init = true;
		for ( int i = 0; i < NUM_SEG; ++i )
		{
			const float a = (float)i / (float)NUM_SEG*H32Pi;
			dir[i * 2] = cosf ( a );
			dir[i * 2 + 1] = sinf ( a );
		}
	}

	for ( int i = 0, j = NUM_SEG - 1; i < NUM_SEG; j = i++ )
	{
		addVertex ( x + dir[j * 2 + 0] * r, y, z + dir[j * 2 + 1] * r ) ;
		addVertex ( x + dir[i * 2 + 0] * r, y, z + dir[i * 2 + 1] * r ) ;
	}

	end () ;

}

void H3DrawPrimitives::drawCylinderWire ( iF32 minx, iF32 miny, iF32 minz, iF32 maxx, iF32 maxy, iF32 maxz ) {

	static const int NUM_SEG = 16;
	static float dir[NUM_SEG * 2];
	static bool init = false;

	begin ( H3DrawPrimitives_Line );

	if ( !init )
	{
		init = true;
		for ( int i = 0; i < NUM_SEG; ++i )
		{
			const float a = (float)i / (float)NUM_SEG*H32Pi ;
			dir[i * 2] = cosf ( a );
			dir[i * 2 + 1] = sinf ( a );
		}
	}
									   
	const float cx = (maxx + minx) / 2 ;
	const float cz = (maxz + minz) / 2 ;
	const float rx = (maxx - minx) / 2 ;
	const float rz = (maxz - minz) / 2 ;
									   
	for ( int i = 0, j = NUM_SEG - 1; i < NUM_SEG; j = i++ )
	{
		addVertex ( cx + dir[j * 2 + 0] * rx, miny, cz + dir[j * 2 + 1] * rz ) ;
		addVertex ( cx + dir[i * 2 + 0] * rx, miny, cz + dir[i * 2 + 1] * rz ) ;
		addVertex ( cx + dir[j * 2 + 0] * rx, maxy, cz + dir[j * 2 + 1] * rz ) ;
		addVertex ( cx + dir[i * 2 + 0] * rx, maxy, cz + dir[i * 2 + 1] * rz ) ;
	}																		   
	for ( int i = 0; i < NUM_SEG; i += NUM_SEG / 4 )						   
	{																		   
		addVertex ( cx + dir[i * 2 + 0] * rx, miny, cz + dir[i * 2 + 1] * rz ) ;
		addVertex ( cx + dir[i * 2 + 0] * rx, maxy, cz + dir[i * 2 + 1] * rz ) ;
	}		

	end ( );

}

void H3DrawPrimitives::drawArc ( iF32 x0, iF32 y0, iF32 z0, iF32 x1, iF32 y1, iF32 z1, iF32 h, iF32 as0, iF32 as1  ) {

	static const int NUM_ARC_PTS = 8;
	static const float PAD = 0.05f;
	static const float ARC_PTS_SCALE = (1.0f - PAD * 2) / (float)NUM_ARC_PTS;
	const float dx = x1 - x0;
	const float dy = y1 - y0;
	const float dz = z1 - z0;
	const float len = sqrtf ( dx*dx + dy*dy + dz*dz );
	float prev[3];

	H3DrawPrimitives::begin ( H3DrawPrimitives_Line );

	evalArc ( x0, y0, z0, dx, dy, dz, len*h, PAD, prev );
	for ( int i = 1; i <= NUM_ARC_PTS; ++i )
	{
		const float u = PAD + i * ARC_PTS_SCALE;
		float pt[3];
		evalArc ( x0, y0, z0, dx, dy, dz, len*h, u, pt );
		H3DrawPrimitives::addVertex ( prev[0], prev[1], prev[2] );
		H3DrawPrimitives::addVertex ( pt[0], pt[1], pt[2] );
		prev[0] = pt[0]; prev[1] = pt[1]; prev[2] = pt[2];
	}

	// End arrows
	if ( as0 > 0.001f )
	{
		float p[3], q[3];
		evalArc ( x0, y0, z0, dx, dy, dz, len*h, PAD, p );
		evalArc ( x0, y0, z0, dx, dy, dz, len*h, PAD + 0.05f, q );
		appendArrowHead ( p, q, as0  );
	}

	if ( as1 > 0.001f )
	{
		float p[3], q[3];
		evalArc ( x0, y0, z0, dx, dy, dz, len*h, 1 - PAD, p );
		evalArc ( x0, y0, z0, dx, dy, dz, len*h, 1 - (PAD + 0.05f), q );
		appendArrowHead ( p, q, as1  );
	}

	H3DrawPrimitives::end () ;

}

void H3DrawPrimitives::drawLine(H3Vector3f origin, H3Vector3f destination)
{

	h3glEnable ( GL_DEPTH_TEST ) ;

	iF32 vertices [6] =
	{
		origin.x , origin.y , origin.z ,
		destination.x , destination.y , destination.z ,
	} ;
	
	iF32 color[3] = { gColor.x , gColor.y , gColor.z } ;

	gShader->use () ;
	gShader->setUniformsForBuiltins () ;
	gShader->setUniformLocationWith3f ( gColorLocation , color ) ;
	gShader->setUniformLocationWith1f ( gAlphaLocation , gAlpha ) ;

	h3glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA ) ;

	h3glBindVertexArray ( gVao ) ;
	h3glBindBuffer ( GL_ARRAY_BUFFER , gPositionVbo ) ;
	h3glBufferData ( GL_ARRAY_BUFFER , 6 * sizeof(iF32) , vertices , GL_STATIC_DRAW ) ;

    h3glEnableVertexAttribArray ( 0 ) ;  
    h3glVertexAttribPointer ( 0 , 3 , GL_FLOAT , false , 3 * sizeof(iF32) , 0 ) ;
	glDrawArrays ( GL_LINES, 0 , 2 ); 
    	
	//  release
    h3glDisableVertexAttribArray ( 0 ) ; 
	h3glBindBuffer ( GL_ARRAY_BUFFER , 0 ) ;
	h3glBindVertexArray ( 0 ) ;
	
	h3glDisable ( GL_DEPTH_TEST ) ;

	H3_GL_DRAW_RECORD_DRAWCALL(1) 
	H3_GL_DRAW_RECORD_VERTICES(2)
	H3_GL_ERROR_CHECK()

}


void H3DrawPrimitives::begin (H3DrawPrimitivesType type) {
	gPrimitivesType = type ;
}

void H3DrawPrimitives::draw () {

	if (depthTestEnable )
		h3glEnable(GL_DEPTH_TEST); 
	else
		h3glDisable(GL_DEPTH_TEST);

	iF32 color[3] = { gColor.x, gColor.y, gColor.z };

	gShader->use();
	gShader->setUniformsForBuiltins();
	gShader->setUniformLocationWith3f(gColorLocation, color);
	gShader->setUniformLocationWith1f(gAlphaLocation, gAlpha);

	h3glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	h3glBindVertexArray(gVao);
	h3glBindBuffer(GL_ARRAY_BUFFER, gPositionVbo);
	h3glBufferData(GL_ARRAY_BUFFER, vertexsArrayTotall * sizeof(iF32), vertexsArray, GL_STATIC_DRAW);

	h3glEnableVertexAttribArray(0);
	h3glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(iF32), 0);
#ifdef WIN32
	if ( H3DrawPrimitives_Point == gPrimitivesType )
		glDrawArrays ( GL_POINT , 0 , vertexsArrayTotall ) ;
	else if ( H3DrawPrimitives_Line == gPrimitivesType )
		glDrawArrays ( GL_LINES , 0 , vertexsArrayTotall / 2 ) ;
	else if ( H3DrawPrimitives_Triangle == gPrimitivesType )
		glDrawArrays ( GL_TRIANGLES , 0 , vertexsArrayTotall / 3) ;
#else
	if ( H3DrawPrimitives_Line == gPrimitivesType )
		glDrawArrays ( GL_LINES , 0 , vertexsArrayTotall / 2 ) ;
	else if ( H3DrawPrimitives_Triangle == gPrimitivesType )
		glDrawArrays ( GL_TRIANGLES , 0 , vertexsArrayTotall / 3) ;
#endif
	//  release
	h3glDisableVertexAttribArray(0);
	h3glBindBuffer(GL_ARRAY_BUFFER, 0);
	h3glBindVertexArray(0);

	h3glDisable(GL_DEPTH_TEST);

	H3_GL_DRAW_RECORD_DRAWCALL(1)
	H3_GL_DRAW_RECORD_VERTICES(2)
	H3_GL_ERROR_CHECK()

}
 
void H3DrawPrimitives::end() {
	draw () ;
	clearn () ;
}
void H3DrawPrimitives::setDepthTestEnable(bool enable) {
	depthTestEnable = enable;
}
