
/*

	+	H3Ray.h
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

#ifndef H3Ray_H
#define H3Ray_H

#include "H3Std.h"
#include "H3MathUtil.h"
#include "H3Camera.h"

class H3Ray : virtual public H3Memory 
{

public :

	H3Ray () ;
	H3Ray ( H3Vector3f o , H3Vector3f d ) ;
	~H3Ray () ;
	void rotate ( H3Vector3f axis , iF32 amout ) ;
	inline void setO ( H3Vector3f o ) { this->o = o ; }
	inline void setD ( H3Vector3f d ) { this->d = d ; }
	inline H3Vector3f getO () { return this->o ; }
	inline H3Vector3f getD () { return this->d ; }
	static H3Vector3f castRay ( H3Camera* camera , H3Vector2f point , H3Vector2f viewPort ) ;

private :
	H3Vector3f o ;
	H3Vector3f d ;
	H3Quaternion rot ;

} ;

#endif