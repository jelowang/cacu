
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

#ifndef H3MeshRender_H
#define H3MeshRender_H

#include "H3Std.h"
#include "H3Mesh.h"
#include "H3Material.h"
#include "H3Shader.h"
#include "H3Light.h"

class H3MeshRenderGroup : virtual public H3Memory
{
public :
	H3MeshRenderGroup () {
		vboPosTexNorTan = 0 ;
		ibo = 0 ;
	}
	~H3MeshRenderGroup () {
		h3glDeleteBuffers ( 1 , &this->vboPosTexNorTan ) ;
		h3glDeleteBuffers ( 1 , &this->ibo ) ;
	}

public :
	iU32 vboPosTexNorTan ;
	iU32 ibo ;

} ;

class H3MeshRender : public H3Component , virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3MeshRender ( H3Mesh* mesh ) ;

    /*
     brief :
    */
	virtual ~H3MeshRender () ;

    /*
     brief :
    */
	virtual void render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;

	/*
	 brief :
	*/ 
	H3Mesh* getMesh () ;

private :
    /*
     brief :
    */
	void init () ;

public :

	iU32 vao ;
	H3Mesh* mesh ;	
	vector<H3MeshRenderGroup* > meshRenderGroup ;

} ;

#endif