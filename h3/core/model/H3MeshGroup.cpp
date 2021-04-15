
#include "H3MeshGroup.h"

/*
 brief :
*/
H3MeshGroup::H3MeshGroup () 
{
	memset ( &this->vertex , 0 , sizeof(this->vertex) ) ;
	memset ( &this->face , 0 , sizeof(this->face) ) ;
	this->aabb.setEmpty () ;
	this->quad = 0 ;
	this->totallQuad = 0 ;
}

/*
 brief :
*/ 
H3MeshGroup::~H3MeshGroup () 
{
	if ( this->quad )  
		MMC_FREE(this->quad) ;
	if ( this->material ) 
		this->material->release () ;
	if ( this->face.data ) 
		MMC_FREE(this->face.data) ;
	releaseVertex () ;
}

/*
 brief :
*/
#ifdef FACIAL_TEST
#include "H3FacialRender.h"
extern H3FacialRender* H3FacialRenderDeformer0 ;
#endif
void H3MeshGroup::generateQuad () 
{
	this->quad = (H3_Quad_4* ) MMC_MALLOC ( sizeof(H3_Quad_4) * this->vertex.position_walker / 3 ) ;
	H3ASSERT(this->quad) ;
	iU32 texCounter = 0 ;
	iU32 positionCounter = 0 ;
	for ( ; positionCounter < this->vertex.position_walker ; ) {
		this->quad[this->totallQuad].vertex.x = this->vertex.position_data[positionCounter] ;
		this->quad[this->totallQuad].vertex.y = this->vertex.position_data[positionCounter+1] ;
		this->quad[this->totallQuad].vertex.z = this->vertex.position_data[positionCounter+2] ;

		if ( 0 < this->vertex.texCoord_walker ) {
		this->quad[this->totallQuad].texCoord.u = this->vertex.texCoord_data[texCounter] ;
		this->quad[this->totallQuad].texCoord.v = this->vertex.texCoord_data[texCounter+1] ;
		}

		if ( 0 < this->vertex.normal_walker ) {
		this->quad[this->totallQuad].normal.x = this->vertex.normal_data[positionCounter] ;
		this->quad[this->totallQuad].normal.y = this->vertex.normal_data[positionCounter+1] ;
		this->quad[this->totallQuad].normal.z = this->vertex.normal_data[positionCounter+2] ;
		}

		if ( 0 < this->vertex.tangent_walker ) {
		this->quad[this->totallQuad].tangent.x = this->vertex.tangent_data[positionCounter] ;
		this->quad[this->totallQuad].tangent.y = this->vertex.tangent_data[positionCounter+1] ;
		this->quad[this->totallQuad].tangent.z = this->vertex.tangent_data[positionCounter+2] ;
		} else{
		this->quad[this->totallQuad].tangent.x = 1.0f ;
		this->quad[this->totallQuad].tangent.y = 1.0f ;
		this->quad[this->totallQuad].tangent.z = 1.0f ;
		}

#ifdef FACIAL_TEST
		if ( H3FacialRenderDeformer0 ){
		H3MeshGroup* mg = H3FacialRenderDeformer0->getMesh()->getMeshGroup(0) ;
		this->quad[this->totallQuad].vertexBlendShape1.x = mg->quad[this->totallQuad].vertex.x ;
		this->quad[this->totallQuad].vertexBlendShape1.y = mg->quad[this->totallQuad].vertex.y ;
		this->quad[this->totallQuad].vertexBlendShape1.z = mg->quad[this->totallQuad].vertex.z ;

		if ( 0 < this->vertex.texCoord_walker ) {
		this->quad[this->totallQuad].texCoordBlendShape1.u = mg->quad[this->totallQuad].texCoord.u ;
		this->quad[this->totallQuad].texCoordBlendShape1.v = mg->quad[this->totallQuad].texCoord.v ;
		}

		if ( 0 < this->vertex.normal_walker ) {
		this->quad[this->totallQuad].normalBlendShape1.x = mg->quad[this->totallQuad].normal.x ;
		this->quad[this->totallQuad].normalBlendShape1.y = mg->quad[this->totallQuad].normal.y ;
		this->quad[this->totallQuad].normalBlendShape1.z = mg->quad[this->totallQuad].normal.z ;
		}

		if ( 0 < this->vertex.tangent_walker ) {
		this->quad[this->totallQuad].tangentBlendShape1.x = mg->quad[this->totallQuad].tangent.x ;
		this->quad[this->totallQuad].tangentBlendShape1.y = mg->quad[this->totallQuad].tangent.y ;
		this->quad[this->totallQuad].tangentBlendShape1.z = mg->quad[this->totallQuad].tangent.z ;
		} else{
		this->quad[this->totallQuad].tangentBlendShape1.x = 1.0f ;
		this->quad[this->totallQuad].tangentBlendShape1.y = 1.0f ;
		this->quad[this->totallQuad].tangentBlendShape1.z = 1.0f ;
		}
		}
#endif



		positionCounter += 3 ;
		texCounter += 2 ;
		this->totallQuad ++ ;
	}
}

/*
 brief :
*/
void H3MeshGroup::releaseVertex () 
{
	if ( this->vertex.position_data ) {
		MMC_FREE(this->vertex.position_data) ;
		this->vertex.position_data = 0 ;
		this->vertex.position_walker = 0 ;
	}
	if ( this->vertex.texCoord_data ) {
		MMC_FREE(this->vertex.texCoord_data) ;
		this->vertex.texCoord_data = 0 ;
		this->vertex.texCoord_walker = 0 ;
	}
	if ( this->vertex.normal_data ) {
		MMC_FREE(this->vertex.normal_data) ;
		this->vertex.normal_data = 0 ;
		this->vertex.normal_walker = 0 ;
	}
	if ( this->vertex.tangent_data ) {
		MMC_FREE(this->vertex.tangent_data) ;
		this->vertex.tangent_data = 0 ;
		this->vertex.tangent_walker = 0 ;
	}
}