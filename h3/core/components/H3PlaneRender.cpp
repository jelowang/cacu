
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
#include "H3PlaneRender.h"
#include "H3Render.h"
#include "H3DrawPrimitives.h"
#include "H3Delegate.h"

/*
 brief :
*/
H3PlaneRender::H3PlaneRender ( H3_Quad_4_QuadGroup* quad , H3Material* material ) 
{
	 
	iU32 indices[] = {
		0 , 1 , 3 ,
		0 , 3 , 2  
	} ;  

	this->totallQuad = 4 ;
	this->totallIndices = 6 ;

	memcpy ( &this->quad , quad , sizeof(H3_Quad_4_QuadGroup) ) ;

    h3glGenVertexArrays ( 1 , &this->vao ) ;
    
	this->materialRef = material ;
	this->type = H3COMPONENT_TYPE_PLANERENDER ;

	h3glBindVertexArray ( this->vao ) ;

	h3glGenBuffers ( 1 , &this->vbo ) ;
	h3glGenBuffers ( 1 , &this->ibo ) ;

	h3glBindBuffer ( GL_ARRAY_BUFFER , this->vbo ) ;  
	h3glBufferData ( GL_ARRAY_BUFFER , sizeof(this->quad) , &this->quad , GL_DYNAMIC_DRAW ) ;
			        
	h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , this->ibo ) ; 
	h3glBufferData ( GL_ELEMENT_ARRAY_BUFFER , sizeof(indices) , indices , GL_STATIC_DRAW ) ;

	h3glBindBuffer ( GL_ARRAY_BUFFER , 0 ) ;
    h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 ) ;
	h3glBindVertexArray (0) ;    
}

/*
 brief :
*/
H3PlaneRender::~H3PlaneRender () 
{
	
	h3glDeleteBuffers ( 1 , &this->vbo ) ;
	h3glDeleteBuffers ( 1 , &this->ibo ) ; 
	h3glDeleteVertexArrays ( 1 , &this->vao ) ;

}

/*
 brief :
*/
void H3PlaneRender::render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) 
{

	h3glDisable ( GL_DEPTH_TEST ) ;

	H3ASSERT ( this->materialRef ) ;

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
		        

#ifdef TEST_ALPHA_BENDING
	iU32 src[] = { GL_ZERO , GL_ONE , GL_SRC_COLOR , GL_ONE_MINUS_SRC_COLOR , GL_SRC_ALPHA ,
	GL_ONE_MINUS_SRC_ALPHA , GL_DST_ALPHA , GL_ONE_MINUS_DST_ALPHA } ;
	iU32 des[] = { GL_ZERO , GL_ONE , GL_SRC_COLOR , GL_ONE_MINUS_SRC_COLOR , GL_SRC_ALPHA ,
	GL_ONE_MINUS_SRC_ALPHA , GL_DST_ALPHA , GL_ONE_MINUS_DST_ALPHA } ;
	int scrindex = -1 ;
	int desindex = 0 ;
	int times = 0 ;

	if ( (desindex % 8) == 0 ) {
		desindex = 0 ;
		scrindex ++ ;
		if ( scrindex == 8 ) {
			scrindex = 0 ;
		}
		h3glBlendFunc ( src[scrindex] , des[desindex] ) ;
		desindex ++ ;
	} else {
		h3glBlendFunc ( src[scrindex] , des[desindex] ) ;
	}

	times ++ ;
	if ( (times % (30*1)) == 0 ) {
		desindex ++ ;
		printf("%x,%x\n",src[scrindex],des[desindex]) ;
	}
	//glDepthMask(GL_FALSE);
	//h3glBlendFunc ( GL_ONE , GL_ONE ) ;
#endif

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
	
	slot = renderingShader->getUniformLocation ( H3_MATERIAL_TEXTURE_DIFFUSE ) ; 
	if ( -1 != slot ) {
	h3glActiveTexture ( this->materialRef->getTextureIndex ( H3_MATERIAL_TEXTURE_DIFFUSE ) ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->materialRef->getTexture ( H3_MATERIAL_TEXTURE_DIFFUSE )->getId() );
	renderingShader->setUniformLocationWith1i ( slot , this->materialRef->getTextureIndexEx ( H3_MATERIAL_TEXTURE_DIFFUSE ) ) ;
	}  	

	slot = renderingShader->getUniformLocation ( H3_MATERIAL_TEXTURE_NORMAL ) ; 
	if ( -1 != slot ) {
	h3glActiveTexture ( this->materialRef->getTextureIndex ( H3_MATERIAL_TEXTURE_NORMAL ) ) ;
	h3glBindTexture ( GL_TEXTURE_2D , this->materialRef->getTexture ( H3_MATERIAL_TEXTURE_NORMAL )->getId() );
	}
	renderingShader->setUniformLocationWith1i ( slot , this->materialRef->getTextureIndexEx ( H3_MATERIAL_TEXTURE_NORMAL ) ) ;

	//	material
	slot = renderingShader->getUniformLocation ( "material.shininess" ) ; 
	renderingShader->setUniformLocationWith1f ( slot , this->materialRef->getFloat ( H3_MATERIAL_SHININESS ) ) ;

	slot = renderingShader->getUniformLocation ( "material.specularIntensity" ) ; 
	renderingShader->setUniformLocationWith1f ( slot , this->materialRef->getFloat ( H3_MATERIAL_SPECULARINTENSITY ) ) ;

	slot = renderingShader->getUniformLocation ( "material.sss" ) ; 
	renderingShader->setUniformLocationWith1f ( slot , this->materialRef->getFloat ( H3_MATERIAL_SSS ) ) ;

	slot = renderingShader->getUniformLocation ( "material.fresnel" ) ; 
	renderingShader->setUniformLocationWith1f ( slot , this->materialRef->getFloat ( H3_MATERIAL_FRESNEL) ) ;

	H3Vector3f* ambient = this->materialRef->getColor ( H3_MATERIAL_COLOR_AMBIENT ) ;
	iF32 color[3] = { ambient->x , ambient->y , ambient->z } ;
	slot = renderingShader->getUniformLocation ( "material.ambientColor" ) ; 
	renderingShader->setUniformLocationWith3f ( slot , color ) ;

	H3Vector3f* diffuse = this->materialRef->getColor ( H3_MATERIAL_COLOR_DIFFUSE ) ;
	iF32 dcolor[3] = { diffuse->x , diffuse->y , diffuse->z } ;
	slot = renderingShader->getUniformLocation ( "material.diffuseColor" ) ; 
	renderingShader->setUniformLocationWith3f ( slot , dcolor ) ;

	H3Vector3f* specular = this->materialRef->getColor ( H3_MATERIAL_COLOR_SPECULAR ) ;
	iF32 scolor[3] = { specular->x , specular->y , specular->z } ;
	slot = renderingShader->getUniformLocation ( "material.specularColor" ) ; 
	renderingShader->setUniformLocationWith3f ( slot , scolor ) ;
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
		
	h3glBindBuffer ( GL_ARRAY_BUFFER , this->vbo ) ;
	h3glBufferData ( GL_ARRAY_BUFFER , sizeof(this->quad) , &this->quad , GL_DYNAMIC_DRAW ) ;
	   
	#define defQuadSize sizeof(H3_Quad_4)
	h3glVertexAttribPointer ( positionLocation , 3 , GL_FLOAT , false , defQuadSize , (GLvoid*) offsetof( H3_Quad_4,vertex) ) ;
	h3glVertexAttribPointer ( texCoordLocation , 2 , GL_FLOAT , false , defQuadSize , (GLvoid*) offsetof( H3_Quad_4,texCoord) ) ;
	h3glVertexAttribPointer ( normalLocation , 3 , GL_FLOAT , false , defQuadSize, (GLvoid*) offsetof( H3_Quad_4,normal) ) ;
	h3glVertexAttribPointer ( tangentLocation , 3 , GL_FLOAT , false , defQuadSize , (GLvoid*) offsetof( H3_Quad_4,tangent) ) ;
	
	h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , this->ibo ) ;
	//glDrawArrays ( GL_TRIANGLES , 0 , this->totallIndices ); 
	if ( false == H3Delegate::getInstance()->getWireFrameEnable () ) 
		glDrawElements ( GL_TRIANGLES , this->totallIndices , GL_UNSIGNED_INT , 0 ) ;
	else 
		glDrawElements ( GL_LINE_STRIP , this->totallIndices , GL_UNSIGNED_INT , 0 ) ;
	 
	//	need divide 3 outside
	H3_GL_DRAW_RECORD_VERTICES(4) ;

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

	h3glEnable ( GL_DEPTH_TEST ) ;

	H3_GL_DRAW_RECORD_DRAWCALL(1)
	H3_GL_ERROR_CHECK()

}

/*
 brief :
*/
void H3PlaneRender::setQuad ( H3_Quad_4_QuadGroup* quad ) 
{
	memcpy ( &this->quad , quad , sizeof(this->quad) ) ;
}
