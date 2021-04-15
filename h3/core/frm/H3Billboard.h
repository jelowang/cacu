
/*

	+	H3Billboard.h
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

#ifndef H3Billboard_H
#define H3Billboard_H

#include "H3Std.h"
#include "H3MathUtil.h"
#include "H3Material.h"
#include "H3Object.h"
#include "H3PlaneRender.h"

class H3Billboard : public H3Object , virtual public H3Memory
{
public :
	/*
	 brief :
	*/	
	H3Billboard ( iF32 radius ) ;
	
	/*
	 brief : 
	*/
	virtual void drawAABB () ;

	/*
	 brief : Tests whether this bounding box intersects the specified ray.
			return mesh group id if intersected otherwise return -1
			return intersect point
	*/
	virtual iS32 intersect ( H3Ray ray , H3Vector3f* color , H3Vector3f& intersectPoint ) ;

	/*
	 breif : get mesh position witch MeshGroup Id
	 Param : MeshGroup Id
	*/
	virtual H3Vector3f getMeshPosition ( iF32 id ) ;

	/*
	 brief :
	*/
	H3Material* getMaterial () ;

	/*
	 brief :
	*/
	void setRadius ( iF32 radius ) ;

	/*
	 brief :
	*/
	void setScale ( H3Vector3f scale ) ;

	/*
	 brief :
	*/	
	virtual void update ( iF32 delta ) ;

	/*
	 brief :
	*/	
	~H3Billboard () ;
	
private :

	iF32 radius ;
	H3Vector3f scale ;
	H3Material* material ;
	H3AABB aabb ;
	H3PlaneRender* planeRenderRef ;

} ;

#endif