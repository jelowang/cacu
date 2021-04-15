
/*

	+	H3Components.h
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

#ifndef H3Components_H
#define H3Components_H

#include "H3Std.h"
#include "H3Vector.h"
#include "H3Matrix.h"
#include "H3Transform.h"
#include "H3Material.h"

/*
 brief :
*/
enum 
{
	H3COMPONENT_TYPE_COMPONENT = 0 ,
	H3COMPONENT_TYPE_LIGHT ,
	H3COMPONENT_TYPE_AMBIENT_LIGHT ,
	H3COMPONENT_TYPE_POINT_LIGHT ,
	H3COMPONENT_TYPE_SPOT_LIGHT ,
	H3COMPONENT_TYPE_DIRECTIONAL_LIGHT ,
	H3COMPONENT_TYPE_CAMERA_PERSPECTIVE ,
	H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC,
	H3COMPONENT_TYPE_MESHRENDER ,
	H3COMPONENT_TYPE_PLANERENDER ,
	H3COMPONENT_TYPE_GEOMETRY_LINE ,
} ;

class H3Component : virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3Component () ;

    /*
     brief :
    */
	virtual ~H3Component () ;

    /*
     brief :
    */
	virtual void update ( iF32 deltaT ) ;

    /*
     brief :
    */
	virtual void render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;

    /*
     brief :
    */
	virtual H3Vector3f& getPosition () ;

    /*
     brief :
    */
	virtual void setPosition ( H3Vector3f position ) ;

    /*
     brief :
    */
	virtual void rotate ( H3Vector3f axis , iF32 amout ) ;

	/*
	 brief :
	*/
	virtual void rotateAround ( H3Vector3f axis , H3Vector3f point , iF32 amout ) ;

    /*
     brief :
    */
	virtual H3Transform& getTransform () ;

    /*
     brief :
    */
	iS32 getType () ;

	/* 
	 brief : call use inside engine 
	*/
	void addRef () ;

	/*
	 brief : engine will call this method to release material memorys
	*/
	void release () ;

public :
	iS32 type ;
	iS32 refCount ;
	H3Transform h3Transform ;

} ;

#endif