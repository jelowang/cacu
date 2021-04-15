#ifdef FACIAL_TEST
/*

	+	H3FacialRender.h
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

#ifndef H3FacialRender_H
#define H3FacialRender_H

#include "H3Std.h"
#include "H3Mesh.h"
#include "H3Material.h"
#include "H3Shader.h"
#include "H3Light.h"
#include "H3MeshRender.h"

class H3FacialRender : public H3MeshRender , virtual public H3Memory
{
public :
    /*
     brief :
    */
	H3FacialRender ( H3Mesh* mesh ) ;

    /*
     brief :
    */
	~H3FacialRender () ;

    /*
     brief :
    */
	virtual void render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) ;

} ;

#endif
#endif