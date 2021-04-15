
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CacuEditorWindow_H
#define CacuEditorWindow_H

#include "H3Std.h"
#include "ImwWindow.h"
#include "ImwMenu.h"
using namespace ImWindow ;
class CacuEditorWindow : public ImwWindow , ImwMenu , virtual public H3Memory
{
public :

	/*
     brief :
    */
	CacuEditorWindow(const char* title = H3UtilGbk2Utf8(string("äÖÈ¾´°¿Ú")).data() ) ;

	/*
     brief :
    */
	~CacuEditorWindow () ;

	/*
     brief :
    */
	virtual void OnGui() ;

	/*
     brief :
    */
	virtual void OnMenu() ;

} ;


#endif
