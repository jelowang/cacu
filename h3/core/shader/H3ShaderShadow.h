
/*

	+	H3ShaderShadow.h
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

#ifndef H3ShaderShadow_H
#define H3ShaderShadow_H

#include "H3Shader.h"

/*
 brief :
*/
class H3ShaderShadow : public H3Shader , virtual public H3Memory
{
public :

	/*
	 brief :
	*/	
	H3ShaderShadow () ;

	/*
	 brief :
	*/
	~H3ShaderShadow () ;

	/*
	 brief :
	*/	
	H3ShaderShadow ( iS8* vShader , iS8* fShader ) ;

	/*
	 brief :
	*/
	virtual void update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;	

	
} ;

#endif