
/*

	+	H3Matrix.h
	+	QDL

	+	hello3d
	+	realtime 3d engine
				

	+	(C)	Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3Matrix_H
#define H3Matrix_H

#include "H3Type.h"
#include "H3Memory.h"

/*
 brief :
*/
class H3Vector3f ;
class H3Vector4f ;

/*
 brief :
*/
class H3Matrix4x4f : virtual public H3Memory
{
public :
	/*
	 brief :
	*/
	H3Matrix4x4f() ;

	/*
	 brief :
	*/	
	~H3Matrix4x4f() ;

	/*
	 brief : identify
	*/	
	void identify() ;

	/*
	 brief : mulfity a scalar
	*/
	H3Matrix4x4f mul( iF32 scalar ) ;

	/*
	 brief : multify a matrix
	*/	
	H3Matrix4x4f mul( const H3Matrix4x4f a ) ;

	/*
	 brief : mulfity a verctor
	*/
	H3Vector3f mul( const H3Vector3f v ) ;

	/*
	 brief : mulfity a verctor
	*/
	H3Vector4f mul( const H3Vector4f v ) ;

	/*
	 brief : 
	*/
	void initScale ( const H3Vector3f v ) ;

	/*
	 brief :
	*/
	iF32 determinant () ;

	/*
	 brief : 
	*/
	void initTranslate ( const H3Vector3f v ) ;

	/*
	 brief :
	*/
	H3Matrix4x4f inverse () ;

	/*
	 brief : mulfity a verctor
	*/
	void transpose() ;

	/*
	 brief : set matrix value with specific value
	*/
	void set( int row , int col , iF32 value ) ;

	/*
	 brief : init with another matrix
	*/
	void set( iF32 matrix[4][4] ) ;

	/*
	 brief : output data on console
	*/
	void display () ;

	/*
	 brief :
	*/
	H3Matrix4x4f& operator * (iF32 scalar) ;

	/*
	 brief :
	*/
	H3Matrix4x4f& operator * (H3Matrix4x4f M) ;

	/*
	 brief :
	*/
	H3Matrix4x4f& operator = (H3Matrix4x4f M) ;

	iF32 m[4][4] ;

} ;

#endif