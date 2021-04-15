
/*

	+	H3ShaderSpotLight.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Shader.h"
#include "H3ShaderSpotLight.h"

/*
 brief :
*/	
H3ShaderSpotLight::H3ShaderSpotLight () 
{
}

/*
 brief :
*/
H3ShaderSpotLight::~H3ShaderSpotLight () 
{
}

/*
 brief :
*/	
H3ShaderSpotLight::H3ShaderSpotLight ( iS8* vShader , iS8* fShader ) 
{
	H3Shader::init ( vShader , fShader ) ; 
}

/*
 brief :
*/
void H3ShaderSpotLight::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix )
{
	H3SpotLight* l = dynamic_cast<H3SpotLight* >(light);

	H3Shader::update( light , camera , worldMatrix , projMatrix , lightProjMatrix ) ;

    iF32 color [3] = { l->color.x , l->color.y , l->color.z } ;
    iU32 slot = h3glGetUniformLocation( this->getProgram () , "slight.color" ) ;
    h3glUniform3fv ( slot , 1 , color ) ;

	iF32 position [3] = { camera->getPosition().x , camera->getPosition().y , camera->getPosition().z } ;
    slot = h3glGetUniformLocation( this->getProgram () , "slight.position" ) ;
	h3glUniform3fv ( slot , 1 , position ) ;

	H3Camera* mainCamera = dynamic_cast <H3Camera* >(camera) ; 
	H3Vector3f dir = mainCamera->getForward () ;

	iF32 direction [3] = { dir.x , dir.y , dir.z } ;
    slot = h3glGetUniformLocation( this->getProgram () , "slight.direction" ) ;
	h3glUniform3fv ( slot , 1 , direction ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "slight.intensity" ) ;
	h3glUniform1f ( slot , l->intensity ) ;

    slot = h3glGetUniformLocation( this->getProgram () , "slight.cutOff" ) ;
	h3glUniform1f ( slot , H3cosf ( H3ToRadian ( l->cutOff ) ) ) ;

}
