
/*

	+	H3Shader.h
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

#ifndef H3Shader_H
#define H3Shader_H

#include "H3Std.h"
#include "H3Vector.h"
#include "H3Matrix.h"
#include "H3Light.h"
#include "H3gl.h"
#include "H3Camera.h"

/*
 brief : 
*/
class H3Shader : virtual public H3Memory
{
public :
	enum
    {
        VERTEX_ATTRIB_POSITION ,        
        VERTEX_ATTRIB_TEXCOORD ,
        VERTEX_ATTRIB_NORMAL ,
		VERTEX_ATTRIB_TANGENT ,
        VERTEX_ATTRIB_COLOR ,        
        VERTEX_ATTRIB_MAX ,

    };

public :
	/*
     brief :
    */
	H3Shader() ;

	/*
     brief :
    */
	H3Shader( iS8* vShader , iS8* fShader ) ;

	/*
     brief :
    */
	virtual ~H3Shader() ;

	/*
	 brief :
	*/
	virtual bool init ( iS8* vShader , iS8* fShader ) ;

	/*
	 brief :
	*/
	virtual void use () ;

	/*
	 brief :
	*/
	virtual void update ( H3Component* light , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;

    /*
     brief :
    */
    iU32 getProgram () ;

    /*
     brief :
    */
	iS32 getUniformLocation ( const iS8* name ) ;

    /*
     brief :
    */
	void setUniformLocationWith1i ( iS32 location , iS32 value ) ;

    /*
     brief :
    */
	void setUniformLocationWith1f ( iS32 location , iF32 value ) ;

    /*
     brief :
    */
	void setUniformLocationWith3f ( iS32 location , iF32 value[3] ) ;

    /*
     brief :
    */
	void setUniformLocationWithMatrix4x4f ( iS32 location , H3Matrix4x4f matrix4x4f ) ;

	/*
	 brief : set view projection matrix
	*/
	void setUniformsForBuiltins () ;

#ifdef H3_QT_5_4_2
public :
	QOpenGLShaderProgram QOpenGLShader ;
#endif
    
    iU32 shaderProgram ;

} ;

#endif