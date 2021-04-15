
/*

	+	H3Light.h
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

#ifndef H3Light_H
#define H3Light_H

#include "H3Std.h"
#include "H3Component.h"
#include "H3MathUtil.h"

class H3Light : public H3Component , virtual public H3Memory
{
public :
	/*
	 brief :
	*/
	H3Light () ;

	/*
	 brief :
	*/
	~H3Light () ;

	/*
	 brief :
	*/
	virtual bool init () { return true ; }

	/*
	 brief :
	*/
	virtual void setColor ( H3Vector3f color ) ;

public:
	void* shader ;
	H3Vector3f direction ;
	H3Vector3f color ;
	iF32 intensity ;

} ;

/*
 brief :
*/
class H3AmbientLight : public H3Light
{
public :
	H3AmbientLight( H3Vector3f colorIntensity ) ;
	~H3AmbientLight() ;
} ;

/*
 brief :
*/
class H3PointLight : public H3Light
{
public :
	H3PointLight( H3Vector3f position , H3Vector3f color , iF32 intensity , iF32 constant , iF32 linear , iF32 quadratic ) ;
	~H3PointLight() ;
	iF32 constant ;
	iF32 linear ;
	iF32 quadratic ;
	iF32 range ;
} ;

/*
 brief :
*/
class H3DirectionalLight : public H3Light
{
public :
	H3DirectionalLight( H3Vector3f color , H3Vector3f direction , iF32 intensity ) ;
	~H3DirectionalLight() ;
public :
	H3Vector3f direction ;
} ;

/*
 brief :
*/
class H3SpotLight : public H3Light
{
public :
	H3SpotLight () ;
	~H3SpotLight() ;
	H3SpotLight( H3Vector3f position , H3Vector3f direction , H3Vector3f color , iF32 intensity , iF32 cutOff ) ;
	iF32 cutOff ;
} ;

#endif
