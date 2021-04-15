
/*

	+	H3ShaderParticle.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Shader.h"
#include "H3ShaderParticle.h"

/*
 brief :
*/	
H3ShaderParticle::H3ShaderParticle () 
{
}

/*
 brief :
*/
H3ShaderParticle::~H3ShaderParticle () 
{
}

/*
 brief :
*/	
H3ShaderParticle::H3ShaderParticle ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ;
}

/*
 brief :
*/
void H3ShaderParticle::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{
	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;  
}
