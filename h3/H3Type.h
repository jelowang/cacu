
/*

	+	H3Type.h
	+	QDL

	+	hello3d
	+	realtime 3d engine
																				
 
	+	(C)	Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3Type_H
#define H3Type_H

#include <assert.h>

typedef unsigned char iU8;
typedef char iS8;
typedef unsigned short iU16;
typedef short iS16;
typedef unsigned int iU32;
typedef int iS32;
typedef unsigned long long iU64;
typedef long iS64;
typedef float iF32;
typedef double iD64;
typedef void* iHANDLE ;
#define iFILE void*

/*
	brief : A texcoord composed of 2 floats: u , v
*/
typedef struct {
	iF32 u ;
	iF32 v ;
} H3_TEX2F;

/*
	brief : RGBA color composed of 4 floats
*/
typedef struct {
	iF32 r ;
	iF32 g ;
	iF32 b ;
	iF32 a ;
} H3_C4F ;

/*
	brief : A value composed of 3 floats: x , y , z 
*/
typedef struct {
	iF32 x ;
	iF32 y ;
	iF32 z ;
} H3_V3F;

/*
	brief : a point with a vertex point , a tex coord point and a color 4F
*/
typedef struct {

	H3_V3F vertex ;
	H3_TEX2F texCoord ;
	H3_C4F color ;

} H3_Quad_C4F_3 ;

/*
	brief : a point with a vertex point , a tex coord point and normal and tangent
*/
typedef struct {

	H3_V3F vertex ;
	H3_TEX2F texCoord ;
	H3_V3F normal ;
	H3_V3F tangent ;

#ifdef FACIAL_TEST
	H3_V3F vertexBlendShape1 ;
	H3_TEX2F texCoordBlendShape1 ;
	H3_V3F normalBlendShape1 ;
	H3_V3F tangentBlendShape1 ;
#endif

} H3_Quad_4 ;
/*
	brief : a point with a vertex point , a tex coord point and a color 4F and normal and tangent
*/
typedef struct {

	H3_V3F vertex ;
	H3_TEX2F texCoord ;
	H3_C4F color ;
	H3_V3F normal ;
	H3_V3F tangent ;

} H3_Quad_5 ;

typedef struct 
{
    //! top left 
    H3_Quad_C4F_3 tl; 
    //! bottom left
    H3_Quad_C4F_3 bl; 
    //! top right
    H3_Quad_C4F_3 tr;
    //! bottom right
    H3_Quad_C4F_3 br;
} H3_V3F_T2F_C4F_Quad ;

typedef struct 
{
    H3_Quad_4 tl ;
    H3_Quad_4 tr ;
    H3_Quad_4 bl ;
    H3_Quad_4 br ;
} H3_Quad_4_QuadGroup ;

typedef enum 
{
	H3_RGB_FORMAT_RGB24 = 0 , 
	H3_RGB_FORMAT_RGBA32 , 
} H3_RGB_FORMAT ;

#define H3ASSERT(x) assert(x)

//	use for render to texture
#define H3_RENDER_FBO_SHADOW "shadowFbo"
#define H3_RENDER_SHADER_SHADOW "shadowShader"
#define H3_RENDER_TEXTURE_RENDER "renderTexture"
#define H3_RENDER_RENDER_BUFFER "renderBuffer"
#define H3_RENDER_FBO_RENDER "renderFbo"

#endif