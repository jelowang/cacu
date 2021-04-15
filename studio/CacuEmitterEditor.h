
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CacuEmitterEditor_H
#define CacuEmitterEditor_H

#include "HpParticle.h"
#include "HpEmitter.h"
#include "HpParticleSystem.h"
#include "CacuEditorWindow.h"

#if 0
namespace CacuEmitterEditor {

	void init () ;
	void clear () ;
	void drawMainMenu () ;
	HpParticleSystem* getCurrentSystem () ;
	HpEmitter* getCurrentEmitter () ;
	void* getEmitterList () ;
	void drawEmitterEditor ( CacuEditorWindow* window ) ;

} ;
#endif

#include "H3Std.h"
#include "ImwWindow.h"
#include "ImwMenu.h"
using namespace ImWindow ;
class CacuEmitterEditor : public ImwWindow , ImwMenu , virtual public H3Memory
{
public :

	/*
     brief :
    */
	CacuEmitterEditor(const char* title = H3UtilGbk2Utf8(string("≈Á…‰∆˜")).data() ) ;

	/*
     brief :
    */
	~CacuEmitterEditor () ;

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