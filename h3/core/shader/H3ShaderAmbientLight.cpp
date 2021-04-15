
/*

	+	H3ShaderAmbientLight.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Shader.h"
#include "H3ShaderAmbientLight.h"

/*
 brief :
*/	
H3ShaderAmbientLight::H3ShaderAmbientLight () 
{
}

/*
 brief :
*/
H3ShaderAmbientLight::~H3ShaderAmbientLight () 
{
}

/*
 brief :
*/	
H3ShaderAmbientLight::H3ShaderAmbientLight ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ;
}

/*
 brief :
*/
void H3ShaderAmbientLight::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{
    
    H3AmbientLight* l = dynamic_cast<H3AmbientLight* >(light);
    
	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;

    iF32 color [3] = { l->color.x , l->color.y , l->color.z } ;
    iU32 slot = h3glGetUniformLocation( this->getProgram () , "ambient.color" ) ;
    h3glUniform3fv ( slot , 1 , color ) ;

	static float dt = 0 ;
    slot = h3glGetUniformLocation( this->getProgram () , "time" ) ;
    h3glUniform1f ( slot , dt ) ;
	dt = dt + 0.016 ;
    
}
