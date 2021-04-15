
/*

	+	H3Light.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Shader.h"
#include "H3ShaderAmbientLight.h"
#include "H3ShaderPointLight.h"
#include "H3ShaderDirectionalLight.h"
#include "H3ShaderSpotLight.h"
#include "H3ShaderCache.h"
#include "H3Light.h"

/*
 brief :
*/
H3Light::H3Light () 
{
	this->type = H3COMPONENT_TYPE_LIGHT ;
}

/*
 brief :
*/
H3Light::~H3Light () 
{
}

/*
 brief :
*/
void H3Light::setColor ( H3Vector3f color ) 
{
	this->color = color ;
}

/*
 brief : H3AmbientLight
*/
H3AmbientLight::H3AmbientLight ( H3Vector3f color )
{
	this->shader = (void* )H3ShaderCache::getInstance()->getShader ( H3_SHADER_CACHE_AMBIENTLIGHT ) ;
	this->color = color ;
	this->type = H3COMPONENT_TYPE_AMBIENT_LIGHT ;
}

H3AmbientLight::~H3AmbientLight () 
{
}

/*
 brief : H3PointLight
*/
H3PointLight::H3PointLight ( H3Vector3f position , H3Vector3f color , iF32 intensity , iF32 constant , iF32 linear , iF32 quadratic ) 
{
	this->shader = (void* )H3ShaderCache::getInstance()->getShader ( H3_SHADER_CACHE_POINTLIGHT ) ;
	this->color = color ;
	this->setPosition( position ) ;
	this->intensity = intensity ;
	this->constant = constant ;
	this->linear = linear ;
	this->quadratic = quadratic ;
	this->type = H3COMPONENT_TYPE_POINT_LIGHT ;
}

H3PointLight::~H3PointLight () 
{
}

/*
 brief : H3DirectionalLight
*/
H3DirectionalLight::H3DirectionalLight ( H3Vector3f color , H3Vector3f direction , iF32 intensity )
{
	this->shader = (void* )H3ShaderCache::getInstance()->getShader ( H3_SHADER_CACHE_DIRECTIONALLIGHT ) ;
	this->color = color ;
	this->intensity = intensity ;
	this->direction = direction ;
	this->type = H3COMPONENT_TYPE_DIRECTIONAL_LIGHT ;
}

H3DirectionalLight::~H3DirectionalLight () 
{
}

H3SpotLight::H3SpotLight( H3Vector3f position , H3Vector3f direction , H3Vector3f color , iF32 intensity , iF32 cutOff ) 
{
	this->shader = (void* )H3ShaderCache::getInstance()->getShader ( H3_SHADER_CACHE_SPOTLIGHT ) ;
	this->color = color ;
	this->setPosition( position ) ;
	this->intensity = intensity ;
	this->cutOff = cutOff ;
	this->type = H3COMPONENT_TYPE_SPOT_LIGHT ;
}

H3SpotLight::~H3SpotLight() 
{
}