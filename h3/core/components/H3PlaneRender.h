
/*

	+	H3MeshRender.h
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

#ifndef H3PlaneRender_H
#define H3PlaneRender_H

#include "H3Std.h"
#include "H3Mesh.h"
#include "H3Material.h"
#include "H3Shader.h"
#include "H3Light.h"

class H3PlaneRender : public H3Component , virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3PlaneRender ( H3_Quad_4_QuadGroup* quad , H3Material* material ) ;

    /*
     brief :
    */
	~H3PlaneRender () ;

    /*
     brief :
    */
	virtual void render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;

	/*
     brief :
    */
	void setQuad ( H3_Quad_4_QuadGroup* quad ) ;

private :
    /*
     brief :
    */
	void init () ;

private :

	iU32 vao ;
	iU32 vbo ;
	iU32 ibo ;
	H3_Quad_4_QuadGroup quad ;
	iU32 totallQuad ;
	iU32 totallIndices ;

	H3Material* materialRef ;

} ;

#endif