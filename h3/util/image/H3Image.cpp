
/*

	+	H3Image.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/


#include "H3Image.h"
#include "H3Raster.h"
#ifdef WIN32
#include "stb_image.h"
#include "stb_image_write.h"
#endif

#ifdef H3_QT_5_4_2
#include "QImage.h"
#endif

#if defined(H3_GLWF) || defined(H3_ANDROID) 
#include "png.h"
#include "pngconf.h"

// premultiply alpha, or the effect will wrong when want to use other pixel format in CCTexture2D,
// such as RGB888, RGB5A1
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))

typedef struct 
{
    unsigned char* data;
    int size;
    int offset;
}tImageSource;

static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
{
    tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);

    if((int)(isource->offset + length) <= isource->size)
    {
        memcpy(data, isource->data+isource->offset, length);
        isource->offset += length;
    }
    else
    {
        png_error(png_ptr, "pngReaderCallback failed");
    }
}
#endif

/*
 brief :
*/
H3Image::H3Image() 
{ 
	this->width = 0 ;
	this->height = 0 ;
	this->data = 0 ;
	this->type = H3_RGB_FORMAT_RGB24 ;
	this->hasAlpha = false ; 
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
	this->hasAlpha = true ;
    CGContextRef sprintContext = CGBitmapContextCreate(this->data,this->width, this->height, 8 , this->width*4 , CGImageGetColorSpace(sprintImage) , kCGImageAlphaPremultipliedLast);
    CGContextDrawImage(sprintContext, CGRectMake(0,0,this->width,this->height), sprintImage ) ;
    CGContextRelease(sprintContext) ;
	
	return true ;

#endif

#if defined( H3_GLWF ) || defined( H3_ANDROID ) 

	iS32 nDatalen = 0 ;
	void* file = SCHalFileOpen ( path , "rb" ) ;
	
	if ( 0 == file )
	{
		iLog ( "image file '%s' not exists" , path ) ;
		return false ;
	}

	nDatalen = SCHalFileLength ( file ) ;
	void* pData = (void* ) MMC_MALLOC ( nDatalen ) ;

	if ( 0 == pData )
	{
		iLog ( "not enough memory for image decode 1" ) ;
		SCHalFileClose ( file ) ;
		return false ;
	}

	SCHalFileRead ( file , pData , nDatalen ) ;
	SCHalFileClose ( file ) ;

	#define PNGSIGSIZE 8
    bool bRet = false;
	bool preMulti ;
    png_byte        header[PNGSIGSIZE]   = {0}; 
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;
	iU32 bitsPerComponent ;

    do 
    {
        // png header len is 8 bytes
       // H3ASSERT(nDatalen < PNGSIGSIZE);

        // check the data is png or not
        //memcpy(header, pData, PNGSIGSIZE);
        //H3ASSERT(png_sig_cmp(header, 0, PNGSIGSIZE));

        // init png_struct
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
        H3ASSERT(png_ptr);

        // init png_info
        info_ptr = png_create_info_struct(png_ptr);
        H3ASSERT(info_ptr);

#if (CC_TARGET_PLATFORM != CC_PLATFORM_BADA && CC_TARGET_PLATFORM != CC_PLATFORM_NACL)
        CC_BREAK_IF(setjmp(png_jmpbuf(png_ptr)));
#endif

        // set the read call back function
        tImageSource imageSource;
        imageSource.data    = (unsigned char*)pData;
        imageSource.size    = nDatalen;
        imageSource.offset  = 0;
        png_set_read_fn(png_ptr, &imageSource, pngReadCallback);
		
        // read png header info
        
        // read png file info
        png_read_info(png_ptr, info_ptr);
        
        this->width = png_get_image_width(png_ptr, info_ptr);
        this->height = png_get_image_height(png_ptr, info_ptr);
        bitsPerComponent = png_get_bit_depth(png_ptr, info_ptr);
        png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

        //CCLOG("color type %u", color_type);
        
        // force palette images to be expanded to 24-bit RGB
        // it may include alpha channel
        if (color_type == PNG_COLOR_TYPE_PALETTE)
        {
            png_set_palette_to_rgb(png_ptr);
        }
        // low-bit-depth grayscale images are to be expanded to 8 bits
        if (color_type == PNG_COLOR_TYPE_GRAY && bitsPerComponent < 8)
        {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        // expand any tRNS chunk data into a full alpha channel
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
        {
            png_set_tRNS_to_alpha(png_ptr);
        }  
        // reduce images with 16-bit samples to 8 bits
        if (bitsPerComponent == 16)
        {
            png_set_strip_16(png_ptr);            
        } 
        // expand grayscale images to RGB
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        {
            png_set_gray_to_rgb(png_ptr);
        }

        // read png data
        // m_nBitsPerComponent will always be 8
        bitsPerComponent = 8;
        png_uint_32 rowbytes;
        png_bytep* row_pointers = (png_bytep*)MMC_MALLOC( sizeof(png_bytep) * this->height );
        
        png_read_update_info(png_ptr, info_ptr);
        
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
		this->data = (unsigned char* )MMC_MALLOC( sizeof(unsigned char) * rowbytes * this->height );
        H3ASSERT(this->data);
        
        for (unsigned short i = 0; i < this->height; ++i)
        {
            row_pointers[i] = this->data + i*rowbytes;
        }
        png_read_image(png_ptr, row_pointers);
        
        png_read_end(png_ptr, NULL);
        
		iU32 channel = rowbytes/this->width;
        if (channel == 4)
        {
            this->hasAlpha = true;
			this->type = H3_RGB_FORMAT_RGBA32 ;
			unsigned int *tmp = (unsigned int *)this->data;
            for(unsigned short i = 0; i < this->height; i++)
            {
                for(unsigned int j = 0; j < rowbytes; j += 4)
                {
                    *tmp++ = CC_RGB_PREMULTIPLY_ALPHA( row_pointers[i][j], row_pointers[i][j + 1], 
                                                      row_pointers[i][j + 2], row_pointers[i][j + 3] );
                }
            }
            
            preMulti = true;
        }

        MMC_FREE(row_pointers);

        bRet = true;
    } while (0);

    if (png_ptr)
    {
        png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
    }
    MMC_FREE(pData) ;
	return bRet;

#endif

#if 0

	void* file = SCHalFileOpen ( path , "rb" ) ;

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
bool H3Image::init ( iU32 width , iU32 height , iU32 type ) 
{

	H3ASSERT( H3_RGB_FORMAT_RGB24 == type || H3_RGB_FORMAT_RGBA32 == type ) ;

	this->type = type ;
	this->width = width ;
	this->height = height ;

	iU32 pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGB24 == type )
		pixelBytes = 3 ;
	else if ( H3_RGB_FORMAT_RGBA32 == type )
		pixelBytes = 4 ;

	this->data = (iU8* ) MMC_MALLOC ( this->width * this->height * pixelBytes ) ;
	if ( 0 == this->data )
	{
		iLog ( "not enough memory for image") ;
		return false ;
	}
	memset ( this->data , 0 , this->width * this->height * pixelBytes ) ;

	return true ;

}

/*
 brief : rgb data , width , height , rgb type
*/
bool H3Image::init ( iU8* data , iU32 width , iU32 height , iU32 type ) 
{

	H3ASSERT( H3_RGB_FORMAT_RGB24 == type || H3_RGB_FORMAT_RGBA32 == type ) ;

	this->type = type ;
	this->width = width ;
	this->height = height ;

	iU32 pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGB24 == type )
		pixelBytes = 3 ;
	else if ( H3_RGB_FORMAT_RGBA32 == type )
		pixelBytes = 4 ;

	this->data = (iU8* ) MMC_MALLOC ( this->width * this->height * pixelBytes ) ;
	if ( 0 == this->data )
	{
		iLog ( "not enough memory for image") ;
		return false ;
	}
	memcpy ( this->data , data , this->width * this->height * pixelBytes ) ;

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
iU8* H3Image::getData () 
{
	return this->data ;
}

/*
 brief : assume the xy range is valid
*/
void H3Image::setData ( iU8 r , iU8 g , iU8 b , iU8 a , iS32 x , iS32 y ) 
{

	int pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGBA32 == this->getType () ) 
		pixelBytes = 4 ;
	else
		pixelBytes = 3 ;

	iS32 row = y * this->width * pixelBytes ;
	iS32 col = x * pixelBytes ;

	this->data [ row + col ] = r ;
	this->data [ row + col+1 ] = g ;
	this->data [ row + col+2 ] = b ;
	if ( H3_RGB_FORMAT_RGBA32 == this->getType () ) 
		this->data [ row + col+3 ] = a ;

}

/*
 brief : 
*/
bool H3Image::getAlpha () 
{
	return this->hasAlpha ;
}

/*
 brief : 
*/
iU32 H3Image::getType () 
{
	return this->type ;
}

/*
 brief : convert to rgba8888 32 bit
*/
void H3Image::convsert2rgba8888 () 
{
	iU32 pixelBytes = 3 ;
	if ( H3_RGB_FORMAT_RGBA32 == this->getType () ) 
		return ;

	iU8* mem = (iU8* ) MMC_MALLOC( this->width*this->height*4 ) ;
	H3ASSERT(mem) ;

	iU32 stride = this->width * pixelBytes ;
	iU32 newStride = this->width * 4 ;
	for( iS32 y = 0 ; y < this->getHeight() ; y ++ )
	{
		for( iS32 x = 0 , deltaX = 0 ; x < this->getWidth() * pixelBytes ; x+=pixelBytes ,deltaX += 4 )
		{
			unsigned char r = 0 ;
			unsigned char g = 0 ;
			unsigned char b = 0 ;
			unsigned char a = 0 ;

			r = this->data[y*stride+x] ;
			g = this->data[y*stride+x+1] ;
			b = this->data[y*stride+x+2] ;

			mem[y*newStride+deltaX] = r ;
			mem[y*newStride+deltaX+1] = g ;
			mem[y*newStride+deltaX+2] = b ;
			mem[y*newStride+deltaX+3] = 255 ;

		}

	}

	this->type = H3_RGB_FORMAT_RGBA32 ;
	MMC_FREE(this->data) ;
	this->data = mem ;

}

/*
 brief : scale the image 
*/
void H3Image::setScale ( iF32 scale ) 
{
	H3ASSERT(H3_RGB_FORMAT_RGBA32 == this->getType ()) ;
	H3Image* newImage = MMC_NEW H3Image() ;
	newImage->init( this->width*scale , this->height*scale , H3_RGB_FORMAT_RGBA32 ) ;

	unsigned char* imageData = this->getData() ;

	H3RasterPen pen = H3RasterPen( H3RasterColor(255,255,255,255), 1) ;

	int stride = 0;
	int pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGBA32 == this->getType () ) 
		pixelBytes = 4 ;
	else
		pixelBytes = 3 ;
	stride = this->width*pixelBytes ;

	for( iS32 y = 0 ; y < newImage->height ; y ++ )
	{
		iS32 newY = y / scale ;
		for( iS32 x = 0 , deltaX =0 ; x < newImage->width * pixelBytes ; x+=pixelBytes ,deltaX ++)
		{
			unsigned char r = 0 ;
			unsigned char g = 0 ;
			unsigned char b = 0 ;
			unsigned char a = 0 ;
			
			iS32 newX = deltaX / scale ;

			r = imageData[newY*stride+newX*pixelBytes] ;
			g = imageData[newY*stride+newX*pixelBytes+1] ;
			b = imageData[newY*stride+newX*pixelBytes+2] ;
			a = imageData[newY*stride+newX*pixelBytes+3] ;

			pen.setColor( H3RasterColor( r,g,b,a )) ;

			H3RasterRender::drawPoint ( newImage , &pen , deltaX , y ) ;
		}
	}

	this->width = this->width * scale ;
	this->height = this->height * scale ;

	this->data = (iU8* )MMC_REALLOC(this->data,this->width*this->height*pixelBytes) ;
	memcpy ( this->data , newImage->getData () , this->width*this->height*pixelBytes ) ;
	MMC_DELETE(newImage) ;

	return  ;
}

/*
 brief : scale the image 
*/
void H3Image::writeFile ( const iS8* path ) 
{
#ifdef WIN32
	int stride = 0;
	int pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGBA32 == this->getType () ) 
		pixelBytes = 4 ;
	else
		pixelBytes = 3 ;
	stride = this->width*pixelBytes ;

	stbi_write_png ( path , this->width , this->height , pixelBytes , this->data , stride ) ;
#endif
}
