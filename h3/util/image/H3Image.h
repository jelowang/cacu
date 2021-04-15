
/*

	+	H3Image.h
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

#ifndef H3Image_H
#define H3Image_H

#include "H3Std.h"
#include "H3Vector.h"

class H3Image : virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3Image() ;

    /*
     brief :
    */
	~H3Image() ;

    /*
     brief :
    */
	H3Image ( iS8* path ) ;

    /*
     brief :
    */
	bool init ( iS8* path ) ;

    /*
     brief :
    */
	bool init ( iU32 width , iU32 height , iU32 type = H3_RGB_FORMAT_RGBA32 ) ;

    /*
     brief : rgb data , width , height , rgb type
    */
	bool init ( iU8* data , iU32 width , iU32 height , iU32 type ) ;

    /*
     brief :
    */
	iU32 getWidth () ;

    /*
     brief :
    */
	iU32 getHeight () ;

    /*
     brief : return pixels data with format rbga8888
    */
	iU8* getData () ;

	/*
	 brief : 
	*/
	void setData ( iU8 r , iU8 g , iU8 b , iU8 a , iS32 x , iS32 y ) ;

	/*
	 brief : 
	*/
	bool getAlpha () ;

	/*
	 brief : 
	*/
	iU32 getType () ;

	/*
	 brief : convert to rgba8888 32 bit
	*/
	void convsert2rgba8888 () ;

	/*
	 brief : scale the image 
	*/
	void setScale ( iF32 scale ) ;

	/*
	 brief : scale the image 
	*/
	void writeFile ( const iS8* path ) ;

private:
	iU32 type ;
	iU32 width ;
	iU32 height ;
	iU8* data ;	
	bool hasAlpha ;  

} ;

#endif