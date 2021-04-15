
#include "H3Billboard.h"
#include "H3PlaneRender.h"
#include "H3Delegate.h"
#include "H3DrawPrimitives.h"

/*
 brief :
*/
H3Billboard::H3Billboard ( iF32 radius ) 
{


	this->material = MMC_NEW H3Material () ;
	this->material->addRef () ;

	H3_Quad_4_QuadGroup quad = {0} ;

	float halfRadius = radius / 2.0f ;
	
	quad.tl.vertex.x = -halfRadius ;
	quad.tl.vertex.y = halfRadius ;
	quad.tl.vertex.z = 0.0f ;

	quad.tr.vertex.x = halfRadius ;
	quad.tr.vertex.y = halfRadius ;
	quad.tr.vertex.z = 0.0f ;

	quad.bl.vertex.x = -halfRadius ;
	quad.bl.vertex.y = -halfRadius ;
	quad.bl.vertex.z = 0.0f ;

	quad.br.vertex.x = halfRadius ;
	quad.br.vertex.y = -halfRadius ;
	quad.br.vertex.z = 0.0f ;

	//texCoord
	quad.tl.texCoord.u = 1.0f ;
	quad.tl.texCoord.v = 1.0f ;

	quad.tr.texCoord.u = 0.0f ;
	quad.tr.texCoord.v = 1.0f ;

	quad.bl.texCoord.u = 1.0f ;
	quad.bl.texCoord.v = 0.0f ;

	quad.br.texCoord.u = 0.0f ;
	quad.br.texCoord.v = 0.0f ;

	this->aabb.setEmpty () ;
	this->aabb.add ( H3Vector3f(quad.tl.vertex.x*2.0,quad.tl.vertex.y*2.0,quad.tl.vertex.z*2.0) ) ;
	this->aabb.add ( H3Vector3f(quad.tr.vertex.x*2.0,quad.tr.vertex.y*2.0,quad.tr.vertex.z*2.0) ) ;
	this->aabb.add ( H3Vector3f(quad.bl.vertex.x*2.0,quad.bl.vertex.y*2.0,quad.bl.vertex.z*2.0) ) ;
	this->aabb.add ( H3Vector3f(quad.br.vertex.x*2.0,quad.br.vertex.y*2.0,quad.br.vertex.z*2.0) ) ;

	this->isBillboard = true ;
	this->radius = radius ;
	this->planeRenderRef = MMC_NEW H3PlaneRender( &quad , material ) ;
	this->addComponent( planeRenderRef ) ;  
	this->scale.init ( 1.0f , 1.0f , 1.0f ) ;

	//	skip light shading
	skipLightShader () ;

}

/*
 brief :
*/
H3Billboard::~H3Billboard () 
{
	this->material->release () ;
}

/*
 brief : 
*/
void H3Billboard::drawAABB () 
{
	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	
	H3AABB* aabb = &this->aabb ;
	H3AABB tranformed = aabb->transform ( mat ) ;
	
	H3Vector3f* aabbCorner = tranformed.getCorners () ;
	
	//	near plane
	H3DrawPrimitives::setColor ( 1.0 , 1.0 , 1.0 , 1.0 ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[0] , aabbCorner[1] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[1] , aabbCorner[2] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[2] , aabbCorner[3] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[3] , aabbCorner[0] ) ;
														   
	//	far plane											   
	H3DrawPrimitives::drawLine ( aabbCorner[4] , aabbCorner[5] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[5] , aabbCorner[6] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[6] , aabbCorner[7] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[7] , aabbCorner[4] ) ;
														   
	//	top plane											   
	H3DrawPrimitives::drawLine ( aabbCorner[1] , aabbCorner[5] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[2] , aabbCorner[6] ) ;
														   
	//	bottom plane										   
	H3DrawPrimitives::drawLine ( aabbCorner[0] , aabbCorner[4] ) ;
	H3DrawPrimitives::drawLine ( aabbCorner[3] , aabbCorner[7] ) ;
}

/*
 brief : Tests whether this bounding box intersects the specified ray.
		return mesh group id if intersected otherwise return -1
		return intersect point
*/
iS32 H3Billboard::intersect ( H3Ray ray , H3Vector3f* color , H3Vector3f& intersectPoint ) 
{

	iS32 ret = -1 ;

	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	iU32 looper = 0 ;

	H3AABB tranformed = this->aabb.transform ( mat ) ;
	H3Vector3f dir ;

	dir.x = 1.0f / ray.getD().x ;
	dir.y = 1.0f / ray.getD().y ;
	dir.z = 1.0f / ray.getD().z ;

	iF32 t1 = ( tranformed.min.x - ray.getO().x ) * dir.x ;
	iF32 t2 = ( tranformed.max.x - ray.getO().x ) * dir.x ;
	iF32 t3 = ( tranformed.min.y - ray.getO().y ) * dir.y ;
	iF32 t4 = ( tranformed.max.y - ray.getO().y ) * dir.y ;
	iF32 t5 = ( tranformed.min.z - ray.getO().z ) * dir.z ;
	iF32 t6 = ( tranformed.max.z - ray.getO().z ) * dir.z ;
												   
	iF32 tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	iF32 tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

	if ( tmax > 0.0f && tmin <= tmax ) {
		ret = 0 ;
		//	caculate intersection point
		H3Vector3f dis = ( this->aabb.min - this->aabb.max ) ;
		dis = dis / 2.0f ;
		intersectPoint = dis + this->aabb.max ; 
		intersectPoint = intersectPoint + this->getPosition() + this->h3Transform.getBasics() ;
	}

	return ret ;
}

/*
 breif : get mesh position witch MeshGroup Id
 Param : MeshGroup Id
*/
H3Vector3f H3Billboard::getMeshPosition ( iF32 id ) 
{

	H3Vector3f position ;

	for ( iU32 walker = 0 ; walker < ((H3Mesh* )this->meshRef)->meshGroupList.size () ; walker ++ ) {
		H3MeshGroup* group = ((H3Mesh* )this->meshRef)->meshGroupList[walker] ;
		if ( id == group->id ) {

			//	caculate position
			H3Vector3f dis = ( group->aabb.min - group->aabb.max ) ;
			dis = dis / 2.0f ;
			position = dis + group->aabb.max ; 
			position = position + this->getPosition() + this->h3Transform.getBasics() ;

			break ;  
		}
	}

	return position ;

}

/*
 brief :
*/
H3Material* H3Billboard::getMaterial () 
{
	return this->material ;
}

/*
 brief :
*/
void H3Billboard::setRadius ( iF32 radius ) 
{
	this->radius = radius ;
}

/*
 brief :
*/	
void H3Billboard::update ( iF32 delta ) 
{
	H3Camera* camera = (H3Camera* )H3Delegate::getInstance()->getCurrentCamera () ;

	if ( 0 == camera ) 
		return ;

	H3Vector3f F = camera->getForward () ;
	H3Vector3f R = H3Vector3f ( 0.0f , 1.0f , 0.0f ).cross(F) ;
	H3Vector3f U = R.cross ( F ) ;

	R.normalize () ;
	U.normalize () ;

	R = R.mul ( this->radius * this->scale.x ) ;
	U = U.mul ( this->radius * this->scale.y ) ;

	H3_Quad_4_QuadGroup quad = {0} ;

	H3Vector3f tl = ( U - R ) ;
	H3Vector3f tr = ( U + R ) ;
	H3Vector3f bl = (U*-1.0f) - R ;
	H3Vector3f br = R - U ;

	quad.tr.vertex.x = tl.x ;
	quad.tr.vertex.y = tl.y ;
	quad.tr.vertex.z = tl.z ;

	quad.tl.vertex.x = tr.x ;
	quad.tl.vertex.y = tr.y ;
	quad.tl.vertex.z = tr.z ;

	quad.br.vertex.x = bl.x ;
	quad.br.vertex.y = bl.y ;
	quad.br.vertex.z = bl.z ;
	 
	quad.bl.vertex.x = br.x ;
	quad.bl.vertex.y = br.y ;
	quad.bl.vertex.z = br.z ;

	//texCoord
	quad.tl.texCoord.u = 1.0f ;
	quad.tl.texCoord.v = 1.0f ;

	quad.tr.texCoord.u = 0.0f ;
	quad.tr.texCoord.v = 1.0f ;

	quad.bl.texCoord.u = 1.0f ;
	quad.bl.texCoord.v = 0.0f ;

	quad.br.texCoord.u = 0.0f ;
	quad.br.texCoord.v = 0.0f ;

	this->aabb.setEmpty () ;
	this->aabb.add ( H3Vector3f(quad.tl.vertex.x,quad.tl.vertex.y,quad.tl.vertex.z) ) ;
	this->aabb.add ( H3Vector3f(quad.tr.vertex.x,quad.tr.vertex.y,quad.tr.vertex.z) ) ;
	this->aabb.add ( H3Vector3f(quad.bl.vertex.x,quad.bl.vertex.y,quad.bl.vertex.z) ) ;
	this->aabb.add ( H3Vector3f(quad.br.vertex.x,quad.br.vertex.y,quad.br.vertex.z) ) ;

	planeRenderRef->setQuad ( &quad ) ;


}

/*
 brief :
*/
void H3Billboard::setScale ( H3Vector3f scale ) 
{
	this->scale = scale ;
}
