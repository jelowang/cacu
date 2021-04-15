
/*

	+	H3Shader.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3gl.h"
#include "h3Shader.h"
#include "h3Render.h"
#include "H3Material.h"
#include "H3Delegate.h"

/*
 brief :
*/
H3Shader::H3Shader() 
{
    this->shaderProgram = 0 ;
}

/*
 brief :
*/
H3Shader::H3Shader( iS8* vShader , iS8* fShader ) 
{
	init ( vShader , fShader ) ;
}

/*
 brief :
*/
H3Shader::~H3Shader() 
{
}

/* 
 brief :
*/
bool H3Shader::init ( iS8* vShader , iS8* fShader ) 
{

#ifdef H3_QT_5_4_2
	//	Compile vertex shader
    if (!this->QOpenGLShader.addShaderFromSourceFile(
		QOpenGLShader::Vertex, SCHalGetFilePath(vShader) ) ) {
		iLog( "WR : Unable to compile vertex shader , %s" , this->QOpenGLShader.log().data() ) ;
		return false ;
	}

    //	Compile fragment shader
    if (!this->QOpenGLShader.addShaderFromSourceFile(
		QOpenGLShader::Fragment, SCHalGetFilePath(fShader) ) ) {
		iLog( "WR : Unable to compile fragment shader , %s" , this->QOpenGLShader.log().data() ) ;
		return false ;
	}

    //	Link the shaders together into a program
    if (!this->QOpenGLShader.link()) {
		iLog( "WR : Unable to link shader program , %s" , this->QOpenGLShader.log().data() ) ;
		return false ;
	}
#endif
    
#if defined(H3_GLWF) || defined(H3_ANDROID) || defined(H3_IOS)
	iS8* openglVersion = (iS8* )h3glGetString( GL_SHADING_LANGUAGE_VERSION ) ;
	iS8 version[64] = {0} ;
	iS8 define[256] = {0} ;
#ifdef WIN32
	strcpy ( version , "#version 330 core\r\n" ) ;
	strcpy ( define , "#define OPENGL3\r\n" ) ;
	iLog ("Use Glsl 330 core , and OPENGL3") ;
#else  

	if ( 0 == strstr( "3" , openglVersion ) ) {
		strcpy ( version , "#version 300 es\r\n" ) ;
		strcpy ( define , "#define OPENGL3\r\n" ) ;
		iLog ("Use Glsl 300 es , and OPENGL3") ; 
	} else {    
		strcpy ( version , "#version 100\r\n" ) ;
		strcpy ( define , "#define OPENGLES2\r\n" ) ;
		iLog ("Use Glsl 100 , and OPENGLES2") ;
	}

#endif

	#ifdef FACIAL_TEST
		strcpy ( define , strcat ( define , "#define FACIAL\r\n" ) ) ;
	#endif

    void* vsFileHandle = SCHalFileOpen ( vShader , "rb" ) ;
	void* fsFileHandle = SCHalFileOpen ( fShader , "rb" ) ;

	if ( 0 == vsFileHandle ) iLog ("%s not exsits",vShader) ;

	if ( 0 == fsFileHandle ) iLog ("%s not exsits",fShader) ;
	 
	H3ASSERT ( vsFileHandle && fsFileHandle ) ;

    iS32 vsFileLength = 0 ;
    iS32 fsFileLength = 0 ;
    iS8* vsFileSource = 0 ;
    iS8* fsFileSource = 0 ;
    
#if 1
    vsFileLength = SCHalFileLength ( vsFileHandle ) + strlen(version) + strlen(define) ;
    fsFileLength = SCHalFileLength ( fsFileHandle ) + strlen(version) + strlen(define) ;

    vsFileSource = (iS8* ) MMC_MALLOC( vsFileLength * sizeof(iS8) ) ;
    fsFileSource = (iS8* ) MMC_MALLOC( fsFileLength * sizeof(iS8) ) ;
 
    H3ASSERT ( vsFileSource && fsFileSource ) ;
    
	strcpy ( vsFileSource , version ) ;
	strcpy ( vsFileSource + strlen(version), define ) ;

	strcpy ( fsFileSource , version ) ;
	strcpy ( fsFileSource + strlen(version), define ) ;

    SCHalFileRead ( vsFileHandle , vsFileSource + strlen(version) + strlen(define) , vsFileLength ) ;
    SCHalFileRead ( fsFileHandle , fsFileSource + strlen(version) + strlen(define) , fsFileLength ) ;
#else
    vsFileLength = SCHalFileLength ( vsFileHandle ) ;
    fsFileLength = SCHalFileLength ( fsFileHandle ) ;

    vsFileSource = (iS8* ) MMC_MALLOC( vsFileLength * sizeof(iS8) ) ;
    fsFileSource = (iS8* ) MMC_MALLOC( fsFileLength * sizeof(iS8) ) ;
 
    H3ASSERT ( vsFileSource && fsFileSource ) ;

    SCHalFileRead ( vsFileHandle , vsFileSource , vsFileLength ) ;
    SCHalFileRead ( fsFileHandle , fsFileSource , fsFileLength ) ;
#endif 

	iU32 vsShaderHandle = glCreateShader ( GL_VERTEX_SHADER ) ;
    iU32 fsShaderHandle = glCreateShader ( GL_FRAGMENT_SHADER ) ;

    glShaderSource ( vsShaderHandle , 1 , (const GLchar **)&vsFileSource , &vsFileLength ) ;
    glShaderSource ( fsShaderHandle , 1 , (const GLchar **)&fsFileSource , &fsFileLength ) ;

	MMC_FREE ( vsFileSource ) ;
	MMC_FREE ( fsFileSource ) ;

    iS32 compileSuccess = 0 ;
    glCompileShader ( vsShaderHandle ) ;
    glGetShaderiv ( vsShaderHandle , GL_COMPILE_STATUS , &compileSuccess ) ;
    if ( GL_FALSE == compileSuccess ) {
        iS8 logs[256] = { 0 } ;
        glGetShaderInfoLog ( vsShaderHandle , sizeof(logs) , 0 , &logs[0] ) ;
        iLog ( "FE : %s %s" , vShader , logs ) ;
        //exit(1) ;
    }

    glCompileShader ( fsShaderHandle ) ;
    glGetShaderiv ( fsShaderHandle , GL_COMPILE_STATUS , &compileSuccess ) ;
    if ( GL_FALSE == compileSuccess ) {
        iS8 logs[256] = { 0 } ;
        glGetShaderInfoLog ( fsShaderHandle , sizeof(logs) , 0 , &logs[0] ) ;
		iLog ( "FE : %s %s" , fShader , logs ) ;
        //exit(1) ;
    }
  
    this->shaderProgram = glCreateProgram () ;
  
    glAttachShader ( this->shaderProgram , vsShaderHandle ) ;
    glAttachShader ( this->shaderProgram , fsShaderHandle ) ;
    glLinkProgram ( this->shaderProgram ) ;

    iS32 linkSuccecss = 0 ;
    glGetProgramiv ( this->shaderProgram , GL_LINK_STATUS , &linkSuccecss ) ;
    if ( GL_FALSE == linkSuccecss ) {
        iS8 logs[256] = { 0 } ;
		iS8 shader[64] = {0} ;
		strcpy ( shader , vShader ) ;
		shader[strlen(vShader)-3] = '\0' ;
        glGetShaderInfoLog ( fsShaderHandle , sizeof(logs) , 0 , &logs[0] ) ;
		iLog ( "FE : %s link failed %s" , shader , logs ) ;
        //exit(1) ;
	}
	  
#endif
    
	return true ;

}

/*
 brief :
*/
void H3Shader::use () 
{
#ifdef H3_QT_5_4_2
	this->QOpenGLShader.bind () ;
#else
	glUseProgram ( this->shaderProgram ) ;
#endif
}

/*
 brief :
*/
void H3Shader::update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{

	H3Camera* mainCamera = dynamic_cast <H3Camera* >(camera) ; 

#ifdef H3_QT_5_4_2
	QMatrix4x4 worldMat4 ;
	QMatrix4x4 projMat4 ;
	QMatrix4x4 lightProjMat4 ;
	QVector3D eyePos ;

	//	set worldMat4
	for (int r = 0 ; r < 4 ; r ++ ) {
		QVector4D vec4 ;
		vec4.setX(worldMatrix->m[r][0]) ;
		vec4.setY(worldMatrix->m[r][1]) ;
		vec4.setZ(worldMatrix->m[r][2]) ;
		vec4.setW(worldMatrix->m[r][3]) ;
		worldMat4.setRow ( r , vec4 ) ;
	}   

	//	set projMat4
	for (int r = 0 ; r < 4 ; r ++ ) {
		QVector4D vec4 ;
		vec4.setX(projMatrix->m[r][0]) ;
		vec4.setY(projMatrix->m[r][1]) ;
		vec4.setZ(projMatrix->m[r][2]) ;
		vec4.setW(projMatrix->m[r][3]) ;
		projMat4.setRow ( r , vec4 ) ;
	}   

	//	set lightProjMat4
	for (int r = 0 ; r < 4 ; r ++ ) {
		QVector4D vec4 ;
		vec4.setX(lightProjMatrix->m[r][0]) ;
		vec4.setY(lightProjMatrix->m[r][1]) ;
		vec4.setZ(lightProjMatrix->m[r][2]) ;
		vec4.setW(lightProjMatrix->m[r][3]) ;
		lightProjMat4.setRow ( r , vec4 ) ;
	}   

	eyePos.setX ( mainCamera->getPosition().x ) ;
	eyePos.setY ( mainCamera->getPosition().y ) ;
	eyePos.setZ ( mainCamera->getPosition().z ) ;

	this->QOpenGLShader.setUniformValue ( (const char* )"worldMatrix" , worldMat4 ) ;
	this->QOpenGLShader.setUniformValue ( (const char* )"viewProjMatrix" , projMat4 ) ;
	this->QOpenGLShader.setUniformValue ( (const char* )"lightProjMatrix" , lightProjMat4 ) ;
	this->QOpenGLShader.setUniformValue ( (const char* )"eye" , eyePos ) ;

#else
    
    float wm[16] ;
    float pm[16] ;
	float lpm[16] ;
    int c = 0 ;

    for( int i = 0 ; i < 4 ; i ++ ) {
        for ( int j = 0 ; j < 4 ; j ++ ) {
            wm[c] = worldMatrix->m[j][i] ;
            c ++ ;
        }
        
    }
    
    c = 0 ;
    for( int i = 0 ; i < 4 ; i ++ ) {
        for ( int j = 0 ; j < 4 ; j ++ ) {
            pm[c] = projMatrix->m[j][i] ;
            c ++ ;
        }
        
    }

    c = 0 ;
    for( int i = 0 ; i < 4 ; i ++ ) {
        for ( int j = 0 ; j < 4 ; j ++ ) {
            lpm[c] = lightProjMatrix->m[j][i] ;
            c ++ ;
        }
        
    }
    
    iU32 slot = glGetUniformLocation( this->shaderProgram , "worldMatrix" ) ;
    glUniformMatrix4fv( slot , 1 , 0 , (const float* )wm ) ;
    
    slot = glGetUniformLocation( this->shaderProgram , "lightProjMatrix" ) ;
    glUniformMatrix4fv( slot , 1 , 0 , (const float* )lpm ) ;

    iF32 eyePos [3] = { mainCamera->getPosition().x , mainCamera->getPosition().y , mainCamera->getPosition().z } ;
    slot = glGetUniformLocation( this->shaderProgram , "eye" ) ;
    glUniform3fv ( slot , 1 , eyePos ) ;
#endif

}

/*
 brief :
 */
iU32 H3Shader::getProgram ()
{
#ifdef H3_QT_5_4_2
	return this->QOpenGLShader.programId() ;
#else
	return this->shaderProgram ;
#endif
}

/*
 brief :
*/
iS32 H3Shader::getUniformLocation ( const iS8* name ) 
{
	iS32 location = h3glGetUniformLocation ( this->shaderProgram , name ) ;
	return location ;
}

/*
 brief :
*/
void H3Shader::setUniformLocationWith1i ( iS32 location , iS32 value ) 
{
	if ( -1 == location )
		return ;
	h3glUniform1i ( location , value ) ;
}

/*
 brief :
*/
void H3Shader::setUniformLocationWith1f ( iS32 location , iF32 value ) 
{
	if ( -1 == location )
		return ;
	h3glUniform1f ( location , value ) ;
}

/*
 brief :
*/
void H3Shader::setUniformLocationWith3f ( iS32 location , iF32 value[3] ) 
{
	if ( -1 == location )
		return ;
	
	glUniform3fv ( location , 1 , value ) ;
}

/*
 brief :
*/
void H3Shader::setUniformLocationWithMatrix4x4f ( iS32 location , H3Matrix4x4f matrix4x4f ) 
{

	if ( -1 == location )
		return ;

    float wm[16] ;
    int c = 0 ;

    for( int i = 0 ; i < 4 ; i ++ ) {
        for ( int j = 0 ; j < 4 ; j ++ ) {
            wm[c] = matrix4x4f.m[j][i] ;
            c ++ ;
        }
        
    }

	glUniformMatrix4fv( location , 1 , 0 , (const float* )wm ) ;
    
}

/*
 brief : set view projection matrix
*/
void H3Shader::setUniformsForBuiltins () 
{
	H3Matrix4x4f matrix = H3Delegate::getInstance()->getMatrix ( H3MATRIX_TYPE::H3_MATRIX_VIEW_PROJECTION ) ;
	iU32 slot = getUniformLocation( "viewProjMatrix" ) ;
	setUniformLocationWithMatrix4x4f ( slot , matrix ) ;
}