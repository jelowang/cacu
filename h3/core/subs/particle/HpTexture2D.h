
/*
+	HpTexture2D.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/

#ifndef __HpTEXTURE_H__
#define __HpTEXTURE_H__

#include "HpAdapter.h"
#include "H3Memory.h"
#include "HpGeometry.h"

/*
	brief : 
*/
typedef enum {

    // 32-bit texture: RGBA8888
    eHpTexture2DPixelFormat_RGBA8888 ,
    // 24-bit texture: RGBA888
    eHpTexture2DPixelFormat_RGB888 ,
    // 16-bit texture without Alpha channel
    eHpTexture2DPixelFormat_RGB565 ,
    // 8-bit textures used as masks
    eHpTexture2DPixelFormat_A8 ,
    // 8-bit intensity texture
    eHpTexture2DPixelFormat_I8 ,
    // 16-bit textures used as masks
    eHpTexture2DPixelFormat_AI88 ,
    // 16-bit textures: RGBA4444
    eHpTexture2DPixelFormat_RGBA4444 ,
    // 16-bit textures: RGB5A1
    eHpTexture2DPixelFormat_RGB5A1 ,    
    // 4-bit PVRTC-compressed texture: PVRTC4
    eHpTexture2DPixelFormat_PVRTC4 ,
    // 2-bit PVRTC-compressed texture: PVRTC2
    eHpTexture2DPixelFormat_PVRTC2 ,
    // Default texture format: RGBA8888
    eHpTexture2DPixelFormat_Default = eHpTexture2DPixelFormat_RGBA8888 ,

} HpTexture2DPixelFormat;

/*
	brief : Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).	  
*/
class HpTexture2D : virtual public H3Memory
{

public:

	/*
		brief : 
	*/
	~HpTexture2D() ;

	/*
		brief : Initializes a texture from a PVR file
	*/
    bool initWithPNGFile ( const char* file ) ;    

	/*
		brief : 
	*/
    bool initWithData ( char* name , const void* data , HpTexture2DPixelFormat pixelFormat , unsigned int pixelsWide , unsigned int pixelsHigh , const HpSize& contentSize ) ;

	/*
		brief : 
	*/
    unsigned int getId () ;

	/*
		brief : 
	*/
	HpSize getContentSize () ;

	/* 
	 brief : call use inside engine 
	*/
	void addRef () ;

	/*
	 brief : engine will call this method to release material memorys
	*/
	void release () ;

private :
	/*
		brief : 
	*/
	unsigned int bitsPerPixelForFormat ( HpTexture2DPixelFormat format ) ;

private :
	//	file name
	char name [64] ;
	unsigned int refCount ;
	unsigned int textureId ;
    HpSize m_contentSize ;
    unsigned int m_pixelsWide ;
    unsigned int m_pixelsHigh ;
    HpTexture2DPixelFormat m_ePixelFormat ;

} ;

#endif