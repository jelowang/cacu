
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef EDITOR_H
#define EDITOR_H

#include "H3Std.h"
#include "H3Texture.h"
#include "H3Scene.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

class Editor : virtual public H3Memory
{
public :

	/*
     brief :
    */
	static iS32 draw2d3dChange ( ) ;

	/*
     brief :
    */
	static void drawMenuSelect ( iS32 index ) ;

	/*
     brief :
    */
	static void drawDetailsWindow ( iS32 index , void* ref ) ;

	/*
     brief :
    */
	static void drawObjectRotateEditor ( H3Object* object ) ;

	/*
     brief :
    */
	static void drawObjectScaleEditor ( H3Object* object ) ;

	/*
     brief :
    */
	static void drawObjectMaterialEditor( H3Object* object ) ;

	/*
     brief :
    */
	static void drawObjectMoveEditor ( H3Object* object ) ;

	/*
     brief :
    */
	static void drawObjectBlendShapeEditor ( H3Object* object ) ;

	/*
     brief :
    */
	static void drawCameraRotateEditor ( H3Camera* camera ) ;

	/*
     brief :
    */
	static void drawCameraMoveEditor ( H3Camera* camera ) ;

	/*
     brief :
    */
	static void drawCameraSetEditor ( H3Camera* camera ) ;

	/*
     brief :
    */
	static H3Texture*  generateWordsTexture ( char* text ) ;

	/*
     brief :
    */
	static void ShowExampleMenuFile() ;

	/*
     brief :
    */
	static void ShowExampleAppMainMenuBar() ;


} ;


#endif