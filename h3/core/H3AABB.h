
/*

	+	H3AABB.h
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

#ifndef H3AABB_H 
#define H3AABB_H 

#include "H3Std.h"
#include "H3MathUtil.h"

class H3AABB : virtual public H3Memory
{

public :

	/*
	 brief :
	*/
	H3AABB() ;

	/*
	 brief :
	*/
	~H3AABB() ;

	/*
	 brief :
	*/
	void setEmpty () ;

	/*
	 brief :
	*/
	void add ( const H3Vector3f& p ) ;

	/*
	 brief :
	*/
	void set ( const H3AABB* aabb ) ;

    /*
     brief : return 8 corners of aabb with left-hands orders of clockwise
    */
	H3Vector3f* getCorners () ;

    /*
     brief : Tests whether this bounding box intersects the specified bounding object.
    */
    bool intersect ( const H3AABB& aabb ) ;

    /*
     brief : check whether the point is in.
    */
    bool containPoint(const H3Vector3f& point ) ;

    /*
     brief : Gets the center point of the bounding box.
    */
    H3Vector3f getCenter();

    /*
     brief : Transforms the bounding box by the given transformation matrix.
    */
	H3AABB transform ( H3Matrix4x4f mat ) ;

	/*
	 brief :
	*/
	H3AABB operator = (H3AABB aabb) ;

	H3Vector3f min ;
	H3Vector3f max ;

} ;

#endif