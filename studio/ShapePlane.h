
/*

+	ShapePlane.h
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

#ifndef ShapePlane_H
#define ShapePlane_H

#include "H3Std.h"
#include "H3MathUtil.h"
#include "H3Material.h"
#include "H3Object.h"
#include "H3MeshRender.h"
 
class ShapePlane : public H3Object , virtual public H3Memory
{
public :
	ShapePlane ( H3Vector3f tl , H3Vector3f tr , H3Vector3f bl , H3Vector3f br , H3Material* material ) {

		std::vector<iF32> vertices ;
		std::vector<iU32> indices ;

		vertices.push_back ( tl.x ) ;
		vertices.push_back ( tl.y ) ;
		vertices.push_back ( tl.z ) ;
								    
		vertices.push_back ( tr.x ) ;
		vertices.push_back ( tr.y ) ;
		vertices.push_back ( tr.z ) ;
								    
		vertices.push_back ( bl.x ) ;
		vertices.push_back ( bl.y ) ;
		vertices.push_back ( bl.z ) ;
								    
		vertices.push_back ( br.x ) ;
		vertices.push_back ( br.y ) ;
		vertices.push_back ( br.z ) ;
								    
		indices.push_back ( 1 - 1 ) ;
		indices.push_back ( 2 - 1 ) ;
		indices.push_back ( 3 - 1 ) ;
								    
		indices.push_back ( 2 - 1 ) ;
		indices.push_back ( 4 - 1 ) ;
		indices.push_back ( 3 - 1 ) ;
								    
		H3MeshRender* planeMeshRenderMaterial = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( vertices , indices , material ) ) ;
		this->addComponent ( planeMeshRenderMaterial );
		H3Mesh* mesh = this->getMesh () ;
		H3MeshGroup* meshGroup = mesh->getMeshGroup (0) ;

		meshGroup->quad[0].texCoord.u = 1.0f;
		meshGroup->quad[0].texCoord.v = 1.0f;

		meshGroup->quad[1].texCoord.u = 0.0f;
		meshGroup->quad[1].texCoord.v = 1.0f;

		meshGroup->quad[2].texCoord.u = 1.0f;
		meshGroup->quad[2].texCoord.v = 0.0f ;

		meshGroup->quad[3].texCoord.u = 0.0f ;
		meshGroup->quad[3].texCoord.v = 0.0f ;

	}
	~ShapePlane () {}
};

#endif