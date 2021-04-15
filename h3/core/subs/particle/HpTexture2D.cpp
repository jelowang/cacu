
/*
+	HpTexture2D.cpp
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "HpAdapter.h"
#include "H3Gl.h"
#include "HpTexture2D.h"
#include "HpTextureCache.h"

#ifdef HP_COCOS2D_PORT
#include "platform/CCImage.h"
#include "platform/CCFileUtils.h"
#endif

#ifdef HP_H3_PORT
#include "H3Image.h"
#endif

/*
	brief : 
*/
HpTexture2D::~HpTexture2D() 
{
	h3glDeleteTextures ( 1 , &this->textureId ) ;
}

/*
	brief : Initializes a texture from a PVR file
*/
bool HpTexture2D::initWithPNGFile ( const char* file ) 
{
	
	strcpy ( this->name , file ) ;
	this->refCount = 0 ;

#ifdef HP_COCOS2D_PORT
	std::string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
	CCImage image ;
	image.initWithImageFile ( m_sPlistFile.data () ) ;

	h3glGenTextures ( 1 , &this->textureId ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId );
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;
	
	if ( true == image.isPremultipliedAlpha() ) {
		h3glTexImage2D ( 
			GL_TEXTURE_2D , 0 , GL_RGBA , image.getWidth () , image.getHeight () , 0 , GL_RGBA ,
			GL_UNSIGNED_BYTE , image.getData () ) ;
	} else {
		h3glTexImage2D ( 
			GL_TEXTURE_2D , 0 , GL_RGB , image.getWidth () , image.getHeight () , 0 , GL_RGB ,
			GL_UNSIGNED_BYTE , image.getData () ) ;
	}
 
	h3glBindTexture ( GL_TEXTURE_2D , 0 ) ;

	m_contentSize.setSize ( (float )image.getWidth () , (float )image.getHeight () ) ;
	m_pixelsWide = (int )m_contentSize.width ;
	m_pixelsHigh = (int )m_contentSize.height ;
	if ( true == image.isPremultipliedAlpha() ) {
		m_ePixelFormat = eHpTexture2DPixelFormat_RGBA8888 ;
	} else {
		m_ePixelFormat = eHpTexture2DPixelFormat_RGB888 ;
	}

	return true ;

#endif

#ifdef HP_H3_PORT
	H3Image image ;
	bool ret = image.init ( (iS8* )file ) ;

	if ( false == ret ) {
		return false ;
	}

    GLint last_texture ;
    h3glGetIntegerv ( GL_TEXTURE_BINDING_2D , &last_texture) ;

	h3glGenTextures ( 1 , &this->textureId ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId );
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;
	
	if ( true == image.getAlpha() ) {
		h3glTexImage2D ( 
			GL_TEXTURE_2D , 0 , GL_RGBA , image.getWidth () , image.getHeight () , 0 , GL_RGBA ,
			GL_UNSIGNED_BYTE , image.getData () ) ;
	} else {
		h3glTexImage2D ( 
			GL_TEXTURE_2D , 0 , GL_RGB , image.getWidth () , image.getHeight () , 0 , GL_RGB ,
			GL_UNSIGNED_BYTE , image.getData () ) ;
	}
 
    // Restore state
    h3glBindTexture ( GL_TEXTURE_2D , last_texture ) ;

	m_contentSize.setSize ( (float )image.getWidth () , (float )image.getHeight () ) ;
	m_pixelsWide = (int )m_contentSize.width ;
	m_pixelsHigh = (int )m_contentSize.height ;
	if ( true == image.getAlpha() ) {
		m_ePixelFormat = eHpTexture2DPixelFormat_RGBA8888 ;
	} else {
		m_ePixelFormat = eHpTexture2DPixelFormat_RGB888 ;
	}

	return true ;
#endif

	return true ;
}

/*
	brief : 
*/
bool HpTexture2D::initWithData ( char* name , const void* data , HpTexture2DPixelFormat pixelFormat , unsigned int pixelsWide , unsigned int pixelsHigh , const HpSize& contentSize ) 
{

	strcpy ( this->name , name ) ;

    unsigned int bitsPerPixel;

    bitsPerPixel = bitsPerPixelForFormat(pixelFormat);

    unsigned int bytesPerRow = pixelsWide * bitsPerPixel / 8;

    if(bytesPerRow % 8 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
    }
    else if(bytesPerRow % 4 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    }
    else if(bytesPerRow % 2 == 0)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    }
    else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

	glGenTextures(1, &textureId);

#ifdef COCOS2DX_PORT
    ccGLBindTexture2D(m_name);
#else
	glBindTexture ( GL_TEXTURE_2D , textureId ) ;
#endif

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    switch(pixelFormat)
    {

		case eHpTexture2DPixelFormat_RGBA8888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		case eHpTexture2DPixelFormat_RGB888:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case eHpTexture2DPixelFormat_RGBA4444:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
			break;
		case eHpTexture2DPixelFormat_RGB5A1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
			break;
		case eHpTexture2DPixelFormat_RGB565:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
			break;
		case eHpTexture2DPixelFormat_AI88:
			H3ASSERT(0) ;
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		case eHpTexture2DPixelFormat_A8:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
			break;
		case eHpTexture2DPixelFormat_I8:
			H3ASSERT(0) ;
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, (GLsizei)pixelsWide, (GLsizei)pixelsHigh, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
			break;
		default:
			assert(0);

    }

    m_contentSize = contentSize;
    m_pixelsWide = pixelsWide;
    m_pixelsHigh = pixelsHigh;
    m_ePixelFormat = pixelFormat;

	return true ;

}

/*
	brief : 
*/
unsigned int HpTexture2D::getId () 
{
	return this->textureId ;
}

/*
	brief : 
*/
unsigned int HpTexture2D::bitsPerPixelForFormat ( HpTexture2DPixelFormat format )
{

	unsigned int ret=0;

	switch (format) {
		case eHpTexture2DPixelFormat_RGBA8888:
			ret = 32;
		break;
		case eHpTexture2DPixelFormat_RGB888 :
			ret = 24 ;
		break ;
		case eHpTexture2DPixelFormat_RGB565:
			ret = 16;
		break;
		case eHpTexture2DPixelFormat_RGBA4444:
			ret = 16;
		break;
		case eHpTexture2DPixelFormat_RGB5A1:
			ret = 16;
		break;
		case eHpTexture2DPixelFormat_AI88:
			ret = 16;
		break;
		case eHpTexture2DPixelFormat_A8:
			ret = 8;
		break;
		case eHpTexture2DPixelFormat_I8:
			ret = 8;
		break;
		case eHpTexture2DPixelFormat_PVRTC4:
			ret = 4;
		break;
		case eHpTexture2DPixelFormat_PVRTC2:
			ret = 2;
		break;
		default:
			ret = -1;
			assert(0);
		break;
	}

	return ret;

}

/*
	brief : 
*/
HpSize HpTexture2D::getContentSize () 
{
	HpSize size ( this->m_pixelsWide , this->m_pixelsHigh ) ;
	return size ;
}

/* 
 brief : call use inside engine 
*/
void HpTexture2D::addRef () 
{
	this->refCount ++ ;
}

/*
 brief : engine will call this method to release material memorys
*/
void HpTexture2D::release () 
{
	this->refCount -- ;
	
	if ( 0 == this->refCount ) {
		HpTextureCache::getInstance()->deleteTexture ( H3UtilString2Number(this->name).getValue() ) ;
		MMC_DELETE(this) ;
	}
}