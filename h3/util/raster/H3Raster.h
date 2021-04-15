
/*

	+	H3Raster.h
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

#ifndef H3Raster_H
#define H3Raster_H

#include "H3Std.h"
#include "H3MathUtil.h"
#include "H3Image.h"

/*
 brief : color of range [0,255]
*/
class H3RasterColor
{
public:
	H3RasterColor (){}
	~H3RasterColor () {}
	H3RasterColor ( iU8 r , iU8 g , iU8 b , iU8 a ) 
	{
		this->r = r ;
		this->g = g ;
		this->b = b ;
		this->a = a ;
	}
	H3RasterColor operator = (H3RasterColor v) 
	{
		this->r = v.r ;
		this->g = v.g ;
		this->b = v.b ;
		this->a = v.a ;
		return *this ;
	}
	iU8 r ;
	iU8 g ;
	iU8 b ;
	iU8 a ;
}  ;

/*
 brief : 
*/
class H3RasterPen
{
public :
	/*
	 brief : 
	*/
	H3RasterPen ( H3RasterColor color , iU32 width ) ;

	/*
	 brief : 
	*/
	~H3RasterPen () ;

	/*
	 brief : 
	*/
	void setColor ( H3RasterColor color ) ;

	/*
	 brief : 
	*/
	H3RasterColor getColor () ;

private :
	H3RasterColor color ;
	iU32 width ;

} ;

/*
 brief : 
*/
class H3RasterRender
{
public :
	/*
	 brief : 
	*/
	H3RasterRender() ;

	/*
	 brief : 
	*/
	~H3RasterRender() ;

	/*
	 brief : 
	*/
	static bool isRangeValid ( H3Vector2i point ) ;

	/*
	 brief : 
	*/
	static void drawPoint ( H3Image* image , H3RasterPen* pen , iS32 x , iS32 y ) ;

	/*
	 brief : 
	*/
	static void drawImage ( H3Image* dest , H3Image* src , H3Vector2i pos , int start_x , int start_y , int end_x , int end_y , H3RasterPen* pen = 0  ) ;

	/*
	 brief : 
	*/
	static void drawLine ( H3Image* image , H3RasterPen* pen , H3Vector2i point1 , H3Vector2i point2 ) ;

	/*
	 brief : 
	*/
	static void drawCircle ( H3Image* image , H3RasterPen* pen , H3Vector2i centor , iF32 radius ) ;

	/*
	 brief : 
	*/
	static void drawPolygon ( H3Image* image , H3RasterPen* pen , H3Vector2i points[] , iU32 totallPoints ) ;

	/*
	 brief : 
	*/
	static void drawRect ( H3Image* image , H3RasterPen* pen , H3Vector2i leftUpPoint , H3Vector2i rightBottomPoint ) ;	

	/*
	 brief : 
	*/
	static void fillCircle ( H3Image* image , H3RasterPen* pen , H3Vector2i centor , iF32 radius ) ;

	/*
	 brief : 
	*/
	static void fillPolygon ( H3Image* image , H3RasterPen* pen , H3Vector2i points[] , iU32 totallPoints ) ;

	/*
	 brief : 
	*/
	static void fillRect ( H3Image* image , H3RasterPen* pen , H3Vector2i leftUpPoint , H3Vector2i range ) ;

} ;

#endif