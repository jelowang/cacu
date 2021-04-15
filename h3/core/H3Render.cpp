
/*

	+	H3Render.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
		   FACEGOOD.
	+
 
*/

#include "H3Std.h"
#include "H3Memory.h"
#include "H3gl.h"
#include "H3Scene.h"
#include "H3Render.h"

//#define SHADOW_DEBUG 
#ifdef SHADOW_DEBUG
#include "H3MeshRender.h"
H3Object* shadowMapTestQuad ;
#endif

/*
 brief :
*/
H3Render::H3Render () 
{
	this->render2Texture = false ;
	this->mipmapGenerated = false ;
	this->width = 0 ;
	this->height = 0 ;
}

/*
 brief :
*/
H3Render::~H3Render () 
{

	//	release texture
    std::map < const iS8* , H3Texture* >::iterator it ;
	for ( it = this->textureMap.begin() ; it != this->textureMap.end() ; ++it ) {
		it->second->release () ;
	}

	//	release shader
    std::map < const iS8* , H3Shader* >::iterator it2 ;
	for ( it2 = this->shaderMap.begin() ; it2 != this->shaderMap.end() ; ++it2 ) {
		MMC_DELETE(it2->second) ;
	}

	//	release fbo
    std::map < const iS8* , iU32 >::iterator it3 ;
	for ( it3 = this->fboMap.begin() ; it3 != this->fboMap.end() ; ++it3 ) {
		h3glDeleteFramebuffers(1,&it3->second) ;
	}

	//	release rbo
    std::map < const iS8* , iU32 >::iterator it4 ;
	for ( it4 = this->rboMap.begin() ; it4 != this->rboMap.end() ; ++it4 ) {
		glDeleteRenderbuffers(1,&it4->second) ;
	}

}

/*
 brief :
*/
void H3Render::init () 
{

	h3glFrontFace ( GL_CW ) ; 
	h3glCullFace ( GL_BACK ) ;
	h3glEnable ( GL_CULL_FACE ) ;
	H3_GL_ERROR_CHECK () ;

}

/*
 brief :
*/
void H3Render::clear () 
{
}

/*
 brief :
*/
void H3Render::resize ( iU32 width , iU32 height ) 
{
	this->width = width ;
	this->height = height ;

	if ( true == this->render2Texture ) {
		//	regenerate render texture
		renderToTexture () ;
	}

}

/*
 brief :
*/
void H3Render::setShadowEnable ( bool enable ) 
{
	 
	if ( true == enable ) {

		//	attach shadowmap to fbo
		iU32 fbo = 0 ;
		h3glGenFramebuffers ( 1 , &fbo ) ;
		h3glBindFramebuffer ( GL_FRAMEBUFFER , fbo ) ;

		this->addTexture ( H3_MATERIAL_TEXTURE_SHADOW , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_SHADOW , 1024 , 1024 ) , GL_TEXTURE31 ) ;		
		this->addShader ( H3_RENDER_SHADER_SHADOW , MMC_NEW H3ShaderShadow ( "shaders/shadow.vs" , "shaders/shadow.fs" ) ) ;
		this->addFbo ( H3_RENDER_FBO_SHADOW , fbo ) ;

		h3glFramebufferTexture2D ( 
			GL_FRAMEBUFFER , 
			GL_DEPTH_ATTACHMENT , 
			GL_TEXTURE_2D , 
			getTexture(H3_MATERIAL_TEXTURE_SHADOW)->getId() , 
			0 ) ;

		#if defined(H3_QT_5_4_2) || defined(H3_GLWF) 
		h3glDrawBuffer ( GL_NONE ) ;
		h3glReadBuffer ( GL_NONE ) ;
		#endif
		h3glBindFramebuffer ( GL_FRAMEBUFFER , 0 ) ;  

		#ifdef SHADOW_DEBUG
		H3Material* material = MMC_NEW H3Material() ;
		material->addTexture ( H3_MATERIAL_TEXTURE_DIFFUSE , getTexture(H3_MATERIAL_TEXTURE_SHADOW) , GL_TEXTURE0 ) ;
		material->addFloat ( H3_MATERIAL_SPECULARINTENSITY , 1.0f ) ;
		material->addFloat ( H3_MATERIAL_SHININESS , 30.0f ) ; 
		shadowMapTestQuad = MMC_NEW H3Object() ;
		shadowMapTestQuad->addComponent ( MMC_NEW H3MeshRender( MMC_NEW H3Mesh( "models/plane3.obj" ) , material ) ) ;
		shadowMapTestQuad->addComponent ( MMC_NEW H3AmbientLight( H3Vector3f(1.0f , 1.0f , 1.0f) ) ) ;
		shadowMapTestQuad->addComponent ( MMC_NEW H3DirectionalLight( H3Vector3f(1.0f , 0.0f , 0.0f) , H3Vector3f(0.6f , 0.0f , -0.1f) , 1.1f ) ) ;
		shadowMapTestQuad->setPosition(H3Vector3f(0.,0,0));
		#endif

		H3_GL_ERROR_CHECK () ;

	}

}

/*
 brief :
*/
H3Texture* H3Render::getTexture ( const iS8* name ) 
{

	H3Texture* texture = 0 ;
	
	std::map < const iS8* , H3Texture* >::iterator it ;
	it = this->textureMap.find( name ) ;
	if ( it != this->textureMap.end() ) {
		texture = this->textureMap[name] ;
	}

	return texture ;

}

/*
 brief :
*/
iS32 H3Render::getTextureIndex ( const iS8* name ) 
{
	iS32 value = 0 ; 

	std::map < const iS8* , iS32 >::iterator it ;
	it = this->textureIndexMap.find( name ) ;
	if ( it != this->textureIndexMap.end() ) {
		value = this->textureIndexMap[name] ;
	}
	
	return value ;
}

/*
 brief : return absolute index to shader
*/
iS32 H3Render::getTextureIndexEx ( const iS8* name ) 
{
	iS32 value = 0 ;

	std::map < const iS8* , iS32 >::iterator it ;
	it = this->textureIndexMap.find( name ) ;
	if ( it != this->textureIndexMap.end() ) {
		value = this->textureIndexMap[name] - GL_TEXTURE0 ;
	}

	return value ;
}

/*
 brief : get current matrix
*/
H3Matrix4x4f H3Render::getMatrix ( H3MATRIX_TYPE type ) 
{

	H3Matrix4x4f matrix ;

	if ( NULL == this->currentCamera )
		return matrix ;

	switch ( type ) {

		case H3MATRIX_TYPE::H3_MATRIX_PROJECTION :
			matrix = ((H3Camera*)this->currentCamera)->getProjection () ;
		break ;

		case H3MATRIX_TYPE::H3_MATRIX_VIEW_PROJECTION :
			matrix = ((H3Camera*)this->currentCamera)->getViewProjection () ;
		break ;

	}

	return matrix ;

}

/*
 brief : set rendering background color
*/
void H3Render::setBackgroundColor ( H3Vector3f color ) 
{
	h3glClearColor ( color.x , color.y , color.z , 1.0f ) ;
}

/*
 brief : rendering all to texture
*/
void H3Render::renderToTexture () 
{

	//	already exist
	if ( 0 != getTexture ( H3_RENDER_TEXTURE_RENDER ) ) {
		removeTexture ( H3_RENDER_TEXTURE_RENDER ) ;
		removeTextureIndex ( H3_RENDER_TEXTURE_RENDER ) ;
		removeFbo ( H3_RENDER_FBO_RENDER ) ;
		removeRbo ( H3_RENDER_RENDER_BUFFER ) ;
	}

	addTexture ( H3_RENDER_TEXTURE_RENDER , H3Texture::create ( H3TEXTURE_TYPE::H3_TEXTURE_RENDER , this->width , this->height ) , GL_TEXTURE30 ) ;

	// create a renderbuffer object to store depth info
	GLuint rboId;
	h3glGenRenderbuffers(1, &rboId);
	h3glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	h3glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,this->width , this->height);
	h3glBindRenderbuffer(GL_RENDERBUFFER, 0);
	addRbo ( H3_RENDER_RENDER_BUFFER , rboId ) ;

	// create a framebuffer object
	GLuint fboId;
	h3glGenFramebuffers(1, &fboId);
	h3glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	// attach the texture to FBO color attachment point
	h3glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,getTexture(H3_RENDER_TEXTURE_RENDER)->getId(),0);
	// attach the renderbuffer to depth attachment point
	h3glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboId);

	// check FBO status
	//GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//if(status != GL_FRAMEBUFFER_COMPLETE)

	h3glBindFramebuffer(GL_FRAMEBUFFER, 0);

	addFbo ( H3_RENDER_FBO_RENDER , fboId ) ;

	this->render2Texture = true ;

}

/*
 brief : return texture id
*/
iU32 H3Render::getRenderTexture () 
{

	iU32 id ;
	H3Texture* texture = getTexture ( H3_RENDER_TEXTURE_RENDER ) ;
	H3ASSERT(texture) ;
	id = texture->getId () ;

	if ( false == this->mipmapGenerated ) {
		glBindTexture ( GL_TEXTURE_2D , id ) ;
		glGenerateMipmap ( GL_TEXTURE_2D ) ;
		glBindTexture ( GL_TEXTURE_2D , 0 ) ;
		this->mipmapGenerated = false ;
	}

	H3_GL_ERROR_CHECK () ;

	return id ;
}

/*
 brief : return current camera
*/
void* H3Render::getCurrentCamera () 
{
	return this->currentCamera ;
}

/*
 brief :
*/
void H3Render::render ( iF32 deltaT , H3Component* camera , H3Object* rootObject )  
{

	if ( false == rootObject->getVisible () ) 
		return ;

	this->deltaT = deltaT ;
	this->currentCamera = camera ;

	if ( true == this->render2Texture ) {
		h3glBindFramebuffer ( GL_FRAMEBUFFER , getFbo (H3_RENDER_FBO_RENDER) ) ;
	}

	rendShadowPass ( rootObject ) ;
	rendGenericPass ( rootObject ) ;
	
	if ( true == this->render2Texture ) {
		h3glBindFramebuffer ( GL_FRAMEBUFFER , 0 ) ;
	}

}

/*
 brief :
*/
void H3Render::rendShadowPass ( H3Object* rootObject ) 
{

	//	for shadow pass
	if ( 0 != getShader(H3_RENDER_SHADER_SHADOW) ) {		
		H3Camera lightViewCamera ;
		H3Matrix4x4f lightProjMatrix ;

		//	for shadows turns
		h3glBindFramebuffer ( GL_FRAMEBUFFER , getFbo ( H3_RENDER_FBO_SHADOW ) ) ;
		h3glClear ( GL_DEPTH_BUFFER_BIT ) ;
		h3glViewport ( 0 , 0 , 1024 , 1024 ) ;
		h3glCullFace ( GL_FRONT ) ;		
		lightViewCamera.initProjection ( 70.0f , (iF32 )this->width/(iF32 )this->height , 0.1f , 10000.0f ) ;
		
		for ( SCClList* lw = rootObject->lightComponents.next ; lw ; lw = lw->next ) {

			H3Light* light = (H3Light* )lw->element ;
				
			lightViewCamera.setPosition ( light->getPosition() ) ;
			lightProjMatrix = lightViewCamera.getViewProjection () ;
			rootObject->render ( this , light , getShader(H3_RENDER_SHADER_SHADOW) , &lightViewCamera , &lightProjMatrix ) ;
	
		}
		h3glCullFace ( GL_BACK ) ;

		if ( true == this->render2Texture ) {
			h3glBindFramebuffer ( GL_FRAMEBUFFER , getFbo (H3_RENDER_FBO_RENDER) ) ;
		} else {
			h3glBindFramebuffer ( GL_FRAMEBUFFER , 0 ) ;
		}

		H3_GL_ERROR_CHECK () ;
	}

}

/*
 brief :
*/
void H3Render::rendGenericPass ( H3Object* rootObject ) 
{
	H3Camera lightViewCamera ;
	H3Matrix4x4f lightProjMatrix ;
	lightViewCamera.initProjection ( 70.0f , (iF32 )this->width/(iF32 )this->height , 0.1f , 10000.0f ) ;

	//	for normal pass
    h3glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;
	h3glEnable ( GL_BLEND ) ;
	h3glDepthFunc ( GL_LEQUAL ) ;
	h3glViewport ( 0 , 0 , this->width , this->height ) ;
	
	//	for lights
	for ( SCClList* lw = rootObject->lightComponents.next ; lw ; lw = lw->next ) {
		  
		H3Light* light = (H3Light* )lw->element ;
		lightViewCamera.setPosition ( light->getPosition() ) ;
		lightProjMatrix = lightViewCamera.getViewProjection () ;
		//rootObject->material->addTexture ( "diffuseTexture" , shadowMapTestQuad->material->getTexture("diffuseTexture") , GL_TEXTURE0 ) ; 
		rootObject->render ( this , light , light->shader , this->currentCamera , &lightProjMatrix ) ;

	}

	//	call rootObject custmized drawing
	((H3Scene* )rootObject)->draw ( this->deltaT ) ;

	h3glDisable ( GL_BLEND ) ; 

	H3_GL_ERROR_CHECK () ;

}

/*
 brief : these add-xxx method just used for render-self. this method can add the same name repeatly
*/
void H3Render::addTexture ( const iS8* name , H3Texture* texture , iS32 index ) 
{

	std::map < const iS8* , H3Texture* >::iterator it ;
	it = this->textureMap.find( name ) ;
	if( it != this->textureMap.end() ) {
		it->second->release () ;
		this->textureMap.erase ( name ) ;
	}
		
	texture->addRef () ;
	this->textureMap[name] = texture ;
	addTextureIndex ( name , index ) ;
}

/*
 brief : this method can add the same name repeatly
*/
void H3Render::addTextureIndex ( const iS8* name , iS32 index ) 
{
	std::map < const iS8* , iS32 >::iterator it ;
	it = this->textureIndexMap.find( name ) ;
	if ( it != this->textureIndexMap.end() ) {
		this->textureIndexMap.erase ( name ) ;
	}
	this->textureIndexMap[name] = index ;
}

/*
 brief :
*/
void H3Render::addShader ( const iS8* name , H3Shader* shader ) 
{

	std::map < const iS8* , H3Shader* >::iterator it ;
	it = this->shaderMap.find( name ) ;
	H3ASSERT( it == this->shaderMap.end() ) ;

	this->shaderMap[name] = shader ;
}

/*
 brief :
*/
void H3Render::addFbo ( const iS8* name , iU32 value ) 
{
	std::map < const iS8* , iU32 >::iterator it ;
	it = this->fboMap.find( name ) ;
	H3ASSERT( it == this->fboMap.end() ) ;
	this->fboMap[name] = value ;
}

/*
 brief :
*/
void H3Render::addRbo ( const iS8* name , iU32 value ) 
{
	std::map < const iS8* , iU32 >::iterator it ;
	it = this->rboMap.find( name ) ;
	H3ASSERT( it == this->rboMap.end() ) ;
	this->rboMap[name] = value ;
}

/*
 brief : remove and release H3Texture* handle
*/
void H3Render::removeTexture ( const iS8* name ) 
{
	std::map < const iS8* , H3Texture* >::iterator it ;
	it = this->textureMap.begin () ;
	while ( it != this->textureMap.end() ) {
		if ( 0 == strcmp ( it->first , name ) ) {
			it->second->release () ;
			this->textureMap.erase ( it ) ; 
			break ;
		}
		it ++ ;
	}
}

/*
 brief : 
*/
void H3Render::removeTextureIndex ( const iS8* name ) 
{
	std::map < const iS8* , iS32 >::iterator it ;
	it = this->textureIndexMap.begin () ;
	while ( it != this->textureIndexMap.end() ) {
		if ( 0 == strcmp ( it->first , name ) ) {
			this->textureIndexMap.erase ( it ) ; 
			break ;
		}
		it ++ ;
	}
}

/*
 brief : remove and release fbo
*/
void H3Render::removeFbo ( const iS8* name ) 
{

	std::map < const iS8* , iU32 >::iterator it ;
	it = this->fboMap.begin () ;
	while ( it != this->fboMap.end() ) {
		if ( 0 == strcmp ( it->first , name ) ) {
			h3glDeleteFramebuffers ( 1 , &it->second ) ;
			this->fboMap.erase ( it ) ; 
			break ;
		}
		it ++ ;
	}

}

/*
 brief : remove and release rbo
*/
void H3Render::removeRbo ( const iS8* name ) 
{

	std::map < const iS8* , iU32 >::iterator it ;
	it = this->rboMap.begin () ;
	while ( it != this->rboMap.end() ) {
		if ( 0 == strcmp ( it->first , name ) ) {
			h3glDeleteRenderbuffers ( 1 , &it->second ) ;
			this->rboMap.erase ( it ) ; 
			break ;
		}
		it ++ ;
	}

}

/*
 brief :
*/
H3Shader* H3Render::getShader ( const iS8* name ) 
{

	H3Shader* shader = 0 ;
	
	std::map < const iS8* , H3Shader* >::iterator it ;
	it = this->shaderMap.find( name ) ;
	if ( it != this->shaderMap.end() ) {
		shader = this->shaderMap[name] ;
	}

	return shader ;

}

/*
 brief :
*/
iU32 H3Render::getFbo ( const iS8* name ) 
{
	iU32 value = 0 ;

	std::map < const iS8* , iU32 >::iterator it ;
	it = this->fboMap.find( name ) ;
	if ( it != this->fboMap.end() ) {
		value = this->fboMap[name] ;
	}

	return value ;
}

/*
 brief :
*/
iU32 H3Render::getRbo ( const iS8* name ) 
{
	iU32 value = 0 ;

	std::map < const iS8* , iU32 >::iterator it ;
	it = this->rboMap.find( name ) ;
	if ( it != this->rboMap.end() ) {
		value = this->rboMap[name] ;
	}

	return value ;
}