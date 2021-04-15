
/*

	+	H3Image.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			Leyoo Co.,Ltd.
	+
 
*/

#include "i51PngDecoder.h"
#include "H3Image.h"
#ifdef H3_QT_5_4_2
#include "QImage.h"
#endif

/*
 brief :
*/
H3Image::H3Image() 
{
	this->width = 0 ;
	this->height = 0 ;
	this->data = 0 ;
}

/*
 brief :
*/
H3Image::~H3Image() 
{
	if ( this->data )
	{
		MMC_FREE ( this->data ) ;
	}
}

/*
 brief :
*/
H3Image::H3Image ( iS8* path ) 
{
	init ( path ) ;
}

/*
 brief :
*/
bool H3Image::init ( iS8* path ) 
{
#ifdef H3_QT_5_4_2
	QImage im ;
	iS32 length = 0 ;
	void* file = SCHalFileOpen ( path , "rb" ) ;
	
	if ( 0 == file )
	{
		iLog ( "image file '%s' not exists" , path ) ;
		return false ;
	}

	length = SCHalFileLength ( file ) ;
	void* buffer = (void* ) MMC_MALLOC ( length ) ;

	if ( 0 == buffer )
	{
		iLog ( "not enough memory for image decode 1" ) ;
		SCHalFileClose ( file ) ;
		return false ;
	}

	SCHalFileRead ( file , buffer , length ) ;
	SCHalFileClose ( file ) ;

	im.loadFromData( (const uchar* )buffer , length ) ;
	MMC_FREE ( buffer ) ;

	im = im.convertToFormat((QImage::Format)QImage::Format_RGBA8888);
	
	this->width = im.width () ;
	this->height = im.height () ;
	this->data = (iS8* )im.bits () ;

	this->data = (iS8* ) MMC_MALLOC ( im.byteCount () ) ;

	if ( 0 == this->data )
	{
		iLog ( "not enough memory for image decode 2" ) ;
		MMC_FREE ( buffer ) ;
		return false ;
	}

	memcpy ( this->data , im.bits () , im.byteCount () ) ;
#endif

#ifdef H3_IOS
	std::string getFullPathForDirectoryAndFilename( const char* path ) ;
	
	std::string filepath = getFullPathForDirectoryAndFilename ( path ) ;
    
    //  open file
    NSString* nsfilename = [NSString stringWithUTF8String:filepath.data()] ;
    CGImageRef sprintImage = [UIImage imageNamed:nsfilename].CGImage ;
    
    if ( 0 == sprintImage ) {
        iLog("Failed to load Image %s",path) ;
        return false ;
    }
    this->width = CGImageGetWidth ( sprintImage ) ;
    this->height = CGImageGetHeight ( sprintImage ) ;
    this->data = (iS8* )MMC_MALLOC( this->width*this->height*4 ) ;
    CGContextRef sprintContext = CGBitmapContextCreate(this->data,this->width, this->height, 8 , this->width*4 , CGImageGetColorSpace(sprintImage) , kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(sprintContext, CGRectMake(0,0,this->width,this->height), sprintImage ) ;
    CGContextRelease(sprintContext) ;
#endif
	
	return true ;

#if 0
	if ( true == i51PngDecoderDecoderIsPng( file , 0 ) )
	{
		i51PngDecoderGetAttribute ( file , 0 , &this->width , &this->height ) ;
		if ( 0 == this->width || 0 == this->height ) 
		{
			iLog ( "image file resolution is error") ;
			SCHalFileClose ( file ) ;
			return false ;
		}
		//	rgb8888
		this->data = (iS8* ) MMC_MALLOC ( this->width * this->height * 4 ) ;
		if ( 0 == this->data )
		{
			iLog ( "not enough memory for image decoding") ;
			SCHalFileClose ( file ) ;
			return false ;
		}
		memset ( this->data , 0 , this->width * this->height * 4 ) ;

		bool ret = i51PngDecoderDecodeImage ( file , 0 , 0 , this->data , 1 ) ;

		if ( false == ret )
		{
			iLog ( "image decode failed") ;
			MMC_FREE_EX ( (void** )&this->data ) ;
			SCHalFileClose ( file ) ;
			return false ;
		}

	}

	SCHalFileClose ( file ) ;

	return true ;

#endif

}

/*
 brief :
*/
bool H3Image::init ( iU32 width , iU32 height ) 
{

	this->width = width ;
	this->height = height ;

	//	rgb8888
	this->data = (iS8* ) MMC_MALLOC ( this->width * this->height * 4 ) ;
	if ( 0 == this->data )
	{
		iLog ( "not enough memory for image") ;
		return false ;
	}
	memset ( this->data , 0 , this->width * this->height * 4 ) ;

	return true ;

}

/*
 brief :
*/
iU32 H3Image::getWidth () 
{
	return this->width ;
}

/*
 brief :
*/
iU32 H3Image::getHeight () 
{
	return this->height ;
}

/*
 brief : return pixels data with format rbga8888
*/
iS8* H3Image::getData () 
{
	return this->data ;
}

/*
 brief : assume the xy range is valid
*/
void H3Image::setData ( iF32 r , iF32 g , iF32 b , iF32 a , iS32 x , iS32 y ) 
{

	iU8 red = (iU8)(r * 255) ;
	iU8 green = (iU8)(g * 255)  ;
	iU8 blue = (iU8)(b * 255)  ;
	iU8 alpha = (iU8)(a * 255)  ;

	iS32 row = y * this->width * sizeof(iU32) ;
	iS32 col = x * sizeof(iU32) ;

	this->data [ row + col ] = red ;
	this->data [ row + col+1 ] = green ;
	this->data [ row + col+2 ] = blue ;
	this->data [ row + col+3 ] = alpha ;

}