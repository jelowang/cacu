
/*

	+	H3ShaderDirectionalLight.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Shader.h"
#include "H3ShaderDirectionalLight.h"

/*
 brief :
*/	
H3ShaderDirectionalLight::H3ShaderDirectionalLight () 
{
}

/*
 brief :
*/
H3ShaderDirectionalLight::~H3ShaderDirectionalLight () 
{
}

/*
 brief :
*/	
H3ShaderDirectionalLight::H3ShaderDirectionalLight ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ; 
}  

/*
 brief :
*/
void H3ShaderDirectionalLight::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{
    
	H3DirectionalLight* l = dynamic_cast<H3DirectionalLight* >(light);

	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;

    iF32 color [3] = { l->color.x , l->color.y , l->color.z } ;
    iU32 slot = h3glGetUniformLocation( this->getProgram () , "dlight.color" ) ;
    h3glUniform3fv ( slot , 1 , color ) ;

    iF32 direction [3] = { l->direction.x , l->direction.y , l->direction.z } ;
    slot = h3glGetUniformLocation( this->getProgram () , "dlight.direction" ) ;
	h3glUniform3fv ( slot , 1 , direction ) ;   

    slot = h3glGetUniformLocation( this->getProgram () , "dlight.intensity" ) ;
	h3glUniform1f ( slot , l->intensity ) ;
    
}
 