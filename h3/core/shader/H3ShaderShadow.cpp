
/*

	+	H3ShaderShadow.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3ShaderShadow.h"

/*
 brief :
*/	
H3ShaderShadow::H3ShaderShadow () 
{
}

/*
 brief :
*/
H3ShaderShadow::~H3ShaderShadow () 
{
}

/*
 brief :
*/	
H3ShaderShadow::H3ShaderShadow ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ; 
}

/*
 brief :
*/
void H3ShaderShadow::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{
	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;
}

