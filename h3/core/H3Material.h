
/*

	+	H3Material.h
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3Material_H
#define H3Material_H

#include "H3Std.h"
#include "H3Memory.h"
#include "H3Texture.h"
#include "H3MathUtil.h"

#define H3_MATERIAL_TEXTURE_SHADOW "shadowTexture"
#define H3_MATERIAL_TEXTURE_DIFFUSE "diffuseTexture"
#define H3_MATERIAL_TEXTURE_NORMAL "normalTexture"
#define H3_MATERIAL_COLOR_AMBIENT "ambientColor"
#define H3_MATERIAL_COLOR_DIFFUSE "diffuseColor"
#define H3_MATERIAL_COLOR_SPECULAR "specularColor"
#define H3_MATERIAL_SPECULARINTENSITY "specularIntensity"
#define H3_MATERIAL_SHININESS "shininess"
#define H3_MATERIAL_SSS "sss"
#define H3_MATERIAL_FRESNEL "fresnel"

/*
 brief :
*/
class H3Material : virtual public H3Memory
{
public :

	/*
	 brief :
	*/
	H3Material() ;

	/*
	 brief :
	*/
	~H3Material() ;

	/*
	 brief :
	*/
	void addTexture ( const iS8* name , H3Texture* texture , iS32 index ) ;

	/*
	 brief :
	*/
	void addVector ( const iS8* name , H3Vector3f* vector ) ;

	/*
	 brief :
	*/
	void addFloat ( const iS8* name , iF32 value ) ;

	/*
	 brief :
	*/
	void addColor ( const iS8* name , H3Vector3f* color ) ;

	/*
	 brief :
	*/
	H3Texture* getTexture ( const iS8* name ) ;

	/*
	 brief :
	*/
	iS32 getTextureIndex ( const iS8* name ) ;

	/*
	 brief :
	*/
	iS32 getTextureIndexEx ( const iS8* name ) ;

	/*
	 brief :
	*/
	H3Vector3f* getVector ( const iS8* name ) ;

	/* 
	 brief :
	*/
	iF32 getFloat ( const iS8* name ) ;

	/*
	 brief :
	*/
	H3Vector3f* getColor ( const iS8* name ) ;

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
	void addTextureIndex ( const iS8* name , iS32 index ) ;

private :

	iS32 refCount ;

	H3Texture* defaultDiffuseTexture ;
	H3Texture* defaultNormalTexture ;

	std::map < const iS8* , H3Texture* > textureMap ;
	std::map < const iS8* , iS32 > textureIndexMap ;
	std::map < const iS8* , H3Vector3f* > vector3fMap ;
	std::map < const iS8* , iF32 > floatMap ;
	std::map < const iS8* , H3Vector3f* > colorMap ;

} ; 

#endif
