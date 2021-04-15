
/*

	+	H3Object.h
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

#ifndef H3Object_H
#define H3Object_H

#include "H3Std.h"
#include "H3Matrix.h"
#include "H3Quaternion.h"
#include "H3Transform.h"
#include "H3Component.h"
#include "H3Material.h"
#include "H3Shader.h"
#include "H3Camera.h"
#include "H3AABB.h"
#include "H3Ray.h"
#include "H3Action.h"
#include "H3Release.h"

class H3Mesh ;

class H3Object : public H3Release , virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3Object () ;

    /*
     brief :
    */
	virtual ~H3Object () ;

    /*
     brief :
    */
	virtual void update ( iF32 deltaT ) ;

    /*
     brief :
    */
	virtual void render ( void* render , H3Light* light , void* shader , H3Component* camera , H3Matrix4x4f* lightProjMatrix ) ;

    /*
     brief :
    */
	virtual void addChild ( H3Object* object ) ;

    /*
     brief :
    */
	virtual void addComponent ( H3Component* component ) ;

    /*
     brief :
    */
	virtual H3Vector3f getPosition () ;

    /*
     brief :
    */
	virtual void setPosition ( H3Vector3f position ) ;

    /*
     brief :
    */
	virtual void setScale ( H3Vector3f scale ) ;

    /*
     brief :
    */
	virtual void setParent ( H3Object* parent ) ;

    /*
     brief :
    */
	virtual void rotate ( H3Vector3f axis , iF32 amout ) ;

    /*
     brief : rotate around a portion point in local space
	 param : portion value must in closed interval [0...1]
    */
	virtual void rotateAroundLocal ( H3Vector3f axis , H3Vector3f portion , iF32 amout ) ;

    /*
     brief : rotate around a position in global 
    */
	virtual void rotateAroundGlobal ( H3Vector3f axis , H3Vector3f position , iF32 amout ) ;

    /*
     brief :
    */
	virtual H3Transform& getTransform () ;

    /*
     brief :
    */
	virtual bool release () ;

    /*
     brief :
    */
	virtual bool removeAllAndReleseSlef () ;

    /*
     brief :
    */
	virtual bool removeAll () ;

	/*
	 brief : rotate , scale , transform
	*/
	virtual H3Matrix4x4f finishTransformation ( bool skipRotate = false ) ;

	/*
	 brief : remove childs and release its resources
	*/
	virtual void removeChild ( H3Object* object ) ;

	/*
	 brief : remove from parent and release its resources
	*/
	virtual void removeFromParent () ;

	/*
	 brief : set visible on screen
	*/
	virtual void setVisible ( bool visible )  ;

	/*
	 brief : 
	*/
	virtual bool getVisible () ;

	/*
	 brief : 
	*/
	virtual void drawAABB () ;

    /*
     brief : Tests whether this bounding box intersects the specified bounding object.
    */
	virtual  bool intersect ( const H3AABB& aabb ) ;

	/*
	 brief : Tests whether this bounding box intersects the specified ray.
			return mesh group id if intersected otherwise return -1
			return intersect point
	*/
	virtual iS32 intersect ( H3Ray ray , H3Vector3f* color , H3Vector3f& intersectPoint ) ;

    /*
     brief : check whether the point is in.
    */
    virtual bool containPoint(const H3Vector3f& point ) ;

	/*
     brief : get object H3Mesh* handle
    */
	virtual H3Mesh* getMesh () ;

	/*
	 breif : get mesh position witch MeshGroup Id
	 Param : MeshGroup Id
	*/
	virtual H3Vector3f getMeshPosition ( iF32 id ) ;

	/*
	 brief : skip light shading
	*/
	virtual void skipLightShader () ;

	/*
	 brief : run an action
	*/
	virtual void startAction ( H3Action* action ) ;

	/*
	 breif : remove all childs and components
	*/
	virtual void removeAllChildAndComponent ( );

	/*
	 brief : for align origin (0,0,0) to center , enable = true as default
	*/
	virtual void setBasicsAlignEnable ( bool enable ) ;

public :

	bool visible ;
	bool isBillboard ;
	bool isSkipLightShader ;

	SCClList childs ;
	SCClList components ;
	SCClList lightComponents ;	
	H3Object* parent ;
	SCClList orignalAABBRefList ;
	
protected :	
	
	bool basicsAlignEnable ;
	H3Transform h3Transform ;
	H3Mesh* meshRef ;

} ;

#endif