
/*

	+	H3Raster.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine

	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Raster.h"

/*
 brief : H3RasterPen
*/

/*
 brief : 
*/
H3RasterPen::H3RasterPen ( H3RasterColor color , iU32 width ) 
{
	this->color = color ;
	this->width = width ;
}

/*
 brief : 
*/
H3RasterPen::~H3RasterPen () {}

/*
 brief : 
*/
void H3RasterPen::setColor ( H3RasterColor color ) 
{
	this->color = color ;
}

/*
 brief : 
*/
H3RasterColor H3RasterPen::getColor () 
{
	return this->color ;
}



/*
 brief : H3RasterRender
*/
H3RasterRender:: H3RasterRender() {}

/*
 brief : 
*/
H3RasterRender::~H3RasterRender() {}

/*
 brief : 
*/
bool H3RasterRender::isRangeValid ( H3Vector2i point ) 
{
	if ( point.x < 0 ) 
		return false ;

	if ( point.y < 0 ) 
		return false ;

	return true ;
}

/*
 brief : 
*/
void H3RasterRender::drawPoint ( H3Image* image , H3RasterPen* pen ,  iS32 x , iS32 y )
{
	H3ASSERT(image && pen) ;

	if ( x<0 || y<0 ) 
		return ;

	image->setData ( pen->getColor().r , pen->getColor().g , pen->getColor().b , pen->getColor().a , x , y ) ;	
}

/*
 brief : 
*/
void H3RasterRender::drawImage ( H3Image* dest , H3Image* src , H3Vector2i pos , int start_x , int start_y , int end_x , int end_y , H3RasterPen* pen )
{
	assert( pos.x >= 0 && pos.y >= 0 ) ;
	assert( pos.x<dest->getWidth() && pos.y<dest->getHeight() ) ;
	assert( start_x>=0 && start_y>=0 && end_x<src->getWidth() && end_y<src->getHeight() ) ;

	H3RasterPen pencilOrignal(H3RasterColor(255,255,255,255),1) ;

	int stride = 0;
	int pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGBA32 == src->getType () ) 
		pixelBytes = 4 ;
	else
		pixelBytes = 3 ;
	stride = src->getWidth()*pixelBytes ;

	iU8* rgbsrc = src->getData () ;
	iF32 colorRatio = 1.0f / 255.0f ;
	for( iS32 y = start_y ; y <= end_y ; y ++ )
	{
		for( iS32 x = start_x*pixelBytes , deltaX =0 ; x <= (end_x*pixelBytes) ; x+=pixelBytes ,deltaX ++)
		{
			unsigned char r = 0 ;
			unsigned char g = 0 ; 
			unsigned char b = 0 ; 
			unsigned char a = 0 ; 

			r = rgbsrc[y*stride+x] ;
			g = rgbsrc[y*stride+x+1] ;
			b = rgbsrc[y*stride+x+2] ;
			if ( H3_RGB_FORMAT_RGBA32 == src->getType () )
				a = rgbsrc[y*stride+x+3] ;
			
			if ( 0 == pen ) {
				pencilOrignal.setColor( H3RasterColor( r,g,b,a )) ;
				H3RasterRender::drawPoint ( dest , &pencilOrignal , pos.x + deltaX , pos.y + y - start_y ) ;
			} else {
				unsigned char r = pen->getColor().r ;
				unsigned char g = pen->getColor().g ; 
				unsigned char b = pen->getColor().b ; 
				pen->setColor( H3RasterColor( r,g,b,a )) ;
				H3RasterRender::drawPoint ( dest , pen , pos.x + deltaX , pos.y + y - start_y ) ;
			}

			
		}
	}
}

/*
 brief : 
*/
void H3RasterRender::drawLine ( H3Image* image , H3RasterPen* pen , H3Vector2i point1 , H3Vector2i point2 ) {}

/*
 brief : 
*/
void H3RasterRender::drawCircle ( H3Image* image , H3RasterPen* pen , H3Vector2i centor , iF32 radius ) {}

/*
 brief : 
*/
void H3RasterRender::drawPolygon ( H3Image* image , H3RasterPen* pen , H3Vector2i points[] , iU32 totallPoints ) {}

/*
 brief : 
*/
void H3RasterRender::drawRect ( H3Image* image , H3RasterPen* pen , H3Vector2i leftUpPoint , H3Vector2i rightBottomPoint ) {}

/*
 brief : 
*/
void H3RasterRender::fillCircle ( H3Image* image , H3RasterPen* pen , H3Vector2i centor , iF32 radius ) {}

/*
 brief : 
*/
void H3RasterRender::fillPolygon ( H3Image* image , H3RasterPen* pen , H3Vector2i points[] , iU32 totallPoints ) {}

/*
 brief : 
*/
void H3RasterRender::fillRect ( H3Image* image , H3RasterPen* pen , H3Vector2i leftUpPoint , H3Vector2i range ) 
{
	H3ASSERT(image && pen) ;

	if ( false == isRangeValid ( leftUpPoint ) || false == isRangeValid (range) ) 
		return ;

	iS32 startY = leftUpPoint.y ;
	iS32 endX = leftUpPoint.x + range.x ;
	iS32 endY = leftUpPoint.y + range.y ;

	for ( ; startY < endY && startY < (iS32 )image->getHeight() ; startY++ )
	{
		for ( iS32 startX = leftUpPoint.x ; startX < endX && startX < (iS32 )image->getWidth() ; startX++ )
		{

			iF32 r = pen->getColor().r ;
			iF32 g = pen->getColor().g ;
			iF32 b = pen->getColor().b ;
			iF32 a = pen->getColor().a ;

			image->setData ( r , g , b , a , startX , startY ) ;
		}
	}

}
