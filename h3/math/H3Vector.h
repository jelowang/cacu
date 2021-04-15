
/*

	+	H3Vector.h
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

#ifndef H3Vector_H
#define H3Vector_H

#include "H3Type.h"
#include "H3Memory.h"

#define H3Vector2fMake(x,y,z) H3Vector2f((iF32 )x,(iF32 )y)
#define H3Vector3fMake(x,y,z) H3Vector3f((iF32 )x,(iF32 )y,(iF32)z)

/*
 brief : 2d
*/
class H3Vector2f : virtual public H3Memory
{
public :

	/*
	 brief :
	*/
	H3Vector2f() ;

	/*
	 brief :
	*/
	H3Vector2f( iF32 x , iF32 y ) ;

	/*
	 brief :
	*/
	~H3Vector2f() ;

	/*
	 brief :
	*/
	void identify() ;

	/*
	 brief :
	*/
	void init( iF32 x , iF32 y ) ;

	/*
	 brief :
	*/
	H3Vector2f add( H3Vector2f v ) ;

	/*
	 brief :
	*/
	H3Vector2f sub( H3Vector2f v ) ;

	/*
	 brief :
	*/
	H3Vector2f mul( iF32 v ) ;

	/*
	 brief :
	*/
	H3Vector2f div( iF32 v ) ;

	/*
	 brief :
	*/
	iF32 dot( H3Vector2f v ) ;

	/*
	 brief :
	*/
	bool equal ( H3Vector2f v ) ;

	/*
	 brief : magnitude of vector
	*/
	iF32 mag () ;

	/*
	 brief :
	*/
	void normalize () ;

	/*
	 brief : distance bettween two vector
	*/
	iF32 distance ( H3Vector2f v , H3Vector2f v2 ) ;

	/*
	 brief : output data 
	*/
	void display () ;

	/*
	 brief :
	*/
	H3Vector2f operator + (H3Vector2f v) ;

	/*
	 brief :
	*/
	H3Vector2f operator - (H3Vector2f v) ;

	/*
	 brief :
	*/
	H3Vector2f operator * (iF32 v) ;

	/*
	 brief :
	*/
	H3Vector2f operator / (iF32 v) ;

	/*
	 brief :
	*/
	H3Vector2f operator = (iF32 value) ;

	/*
	 brief :
	*/
	H3Vector2f operator = (H3Vector2f v) ;

	iF32 x ;
	iF32 y ;

} ;

/*
 brief : 2d
*/
class H3Vector2i : virtual public H3Memory
{
public :

	/*
	 brief :
	*/
	H3Vector2i() ;

	/*
	 brief :
	*/
	H3Vector2i( iS32 x , iS32 y ) ;

	/*
	 brief :
	*/
	~H3Vector2i() ;

	/*
	 brief :
	*/
	void identify() ;

	/*
	 brief :
	*/
	void init( iS32 x , iS32 y ) ;

	/*
	 brief :
	*/
	H3Vector2i add( H3Vector2i v ) ;

	/*
	 brief :
	*/
	H3Vector2i sub( H3Vector2i v ) ;

	/*
	 brief :
	*/
	H3Vector2i mul( iS32 v ) ;

	/*
	 brief :
	*/
	H3Vector2i div( iS32 v ) ;

	/*
	 brief :
	*/
	iS32 dot( H3Vector2i v ) ;

	/*
	 brief :
	*/
	bool equal ( H3Vector2i v ) ;

	/*
	 brief : magnitude of vector
	*/
	iS32 mag () ;

	/*
	 brief :
	*/
	void normalize () ;

	/*
	 brief : distance bettween two vector
	*/
	iS32 distance ( H3Vector2i v , H3Vector2i v2 ) ;

	/*
	 brief : output data 
	*/
	void display () ;

	/*
	 brief :
	*/
	H3Vector2i operator + (H3Vector2i v) ;

	/*
	 brief :
	*/
	H3Vector2i operator - (H3Vector2i v) ;

	/*
	 brief :
	*/
	H3Vector2i operator * (iS32 v) ;

	/*
	 brief :
	*/
	H3Vector2i operator / (iS32 v) ;

	/*
	 brief :
	*/
	H3Vector2i operator = (iS32 value) ;

	/*
	 brief :
	*/
	H3Vector2i operator = (H3Vector2i v) ;

	iS32 x ;
	iS32 y ;

} ;

/*
 brief : 3d
*/
class H3Vector3f : virtual public H3Memory
{
public :

	/*
	 brief :
	*/
	H3Vector3f() ;

	/*
	 brief :
	*/
	H3Vector3f( iF32 x , iF32 y , iF32 z ) ;

	/*
	 brief :
	*/
	~H3Vector3f() ;

	/*
	 brief :
	*/
	void identify() ;

	/*
	 brief :
	*/
	void init( iF32 x , iF32 y , iF32 z ) ;

	/*
	 brief :
	*/
	H3Vector3f add( H3Vector3f v ) ;

	/*
	 brief :
	*/
	H3Vector3f sub( H3Vector3f v ) ;

	/*
	 brief :
	*/
	H3Vector3f mul( iF32 v ) ;

	/*
	 brief :
	*/
	H3Vector3f div( iF32 v ) ;

	/*
	 brief :
	*/
	iF32 dot( H3Vector3f v ) ;

	/*
	 brief :
	*/
	H3Vector3f cross ( H3Vector3f v ) ;

	/*
	 brief :
	*/
	bool equal ( H3Vector3f v ) ;

	/*
	 brief : magnitude of vector
	*/
	iF32 mag () ;

	/*
	 brief :
	*/
	H3Vector3f normalize () ;

	/*
	 brief : distance bettween two vector
	*/
	iF32 distance ( H3Vector3f v , H3Vector3f v2 ) ;
	
	/*
	 brief : output data 
	*/
	void display () ;

	/*
	 brief :
	*/
	H3Vector3f operator + (H3Vector3f v) ;

	/*
	 brief :
	*/
	H3Vector3f operator - (H3Vector3f v) ;

	/*
	 brief :
	*/
	H3Vector3f operator * (iF32 v) ;

	/*
	 brief :
	*/
	H3Vector3f operator / (iF32 v) ;

	/*
	 brief :
	*/
	H3Vector3f operator = (iF32 value) ;

	/*
	 brief :
	*/
	H3Vector3f operator = (H3Vector3f v) ;

	iF32 x ;
	iF32 y ;
	iF32 z ;

} ;


/*
 brief : 4d
*/
class H3Vector4f : virtual public H3Memory
{
public :

	/*
	 brief :
	*/
	H3Vector4f() ;

	/*
	 brief :
	*/
	H3Vector4f( iF32 x , iF32 y , iF32 z , iF32 w ) ;

	/*
	 brief :
	*/
	~H3Vector4f() ;

	/*
	 brief :
	*/
	void identify() ;

	/*
	 brief :
	*/
	void init( iF32 x , iF32 y , iF32 z , iF32 w ) ;

	/*
	 brief :
	*/
	H3Vector4f add( H3Vector4f v ) ;

	/*
	 brief :
	*/
	H3Vector4f sub( H3Vector4f v ) ;

	/*
	 brief :
	*/
	H3Vector4f mul( iF32 v ) ;

	/*
	 brief :
	*/
	H3Vector4f div( iF32 v ) ;

	/*
	 brief :
	*/
	iF32 dot( H3Vector4f v ) ;

	/*
	 brief :
	*/
	H3Vector4f cross ( H3Vector4f v ) ;

	/*
	 brief :
	*/
	bool equal ( H3Vector4f v ) ;

	/*
	 brief : magnitude of vector
	*/
	iF32 mag () ;

	/*
	 brief :
	*/
	H3Vector4f normalize () ;

	/*
	 brief : distance bettween two vector
	*/
	iF32 distance ( H3Vector4f v , H3Vector4f v2 ) ;
	
	/*
	 brief : output data 
	*/
	void display () ;

	/*
	 brief :
	*/
	H3Vector4f operator + (H3Vector4f v) ;

	/*
	 brief :
	*/
	H3Vector4f operator - (H3Vector4f v) ;

	/*
	 brief :
	*/
	H3Vector4f operator * (iF32 v) ;

	/*
	 brief :
	*/
	H3Vector4f operator / (iF32 v) ;

	/*
	 brief :
	*/
	H3Vector4f operator = (iF32 value) ;

	/*
	 brief :
	*/
	H3Vector4f operator = (H3Vector4f v) ;

	iF32 x ;
	iF32 y ;
	iF32 z ;
	iF32 w ;

} ;

#endif