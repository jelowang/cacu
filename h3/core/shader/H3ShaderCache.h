
#if defined(_MSC_VER)
#pragma once
#endif

#include "H3Std.h"
#include "H3Shader.h"

#define H3_SHADER_CACHE_AMBIENTLIGHT "shaderAmbientLight"
#define H3_SHADER_CACHE_DIRECTIONALLIGHT "shaderDirectionalLight"
#define H3_SHADER_CACHE_POINTLIGHT "shaderPointLight"
#define H3_SHADER_CACHE_SPOTLIGHT "shaderSpotLight"
#define H3_SHADER_CACHE_SHADOW "shaderShadow"
#define H3_SHADER_CACHE_PRIMITIVES "shaderPrimitives"
#define H3_SHADER_CACHE_PARTICLE "shaderParticle"

class H3ShaderCache : virtual public H3Memory
{
public :
	/*
	 brief : 
	*/
	H3ShaderCache () ;

	/*
	 brief : 
	*/
	~H3ShaderCache () ;

	/*
	 brief : 
	*/
	bool init () ;

	/*
	 brief : get singleton handle
	*/
	static H3ShaderCache* getInstance () ;

	/*
	 brief : release singleton
	*/
	static void release () ;

	/*
	 brief : 
	*/
	H3Shader* getShader ( const iS8* name ) ;

private :
	std::map < const iS8* , H3Shader* > shaderMap ;

} ;
