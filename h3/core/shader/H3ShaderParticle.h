
/*

	+	H3ShaderParticle.h
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

#include "H3Shader.h"

#ifndef H3ShaderParticle_H
#define H3ShaderParticle_H

/*
 brief :
*/
class H3ShaderParticle : public H3Shader , virtual public H3Memory
{
public :

	/*
	 brief :
	*/	
	H3ShaderParticle () ;

	/*
	 brief :
	*/
	~H3ShaderParticle () ;

	/*
	 brief :
	*/	
	H3ShaderParticle ( iS8* vShader , iS8* fShader ) ;

	/*
	 brief :
	*/
	virtual void update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;	
	
} ;

#endif