
/*

	+	H3Transform.h
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

#ifndef H3Transform_H
#define H3Transform_H

#include "H3Type.h"
#include "H3Memory.h"
#include "H3Vector.h"
#include "H3Matrix.h"
#include "H3Quaternion.h"

class H3Transform : virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3Transform() ;

    /*
     brief :
    */
	~H3Transform() ; 

	/*
	 brief :
	*/
	void reset () ;

    /*
     brief :
    */
	H3Vector3f& getPosition () ;

	/*
	 brief : set basics
	*/
	void setBasics ( H3Vector3f basics ) ;

	/*
	 brief : get basics
	*/
	H3Vector3f& getBasics () ;

    /*
     brief :
    */
	void setPosition ( H3Vector3f position ) ;

    /*
     brief :
    */
	void setScale ( H3Vector3f scale ) ;

    /*
     brief :
    */
	void rotate ( H3Quaternion Q ) ;

	/*
	 brief :
	*/
	H3Quaternion getRotate () ;

    /*
     brief : rotate , scale , transform
    */
	H3Matrix4x4f finishTransformation ( bool skipRotate = false ) ;

private :
	H3Vector3f basics ;
	H3Vector3f scale ;
	H3Vector3f position ;
	H3Quaternion quaternion ;


} ;

#endif