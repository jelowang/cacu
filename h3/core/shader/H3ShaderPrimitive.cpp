
/*

	+	H3ShaderPrimitive.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Shader.h"
#include "H3ShaderPrimitive.h"

/*
 brief :
*/	
H3ShaderPrimitive::H3ShaderPrimitive () 
{
}

/*
 brief :
*/
H3ShaderPrimitive::~H3ShaderPrimitive () 
{
}

/*
 brief :
*/	
H3ShaderPrimitive::H3ShaderPrimitive ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ;
}

/*
 brief :
*/
void H3ShaderPrimitive::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{
	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;
}
