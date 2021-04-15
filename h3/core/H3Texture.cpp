
/*

	+	H3Texture.h
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3gl.h" 
#include "H3Image.h" 
#include "H3Texture.h" 

/*
 brief :
*/
H3Texture::H3Texture() 
{
	this->refCount = 0 ;
	this->textureId = 0 ;
	this->width = 0 ;
	this->height = 0 ;  
}

/*
 brief :
*/
H3Texture::~H3Texture() 
{
	if ( this->textureId > 0 )
		h3glDeleteTextures ( 1 , &this->textureId ) ;
}

H3Texture* H3Texture::create ( H3TEXTURE_TYPE type , iS8* path ) 
{
	H3Texture* texture = 0 ;
	H3ASSERT(H3TEXTURE_TYPE::H3_TEXTURE_PIXEL == type ) ;
	texture = MMC_NEW H3Texture() ;
	texture->initPixelTexture ( path ) ;
	return texture ;
}

H3Texture* H3Texture::create ( H3TEXTURE_TYPE type , iU32 width , iU32 height , void* data , iU32 rgbType ) 
{
	H3Texture* texture = 0 ;

	switch ( type )
	{
		case H3TEXTURE_TYPE::H3_TEXTURE_PIXEL :
			texture = MMC_NEW H3Texture() ;
			texture->initPixelTexture ( width , height , data , rgbType ) ;
		break ;
		case H3TEXTURE_TYPE::H3_TEXTURE_SHADOW :
			texture = MMC_NEW H3Texture() ;
			texture->initShadowTexture ( width , height ) ;
		break ;

		case H3TEXTURE_TYPE::H3_TEXTURE_RENDER :
			texture = MMC_NEW H3Texture() ;
			texture->initRenderTexture ( width , height ) ;
		break ;
	}
	
	return texture ;
}


H3Texture* H3Texture::create ( H3TEXTURE_TYPE type , iU32 width , iU32 height )
{
	H3Texture* texture = 0 ;
	switch ( type )
	{
		case H3TEXTURE_TYPE::H3_TEXTURE_PIXEL :
			texture = MMC_NEW H3Texture() ;
			texture->initPixelTexture ( width , height ) ;
		break ;
		case H3TEXTURE_TYPE::H3_TEXTURE_SHADOW :
			texture = MMC_NEW H3Texture() ;
			texture->initShadowTexture ( width , height ) ;
		break ;

		case H3TEXTURE_TYPE::H3_TEXTURE_RENDER :
			texture = MMC_NEW H3Texture() ;
			texture->initRenderTexture ( width , height ) ;
		break ;
	}
	return texture ;
}

/*
 brief :
*/
iU32 H3Texture::getWidth () 
{
	return this->width ;
}

/*
 brief :
*/
iU32 H3Texture::getHeight () 
{
	return this->height ;
}

/*
 brief :
*/
void H3Texture::bind ( iU32 index ) 
{
	h3glActiveTexture ( index ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId );
}

/*
 brief :
*/
void H3Texture::unbind ( iU32 index ) 
{
	h3glBindTexture ( GL_TEXTURE_2D , index );
}

/*
 brief :
*/
iU32 H3Texture::getId () 
{
	return textureId ;
}

/*
 brief :
*/
void H3Texture::setId ( iU32 textureId ) 
{
	this->textureId = textureId ;
}

/* 
 brief : call use inside engine 
*/
void H3Texture::addRef () 
{
	this->refCount ++ ;
}

/*
 brief : engine will call this method to release material memorys
*/
void H3Texture::release () 
{
	this->refCount -- ;

	if ( 0 == this->refCount ) {
		MMC_DELETE(this) ;
	}
}

/*
 brief :
*/
bool H3Texture::initPixelTexture ( iS8* path ) 
{

	H3Image image ; 

	bool ret = image.init ( path ) ;

	if ( false == ret ) {
		iLog ( "textures create failed" ) ;  
		return false ;
	}

	this->width = image.getWidth () ;
	this->height = image.getHeight () ;

    GLint last_texture ;
    h3glGetIntegerv ( GL_TEXTURE_BINDING_2D , &last_texture) ;
	   
	h3glGenTextures ( 1 , &this->textureId ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId );
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;
	h3glPixelStorei ( GL_UNPACK_ALIGNMENT , 1 ) ;
	MMC_CHECK_MEMORY_OVERFLOW () ; 
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
	 
	H3_GL_ERROR_CHECK () ;

	return true ;

}

/*
 brief :
*/
bool H3Texture::initPixelTexture ( iU32 width , iU32 height , void* data , iU32 rgbType ) 
{

	this->width = width ;
	this->height = height ;

    GLint last_texture ;
    h3glGetIntegerv ( GL_TEXTURE_BINDING_2D , &last_texture) ;

	h3glGenTextures ( 1 , &this->textureId ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId );
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;
	
	if ( H3_RGB_FORMAT_RGBA32 == rgbType ) {
		h3glTexImage2D ( 
			GL_TEXTURE_2D , 0 , GL_RGBA , width , height , 0 , GL_RGBA ,
			GL_UNSIGNED_BYTE , data ) ;
	} else {
		h3glTexImage2D ( 
			GL_TEXTURE_2D , 0 , GL_RGB , width , height , 0 , GL_RGB ,
			GL_UNSIGNED_BYTE , data ) ;
	}
	   
    // Restore state
    glBindTexture ( GL_TEXTURE_2D , last_texture ) ;

	H3_GL_ERROR_CHECK () ;

	return true ;

}

/*
 brief :
*/
bool H3Texture::initPixelTexture ( iU32 width , iU32 height ) 
{

	this->width = width ;
	this->height = height ;

    GLint last_texture ;
    h3glGetIntegerv ( GL_TEXTURE_BINDING_2D , &last_texture) ;

	h3glGenTextures ( 1 , &this->textureId ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId );
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR ) ;
    h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR ) ;

    // Restore state
    glBindTexture ( GL_TEXTURE_2D , last_texture ) ;

	H3_GL_ERROR_CHECK () ;

	return true ;


}
/*
 brief :
*/
bool H3Texture::initShadowTexture ( iU32 width , iU32 height ) 
{

	this->width = width ;
	this->height = height ;

    GLint last_texture;
    h3glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

	h3glGenTextures ( 1 , &this->textureId);
	h3glBindTexture ( GL_TEXTURE_2D , this->textureId);
	h3glTexImage2D ( GL_TEXTURE_2D , 0 , GL_DEPTH_COMPONENT ,  width , height , 0 , GL_DEPTH_COMPONENT , GL_FLOAT , NULL ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST ) ;
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT ) ; 
	h3glTexParameteri ( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT ) ; 
	#ifdef H3_QT_5_4_2
		h3glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		h3glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		iF32 borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
		h3glTexParameterfv ( GL_TEXTURE_2D , GL_TEXTURE_BORDER_COLOR , borderColor ) ; 
	#else 
		#ifdef H3_GLWF
			h3glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			h3glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			iF32 borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
			h3glTexParameterfv ( GL_TEXTURE_2D , GL_TEXTURE_BORDER_COLOR , borderColor ) ; 
		#endif
	#endif

    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

	H3_GL_ERROR_CHECK () ;

	return true ;

}

/*
 brief :
*/
bool H3Texture::initRenderTexture ( iU32 width , iU32 height ) 
{

	this->width = width ;
	this->height = height ;

#ifdef WIN32

    GLint last_texture;
    h3glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

	glGenTextures(1, &this->textureId);
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_AUTO_GENERATE_MIPMAP, GL_TRUE); 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
    // Restore state
    glBindTexture(GL_TEXTURE_2D, last_texture);

	H3_GL_ERROR_CHECK () ;

#endif
	return true ;

}
