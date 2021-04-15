
/*

	+	H3ShaderSpotLight.h
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

#ifndef H3ShaderSpotLight_H
#define H3ShaderSpotLight_H

#include "H3Shader.h"

/*
 brief :
*/
class H3ShaderSpotLight : public H3Shader , virtual public H3Memory
{
public :

	/*
	 brief :
	*/	
	H3ShaderSpotLight () ;

	/*
	 brief :
	*/
	~H3ShaderSpotLight () ;

	/*
	 brief :
	*/	
	H3ShaderSpotLight ( iS8* vShader , iS8* fShader ) ;

	/*
	 brief :
	*/
	virtual void update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;	

	
} ;

#endif