
#include "H3ShaderCache.h"
#include "H3ShaderAmbientLight.h"
#include "H3ShaderDirectionalLight.h"
#include "H3ShaderPointLight.h"
#include "H3ShaderSpotLight.h"
#include "H3ShaderShadow.h"
#include "H3ShaderPrimitive.h"
#include "H3ShaderParticle.h"

static H3ShaderCache* gInstance = NULL ;

/*
 brief : 
*/
H3ShaderCache::H3ShaderCache () 
{
}

/*
 brief : 
*/
H3ShaderCache::~H3ShaderCache () 
{
	//	release H3Shader
    std::map < const iS8* , H3Shader* >::iterator it ;
	for ( it = this->shaderMap.begin() ; it!= this->shaderMap.end() ; ++it ) {
		MMC_DELETE(it->second) ;
	}

}

/*
 brief : 
*/
bool H3ShaderCache::init () 
{

	if ( 0 < this->shaderMap.size() ) {
		return true ;
	}

	this->shaderMap[H3_SHADER_CACHE_AMBIENTLIGHT] = MMC_NEW H3ShaderAmbientLight( "shaders/alight.vs" , "shaders/alight.fs" ) ;
	this->shaderMap[H3_SHADER_CACHE_DIRECTIONALLIGHT] = MMC_NEW H3ShaderDirectionalLight( "shaders/dlight.vs" , "shaders/dlight.fs" ) ;
	this->shaderMap[H3_SHADER_CACHE_POINTLIGHT] = MMC_NEW H3ShaderPointLight( "shaders/plight.vs" , "shaders/plight.fs" ) ;
	this->shaderMap[H3_SHADER_CACHE_SPOTLIGHT] = MMC_NEW H3ShaderSpotLight( "shaders/slight.vs" , "shaders/slight.fs" ) ;
	this->shaderMap[H3_SHADER_CACHE_SHADOW] = MMC_NEW H3ShaderShadow( "shaders/shadow.vs" , "shaders/shadow.fs" ) ;
	this->shaderMap[H3_SHADER_CACHE_PRIMITIVES] = MMC_NEW H3ShaderPrimitive( "shaders/primitive.vs" , "shaders/primitive.fs" ) ;
	this->shaderMap[H3_SHADER_CACHE_PARTICLE] = MMC_NEW H3ShaderParticle( "shaders/particle.vs" , "shaders/particle.fs" ) ;

	return true ;

}

/*
 brief : 
*/
H3ShaderCache* H3ShaderCache::getInstance () 
{
    if (!gInstance) {
        gInstance = MMC_NEW H3ShaderCache ();
        gInstance->init () ;
    }

    return gInstance;
}

/*
 brief : release singleton
*/
void H3ShaderCache::release () 
{
    if (gInstance) {
		MMC_DELETE(gInstance) ;
		gInstance = NULL ;
    }
}

/*
 brief : 
*/
H3Shader* H3ShaderCache::getShader ( const iS8* name ) 
{
	H3Shader* shader = 0 ;
	
	std::map < const iS8* , H3Shader* >::iterator it ;
	it = this->shaderMap.find( name ) ;
	if ( it != this->shaderMap.end() ) {
		shader = shaderMap[name] ;
	}

	return shader ;
}