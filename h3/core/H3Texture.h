
/*

	+	H3Texture.h
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

#ifndef H3Texture_H
#define H3Texture_H

#include "H3Std.h"

enum class H3TEXTURE_TYPE
{
	H3_TEXTURE_PIXEL ,
	//	for shadow map
	H3_TEXTURE_SHADOW ,
	//	for render to texture
	H3_TEXTURE_RENDER ,
} ;

class H3Texture : virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3Texture() ;

    /*
     brief :
    */
	~H3Texture() ;

	static H3Texture* create ( H3TEXTURE_TYPE type , iU32 width , iU32 height ) ;
	static H3Texture* create ( H3TEXTURE_TYPE type , iS8* path ) ;
	static H3Texture* create ( H3TEXTURE_TYPE type , iU32 width , iU32 height , void* data , iU32 rgbType ) ;

	/*
	 brief :
	*/
	iU32 getWidth () ;

	/*
	 brief :
	*/
	iU32 getHeight () ;

	/*
	 brief :
	*/
	void bind ( iU32 index ) ;

	/*
	 brief :
	*/
	void unbind ( iU32 index ) ;

	/*
	 brief :
	*/
	iU32 getId () ;

	/*
	 brief :
	*/
	void setId ( iU32 textureId ) ;

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
	bool initPixelTexture ( iS8* path ) ;

	/*
     brief :
    */
	bool initPixelTexture ( iU32 width , iU32 height , void* data , iU32 rgbType ) ;

	/*
     brief :
    */
	bool initPixelTexture ( iU32 width , iU32 height ) ;

	/*
     brief :
    */
	bool initShadowTexture ( iU32 width , iU32 height ) ;

	/*
     brief :
    */
	bool initRenderTexture ( iU32 width , iU32 height ) ;

private :
	iS32 refCount ;
	iU32 textureId ;
	iU32 width ;
	iU32 height ;  

} ;

#endif