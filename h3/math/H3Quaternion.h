
/*

	+	H3Quaternion.h
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

#ifndef H3Quaternion_H
#define H3Quaternion_H

#include "H3Type.h"
#include "H3Memory.h"

/*
 brief :
*/
class H3Vector3f ;
/*
 brief :
*/
class H3Matrix4x4f ;

class H3Quaternion : virtual public H3Memory
{
public :
    
	/*
	 brief :
	*/
	H3Quaternion() ;

	/*
	 brief :
	*/
	H3Quaternion( iF32 w , iF32 x , iF32 y , iF32 z ) ;

   /*
     brief : rotate about ambitary axis
    */
    H3Quaternion( const H3Vector3f aixs , iF32 theta ) ;

	/*
	 brief :
	*/
	~H3Quaternion() ;

	/*
	 brief :
	*/
	void identify () ;

	/*
	 brief :
	*/
	H3Quaternion mul ( H3Quaternion Q ) ;

	/*
	 brief :
	*/
	H3Quaternion mul ( H3Vector3f v ) ;

    /*
     brief :
    */
	H3Quaternion conjugate() ;

	/*
	 brief : magnitude 
	*/
	iF32 mag () ;

	/*
	 brief :
	*/
	void normalize () ;

    /*
     brief : 
    */
    H3Vector3f getRotateAxis() ;

    /*
     brief : get matrix forms
    */
	H3Matrix4x4f getMatrixForm() ;

	/*
	 brief : output data on console
	*/
	void display () ;

	/*
	 brief :
	*/
	H3Quaternion operator * (H3Quaternion Q) ;

	/*
	 brief :
	*/
	H3Quaternion operator = (H3Quaternion Q) ;
 
    iF32 w ;
    iF32 x ;
	iF32 y ;
	iF32 z ;


} ;

#endif