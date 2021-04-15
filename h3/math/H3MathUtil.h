
/*

	+	H3MathUtil.h
	+	QDL

	+	hello3d
	+	realtime 3d engine

	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3MathUtil_H
#define H3MathUtil_H

#include "H3Std.h"
#include "H3Math.h"
#include "H3Eular.h"
#include "H3Matrix.h"
#include "H3Quaternion.h"
#include "H3Transform.h"
#include "H3Vector.h"

/*
 brief :
*/
typedef struct
{
	iF32 x ;
	iF32 y ;
	iF32 z ;
} H3QuadVertex ;

/*
 brief :
*/
typedef struct
{
	iF32 x ;
	iF32 y ;
} H3QuadTexCoord ;

/*
 brief :
*/
typedef struct
{
	H3QuadVertex vertex ;
	H3QuadTexCoord texCoord ;
} H3Quad ;

/*
 brief :
*/
typedef struct
{
	iU32 index ;
} H3Indices ;

enum class H3MATRIX_TYPE
{
	//	camera projection matrix
    H3_MATRIX_PROJECTION ,
	//	camera view projection matrix
    H3_MATRIX_VIEW_PROJECTION ,
};

#endif