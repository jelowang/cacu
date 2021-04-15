
/*

	+	H3Math.h
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

#ifndef H3Math_H
#define H3Math_H

#include <math.h>

#define H3Tolerance	2e-37f
#define epsino 0.000001
#define H3MathSqrt(x) sqrt(x)
#define H3MathZero(x) (-epsino <= x && x <= epsino)
#define H3Pi 3.14159265f
#define H32Pi 2.0 * H3Pi 
#define H31OverPi 1.0f / H3Pi 
#define H31Over2Pi 1.0f / H32Pi

extern float H3MathWrapPi ( float theta ) ;
extern float H3Clampf ( float value , float min_inclusive , float max_inclusive ) ;
extern double H3Sine ( double x ) ; 
extern double H3Cosine ( double x ) ; 
extern float H3Sqrt3 ( const float x ) ;  
extern float H3CarmSqrt ( float x ) ;

#define H3cosf(x) H3Cosine(x)
#define H3sinf(x) H3Sine(x)
#define H3ToDegree(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180
#define H3ToRadian(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define H3abs(x) (0 > x) ? -x : (x) 

#endif