
/*

	+	H3Component.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Std.h"
#include "H3gl.h"
#include "H3Texture.h"
#include "H3MeshRender.h"
#include "H3Render.h"
#include "H3DrawPrimitives.h"
#include "H3Delegate.h"

/* 
 brief :
*/
H3MeshRender::H3MeshRender ( H3Mesh* mesh ) 
{

	mesh->addRef () ;

    h3glGenVertexArrays ( 1 , &this->vao ) ;
    
	this->mesh = mesh ;
	this->type = H3COMPONENT_TYPE_MESHRENDER ;

	h3glBindVertexArray ( this->vao ) ;
	for ( iU32 looper = 0 ; looper < mesh->meshGroupList.size () ; looper ++ ) { 

		H3MeshGroup* meshGroup = mesh->meshGroupList[looper] ;
		H3MeshRenderGroup* meshRenderGroup = MMC_NEW H3MeshRenderGroup () ;

		h3glGenBuffers ( 1 , &meshRenderGroup->vboPosTexNorTan ) ;
		h3glGenBuffers ( 1 , &meshRenderGroup->ibo ) ;

		h3glBindBuffer ( GL_ARRAY_BUFFER , meshRenderGroup->vboPosTexNorTan ) ;  
		h3glBufferData ( GL_ARRAY_BUFFER , meshGroup->totallQuad * sizeof(H3_Quad_4) , meshGroup->quad , GL_DYNAMIC_DRAW ) ;
				    
		h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , meshRenderGroup->ibo ) ;
		h3glBufferData ( GL_ELEMENT_ARRAY_BUFFER , meshGroup->face.length * sizeof(iU32) , meshGroup->face.data , GL_STATIC_DRAW ) ;

		this->meshRenderGroup.push_back ( meshRenderGroup ) ;

	}
	h3glBindBuffer ( GL_ARRAY_BUFFER , 0 ) ;
    h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 ) ;
	h3glBindVertexArray (0) ;

	H3_GL_ERROR_CHECK();
}

/*
 brief :
*/
H3MeshRender::~H3MeshRender () 
{
	
	for ( iU32 looper = 0 ; looper < mesh->meshGroupList.size () ; looper ++ ) { 
		MMC_DELETE(this->meshRenderGroup[looper]) ;
	}

	h3glDeleteVertexArrays ( 1 , &this->vao ) ;

	this->mesh->release () ;

}

/*
 brief :
*/
void H3MeshRender::render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{

	h3glEnable ( GL_DEPTH_TEST ) ;

	H3Light* l = dynamic_cast<H3Light* >(light) ;
	H3Shader* renderingShader = (H3Shader* )shader;

	/*
	about gamma correction
	use alpha blend, especially the add blend, the result was brighter than blending in the SRGB space and some soft edges became sharp. 
	Because the SRGB add blend result in linear space was (SRGB0 + SRGB1*alpha)^2.2 , when change to linear blend, it became to SRGB0^2.2 + SRGB1^2.2*alpha . 
	To fix this problem, a lot of textures must been modified, it`s terrible... 
	So question is, SRGB space and linear space,which is the correct way to do alpha blending? 
	*/
	if ( H3COMPONENT_TYPE_AMBIENT_LIGHT == l->getType () ) {
		h3glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA ) ;
	} else {
		h3glBlendFunc ( GL_ONE , GL_ONE ) ;  
	}
		        
	//	shading start
    renderingShader->use () ;
	renderingShader->setUniformsForBuiltins() ;
    renderingShader->update ( l , camera , worldMatrix , projMatrix , lightProjMatrix ) ;

    iU32 slot = 0 ; 	 

	if ( ((H3Render* )render)->getTexture ( H3_MATERIAL_TEXTURE_SHADOW ) ) {
	h3glActiveTexture ( ((H3Render* )render)->getTextureIndex ( H3_MATERIAL_TEXTURE_SHADOW ) ) ;  
	h3glBindTexture ( GL_TEXTURE_2D , ((H3Render* )render)->getTexture ( H3_MATERIAL_TEXTURE_SHADOW )->getId () );
	slot = renderingShader->getUniformLocation ( H3_MATERIAL_TEXTURE_SHADOW ) ; 
	renderingShader->setUniformLocationWith1i ( slot , ((H3Render* )render)->getTextureIndexEx ( H3_MATERIAL_TEXTURE_SHADOW ) ) ;
	}
	
#if 0
	slot = renderingShader->getUniformLocation ( H3_MATERIAL_TEXTURE_DIFFUSE ) ; 
	if ( -1 != slot ) {
	h3glActiveTexture ( material->getTextureIndex ( H3_MATERIAL_TEXTURE_DIFFUSE ) ) ;
	h3glBindTexture ( GL_TEXTURE_2D , material->getTexture ( H3_MATERIAL_TEXTURE_DIFFUSE )->getId() );
	}  
	renderingShader->setUniformLocationWith1i ( slot , material->getTextureIndexEx ( H3_MATERIAL_TEXTURE_DIFFUSE ) ) ;

	slot = renderingShader->getUniformLocation ( H3_MATERIAL_TEXTURE_NORMAL ) ; 
	if ( -1 != slot ) {
	h3glActiveTexture ( material->getTextureIndex ( H3_MATERIAL_TEXTURE_NORMAL ) ) ;
	h3glBindTexture ( GL_TEXTURE_2D , material->getTexture ( H3_MATERIAL_TEXTURE_NORMAL )->getId() );
	}
	renderingShader->setUniformLocationWith1i ( slot , material->getTextureIndexEx ( H3_MATERIAL_TEXTURE_NORMAL ) ) ;

	//	material
	slot = renderingShader->getUniformLocation ( "material.shininess" ) ; 
	renderingShader->setUniformLocationWith1f ( slot , material->getFloat ( H3_MATERIAL_SHININESS ) ) ;

	slot = renderingShader->getUniformLocation ( "material.specularIntensity" ) ; 
	renderingShader->setUniformLocationWith1f ( slot , material->getFloat ( H3_MATERIAL_SPECULARINTENSITY ) ) ;

	H3Vector3f* ambient = material->getColor ( H3_MATERIAL_COLOR_AMBIENT ) ;
	iF32 color[3] = { ambient->x , ambient->y , ambient->z } ;
	slot = renderingShader->getUniformLocation ( "material.ambientColor" ) ; 
	renderingShader->setUniformLocationWith3f ( slot , color ) ;

	H3Vector3f* diffuse = material->getColor ( H3_MATERIAL_COLOR_DIFFUSE ) ;
	iF32 dcolor[3] = { diffuse->x , diffuse->y , diffuse->z } ;
	slot = renderingShader->getUniformLocation ( "material.diffuseColor" ) ; 
	renderingShader->setUniformLocationWith3f ( slot , dcolor ) ;

	H3Vector3f* specular = material->getColor ( H3_MATERIAL_COLOR_SPECULAR ) ;
	iF32 scolor[3] = { specular->x , specular->y , specular->z } ;
	slot = renderingShader->getUniformLocation ( "material.specularColor" ) ; 
	renderingShader->setUniformLocationWith3f ( slot , scolor ) ;
#endif
	//	shading finished

#ifdef WIN32
	iU32 positionLocation = 0 ;
	iU32 texCoordLocation = 1 ;
	iU32 normalLocation = 2 ;
	iU32 tangentLocation = 3 ;
#else
	iU32 positionLocation = glGetAttribLocation ( renderingShader->getProgram() , "vertexPosition") ;
	iU32 texCoordLocation = glGetAttribLocation ( renderingShader->getProgram() , "vertexTexCoord") ;
	iU32 normalLocation = glGetAttribLocation ( renderingShader->getProgram() , "vertexNormal") ;
	iU32 tangentLocation = glGetAttribLocation ( renderingShader->getProgram() , "vertexTangent") ;
#endif

	h3glBindVertexArray ( this->vao ) ;
    h3glEnableVertexAttribArray ( positionLocation ) ;
    h3glEnableVertexAttribArray ( texCoordLocation ) ;
    h3glEnableVertexAttribArray ( normalLocation ) ; 
	h3glEnableVertexAttribArray ( tangentLocation ) ;

	for ( iU32 looper = 0 ; looper < mesh->meshGroupList.size () ; looper ++ ) { 

		H3MeshGroup* meshGroup = mesh->meshGroupList[looper] ;
		H3MeshRenderGroup* meshRenderGroup = this->meshRenderGroup[looper] ;
			H3_GL_ERROR_CHECK();
		if ( meshGroup->material ) {

			//	set diffuse textures
			slot = renderingShader->getUniformLocation ( H3_MATERIAL_TEXTURE_DIFFUSE ) ; 
			if ( -1 != slot ) {
			h3glActiveTexture ( meshGroup->material->getTextureIndex ( H3_MATERIAL_TEXTURE_DIFFUSE ) ) ;
			h3glBindTexture ( GL_TEXTURE_2D , meshGroup->material->getTexture ( H3_MATERIAL_TEXTURE_DIFFUSE )->getId() );
			renderingShader->setUniformLocationWith1i ( slot , meshGroup->material->getTextureIndexEx ( H3_MATERIAL_TEXTURE_DIFFUSE ) ) ;
			}  			

			slot = renderingShader->getUniformLocation ( "material.shininess" ) ; 
			renderingShader->setUniformLocationWith1f ( slot , meshGroup->material->getFloat ( H3_MATERIAL_SHININESS ) ) ;

			slot = renderingShader->getUniformLocation ( "material.specularIntensity" ) ; 
			renderingShader->setUniformLocationWith1f ( slot , meshGroup->material->getFloat ( H3_MATERIAL_SPECULARINTENSITY ) ) ;

			slot = renderingShader->getUniformLocation ( "material.sss" ) ; 
			renderingShader->setUniformLocationWith1f ( slot , meshGroup->material->getFloat ( H3_MATERIAL_SSS ) ) ;

			slot = renderingShader->getUniformLocation ( "material.fresnel" ) ; 
			renderingShader->setUniformLocationWith1f ( slot , meshGroup->material->getFloat ( H3_MATERIAL_FRESNEL) ) ;
			  
			H3Vector3f* ambient = meshGroup->material->getColor ( H3_MATERIAL_COLOR_AMBIENT ) ;
			iF32 color[3] = { ambient->x , ambient->y , ambient->z } ;
			slot = renderingShader->getUniformLocation ( "material.ambientColor" ) ; 
			renderingShader->setUniformLocationWith3f ( slot , color ) ;

			H3Vector3f* diffuse = meshGroup->material->getColor ( H3_MATERIAL_COLOR_DIFFUSE ) ;
			iF32 dcolor[3] = { diffuse->x , diffuse->y , diffuse->z } ;
			slot = renderingShader->getUniformLocation ( "material.diffuseColor" ) ; 
			renderingShader->setUniformLocationWith3f ( slot , dcolor ) ;
			 
			H3Vector3f* specular = meshGroup->material->getColor ( H3_MATERIAL_COLOR_SPECULAR ) ;
			iF32 scolor[3] = { specular->x , specular->y , specular->z } ;
			slot = renderingShader->getUniformLocation ( "material.specularColor" ) ; 
			renderingShader->setUniformLocationWith3f ( slot , scolor ) ;
		}

		h3glBindBuffer ( GL_ARRAY_BUFFER , meshRenderGroup->vboPosTexNorTan ) ;
		h3glBufferData ( GL_ARRAY_BUFFER , meshGroup->totallQuad * sizeof(H3_Quad_4) , meshGroup->quad , GL_DYNAMIC_DRAW ) ;
		H3_GL_ERROR_CHECK();
		#define defQuadSize sizeof(H3_Quad_4)
		h3glVertexAttribPointer ( positionLocation , 3 , GL_FLOAT , false , defQuadSize , (GLvoid*) offsetof( H3_Quad_4,vertex) ) ;
		h3glVertexAttribPointer ( texCoordLocation , 2 , GL_FLOAT , false , defQuadSize , (GLvoid*) offsetof( H3_Quad_4,texCoord) ) ;
		h3glVertexAttribPointer ( normalLocation , 3 , GL_FLOAT , false , defQuadSize, (GLvoid*) offsetof( H3_Quad_4,normal) ) ;
		h3glVertexAttribPointer ( tangentLocation , 3 , GL_FLOAT , false , defQuadSize , (GLvoid*) offsetof( H3_Quad_4,tangent) ) ;
		
		h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , meshRenderGroup->ibo ) ;
		//h3glDrawElements ( GL_TRIANGLES , meshGroup->face.length , GL_UNSIGNED_INT , 0 ) ;

		if ( false == H3Delegate::getInstance()->getWireFrameEnable () ) 
			glDrawArrays ( GL_TRIANGLES , 0 , meshGroup->face.length ) ;
		else 
			glDrawArrays ( GL_LINE_STRIP , 0 , meshGroup->face.length ); 

		//	need divide 3 outside
		H3_GL_DRAW_RECORD_VERTICES(meshGroup->vertex.position_walker) ;

	}
    h3glDisableVertexAttribArray ( positionLocation ) ;
    h3glDisableVertexAttribArray ( texCoordLocation ) ;
    h3glDisableVertexAttribArray ( normalLocation ) ; 
	h3glDisableVertexAttribArray ( tangentLocation ) ;
  
    //  release
	h3glActiveTexture ( 0 ) ;
	h3glBindTexture ( GL_TEXTURE_2D , 0 ) ;
    h3glBindBuffer ( GL_ARRAY_BUFFER , 0 ) ;
    h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 ) ;
	h3glBindVertexArray ( 0 ) ;

	h3glDisable ( GL_DEPTH_TEST ) ;

	H3_GL_DRAW_RECORD_DRAWCALL(1)
	H3_GL_ERROR_CHECK();

}

/*
 brief :
*/ 
H3Mesh* H3MeshRender::getMesh () 
{
	return this->mesh ;
}