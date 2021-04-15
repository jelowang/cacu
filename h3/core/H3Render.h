
/*

	+	H3Render.h
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

#ifndef H3Render_H
#define H3Render_H

#include "H3Std.h"
#include "H3Mesh.h"
#include "H3Object.h"
#include "H3Camera.h"
#include "H3Light.h"
#include "H3ShaderShadow.h"

class H3Render : virtual public H3Memory
{

public :
    /*
     brief :
    */
	H3Render () ;

    /*
     brief :
    */
	virtual ~H3Render () ;

    /*
     brief :
    */
	void init () ;

	/*
     brief :
    */
	void clear () ;

	/*
	 brief :
	*/
 	void resize ( iU32 width , iU32 height ) ;

	/*
	 brief :
	*/
 	void setShadowEnable ( bool enable ) ;

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
	 brief : get current matrix
	*/
	H3Matrix4x4f getMatrix ( H3MATRIX_TYPE type ) ;

	/*
	 brief : set rendering background color
	*/
	void setBackgroundColor ( H3Vector3f color ) ;

	/*
	 brief : rendering all to texture
	*/
	void renderToTexture () ;

	/*
	 brief : return texture id
	*/
	iU32 getRenderTexture () ;

	/*
	 brief : return current camera
	*/
	void* getCurrentCamera () ;

    /*
     brief :
    */
	void render ( iF32 deltaT , H3Component* camera , H3Object* rootObject ) ;

private :

	/*
	 brief :
	*/
	void rendShadowPass ( H3Object* rootObject ) ;

	/*
	 brief :
	*/
	void rendGenericPass ( H3Object* rootObject ) ;

	/*
	 brief :
	*/
	void addTexture ( const iS8* name , H3Texture* texture , iS32 index ) ;

	/*
	 brief :
	*/
	void addTextureIndex ( const iS8* name , iS32 index ) ;

	/*
	 brief :
	*/
	void addShader ( const iS8* name , H3Shader* shader ) ;

	/*
	 brief :
	*/
	void addFbo ( const iS8* name , iU32 value ) ;

	/*
	 brief :
	*/
	void addRbo ( const iS8* name , iU32 value ) ;

	/*
	 brief : remove and release H3Texture* handle
	*/
	void removeTexture ( const iS8* name ) ;

	/*
	 brief :
	*/
	void removeTextureIndex ( const iS8* name ) ;

	/*
	 brief : remove and release fbo
	*/
	void removeFbo ( const iS8* name ) ;

	/*
	 brief : remove and release rbo
	*/
	void removeRbo ( const iS8* name ) ;

	/*
	 brief :
	*/
	H3Shader* getShader ( const iS8* name ) ;

	/* 
	 brief :
	*/
	iU32 getFbo ( const iS8* name ) ;

	/* 
	 brief :
	*/
	iU32 getRbo ( const iS8* name ) ;

private :

	bool render2Texture ;
	bool mipmapGenerated ;

	iF32 deltaT ;
	iU32 width ;
	iU32 height ;

	H3Component* currentCamera ;
	H3Matrix4x4f currentModelMatrix ;
	H3Matrix4x4f currentProjectionMatrix ;
	H3Matrix4x4f currentMVPMatrix ;
	
	std::map < const iS8* , H3Shader* > shaderMap ;
	std::map < const iS8* , H3Texture* > textureMap ;
	std::map < const iS8* , iS32 > textureIndexMap ;
	std::map < const iS8* , iU32 > fboMap ;
	std::map < const iS8* , iU32 > rboMap ;

} ;

#endif