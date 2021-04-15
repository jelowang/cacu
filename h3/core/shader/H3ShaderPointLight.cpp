
/*

	+	H3ShaderPointLight.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Shader.h"
#include "H3ShaderPointLight.h"

/*
 brief :
*/	
H3ShaderPointLight::H3ShaderPointLight () 
{
}

/*
 brief :
*/
H3ShaderPointLight::~H3ShaderPointLight () 
{
}

/*
 brief :
*/	
H3ShaderPointLight::H3ShaderPointLight ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ; 
}

/*
 brief :
*/
void H3ShaderPointLight::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix )
{
	H3PointLight* l = dynamic_cast<H3PointLight* >(light);

	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;

    iF32 color [3] = { l->color.x , l->color.y , l->color.z } ;
    iU32 slot = h3glGetUniformLocation( this->getProgram () , "plight.color" ) ;
    h3glUniform3fv ( slot , 1 , color ) ;

	iF32 position [3] = { l->getPosition().x , l->getPosition().y , l->getPosition().z } ;
    slot = h3glGetUniformLocation( this->getProgram () , "plight.position" ) ;
	h3glUniform3fv ( slot , 1 , position ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "plight.constant" ) ;
	h3glUniform1f ( slot , l->constant ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "plight.linear" ) ;
	h3glUniform1f ( slot , l->linear ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "plight.quadratic" ) ;
	h3glUniform1f ( slot , l->quadratic ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "plight.intensity" ) ;
	h3glUniform1f ( slot , l->intensity ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "plight.range" ) ;
	h3glUniform1f ( slot , l->range ) ;

}
