
/*

	+	H3Object.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Object.h"
#include "H3Camera.h"
#include "H3Light.h"
#include "H3MeshRender.h"
#include "H3DrawPrimitives.h"

/*
 brief :
*/
H3Object::H3Object() 
{
	SCClListInit ( &this->childs ) ;	
	SCClListInit ( &this->components ) ;
	SCClListInit ( &this->lightComponents ) ;
	SCClListInit ( &this->orignalAABBRefList ) ;

	this->visible = true ;
	this->isBillboard = false ;
	this->isSkipLightShader = false ;
	this->parent = 0 ;  
	this->meshRef = 0 ;
	this->basicsAlignEnable = false ;

}

/*
 brief :
*/
H3Object::~H3Object () 
{
	//	just clear link-list , leave the elements memory to memory-auto-release system
	H3ASSERT ( 1 == SCClListEmpty ( &this->childs) ) ;	
	H3ASSERT ( 1 == SCClListEmpty ( &this->components) ) ;	
	H3ASSERT ( 1 == SCClListEmpty ( &this->lightComponents) ) ;
	//	clear aabb reference
	SCClListClear ( &this->orignalAABBRefList ) ;
}
 
/*
 brief :
*/
void H3Object::update ( iF32 deltaT ) 
{

	//	force the basics to the center in local space 
	if ( true == this->basicsAlignEnable ) {
		iF32 ratio = 0.5f ;
		H3Mesh* mesh = this->getMesh ( );
		this->h3Transform.setBasics (
			H3Vector3f (
			-(fabs ( mesh->aabb.max.x ) - fabs ( mesh->aabb.min.x )) * ratio ,
			-(fabs ( mesh->aabb.max.y ) - fabs ( mesh->aabb.min.y )) * ratio ,
			-(fabs ( mesh->aabb.max.z ) - fabs ( mesh->aabb.min.z )) * ratio
			)
			);
	}

	for ( SCClList* walker = this->childs.next ; walker ; walker = walker->next ) {
		H3Object* child = (H3Object* )walker->element ;
		child->update ( deltaT ) ;
	}

	for ( SCClList* walker = this->components.next ; walker ; walker = walker->next ) {
		H3Component* component = (H3Component* )walker->element ;
		component->update ( deltaT ) ;
	}

}

/*
 brief :
*/
void H3Object::render ( void* render , H3Light* light , void* shader , H3Component* camera , H3Matrix4x4f* lightProjMatrix ) 
{

	H3Camera* mainCamera = dynamic_cast <H3Camera* >(camera) ; 
	H3Matrix4x4f projMatrix = mainCamera->getViewProjection () ;
	H3Matrix4x4f worldMat4 = this->finishTransformation ( this->isBillboard ) ;

	if ( true == this->isSkipLightShader ) {
		if ( H3COMPONENT_TYPE_DIRECTIONAL_LIGHT == light->getType () ) {
			return ;
		} else {
			static H3AmbientLight ambientLight (H3Vector3f(1.0f,1.0f,1.0f)) ;
			light = &ambientLight ;
		}
	}   

	//	forward rendering
	for ( SCClList* walker = this->childs.next ; walker ; walker = walker->next ) {
		H3Object* child = (H3Object* )walker->element ;
		if ( true == child->getVisible () ) {
			
			child->render ( render , light , shader , camera , lightProjMatrix ) ;
			child->drawAABB () ;

		}
	}

	for ( SCClList* walker = this->components.next ; walker ; walker = walker->next ) {
		H3Component* component = (H3Component* )walker->element ;
		component->render ( render , light , shader , camera , &worldMat4 , &projMatrix , lightProjMatrix ) ;
	}


}

/*
 brief :
*/
void H3Object::addChild ( H3Object* object ) 
{
	//	not alowed multi-add for object

	H3ASSERT( false == object->isUse () ) ;
	object->use ();	
	object->setParent ( this ) ;
	SCClListInsert ( &this->childs , object ) ;	
}

/*
 brief :
*/

void H3Object::addComponent ( H3Component* component )
{
	if ( H3COMPONENT_TYPE_LIGHT <= component->getType () && 
		component->getType () <= H3COMPONENT_TYPE_DIRECTIONAL_LIGHT ) {
		component->addRef () ;
		SCClListInsert ( &this->lightComponents , component ) ;
		return ;
	}

	component->addRef () ;
	SCClListInsert ( &this->components , component ) ; 
	    
	if ( H3COMPONENT_TYPE_MESHRENDER == component->getType () ) {
		H3Mesh* mesh = ((H3MeshRender* )component)->getMesh () ;
		this->meshRef = mesh ;
		for ( iU32 looper = 0 ; looper < mesh->meshGroupList.size() ; looper ++ ) {
			//	just save reference here
			SCClListInsert ( &this->orignalAABBRefList , &mesh->meshGroupList[looper]->aabb ) ;
		}
	}

	return ;
	 
}

/*
 brief :
*/
H3Vector3f H3Object::getPosition () 
{
	return this->h3Transform.getPosition () ;
}

/*
 brief :
*/
void H3Object::setPosition ( H3Vector3f position ) 
{
	this->h3Transform.setPosition ( position ) ;
}

/*
 brief :
*/
void H3Object::setScale ( H3Vector3f scale ) 
{
	this->h3Transform.setScale ( scale ) ;
}

/*
 brief :
*/
void H3Object::setParent ( H3Object* parent ) 
{
	this->parent = parent;
}

/*
 brief :
*/
void H3Object::rotate ( H3Vector3f axis , iF32 amout ) 
{

	H3Quaternion Q ( axis , amout ) ;

	this->h3Transform.rotate ( Q ) ;
}

/*
 brief :
*/
void H3Object::rotateAroundLocal ( H3Vector3f aixs , H3Vector3f point , iF32 amout ) 
{

	H3Mesh* mesh = 0 ;

	mesh = (H3Mesh* )this->meshRef ;

	point.x = -point.x * (fabs(mesh->aabb.max.x)-fabs(mesh->aabb.min.x)) / 2.0f ;
	point.y = -point.y * (fabs(mesh->aabb.max.y)-fabs(mesh->aabb.min.y)) / 2.0f ;
	point.z = -point.z * (fabs(mesh->aabb.max.z)-fabs(mesh->aabb.min.z)) / 2.0f ;

	H3Vector3f basics ( 
		-(fabs(mesh->aabb.max.x)-fabs(mesh->aabb.min.x)) / 2.0f ,
		-(fabs(mesh->aabb.max.y)-fabs(mesh->aabb.min.y)) / 2.0f ,
		-(fabs(mesh->aabb.max.z)-fabs(mesh->aabb.min.z)) / 2.0f
	) ; 

	basics = basics + point ;

	if ( false == this->h3Transform.getBasics().equal ( basics ) ) {
		//	update the translate
		H3Vector3f position = this->h3Transform.getPosition () ;
		position = position - point ;
		this->h3Transform.setPosition ( position ) ;
	}
	this->h3Transform.setBasics ( basics ) ;

	H3Quaternion Q ( aixs , amout ) ;

	this->h3Transform.rotate ( Q ) ;

}

/*
 brief :
*/
void H3Object::rotateAroundGlobal ( H3Vector3f aixs , H3Vector3f point , iF32 amout ) 
{
}

/*
 brief :
*/
H3Transform& H3Object::getTransform () 
{
	return this->h3Transform ;
}

/*
 brief : release object and components memory here
*/
bool H3Object::release () 
{

	deUse ();

	if ( false == isUse () ) {
		this->removeAllChildAndComponent () ;
		MMC_DELETE(this) ;
	}

	return false ;

}

/*
 brief : remove all childs and components
*/
bool H3Object::removeAllAndReleseSlef () 
{
	this->release () ;
	return true ;
}

/*
 brief :
*/
bool H3Object::removeAll () 
{
	this->removeAllChildAndComponent () ;
	return true ;
}

/*
 brief : rotate , scale , transform
*/
H3Matrix4x4f H3Object::finishTransformation ( bool skipRotate ) 
{

	H3Matrix4x4f mat ;
	H3Matrix4x4f parentMat4 ;

	if ( NULL != this->parent ) {
		parentMat4 = this->parent->finishTransformation ( skipRotate ) ;
	}

	mat = this->h3Transform.finishTransformation ( skipRotate ) ;
	mat = parentMat4 * mat ;

	return mat ;

}

/*
 brief : remove childs and release its resources
*/
void H3Object::removeChild ( H3Object* object ) 
{
	object->release () ;
	SCClListDelete ( &this->childs , object ) ;
}

/*
 brief : remove from parent and release its resources
*/
void H3Object::removeFromParent () 
{
	//	only remove if parent is exists
	H3ASSERT(this->parent) ;
	this->parent->removeChild ( this ) ;
}

/*
 brief : set visible on screen
*/
void H3Object::setVisible ( bool visible )  
{
	this->visible = visible ;
}

/*
 brief : 
*/
bool H3Object::getVisible () 
{
	return this->visible ;
}

/*
 brief : 
*/
void H3Object::drawAABB () 
{

#if 1

	if ( 0 == this->meshRef )  
		return ;
	
	H3Mesh* mesh = (H3Mesh* ) this->meshRef ;
	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	
	H3AABB* aabb = &mesh->aabb ;
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
	
	//H3DrawPrimitives::setColor ( 1.0 , 0.0 , 0.0 , 1.0 ) ;
	//H3Vector3f basics = child->h3Transform.getBasics () ;
	//H3Vector3f tranformedBasics = mat.mul ( basics ) ;
	//H3DrawPrimitives::drawLine ( tranformedBasics , tranformedBasics.mul(2.0f) ) ;

#endif

#if 0
	{
	//	draw aabb
	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	for ( SCClList* walker = this->orignalAABBRefList.next ; walker ; walker = walker->next  ) {
	
		H3AABB* aabb = (H3AABB* ) walker->element ;
		H3AABB tranformed = aabb->transform ( mat ) ;
	
		H3Vector3f* aabbCorner = tranformed.getCorners () ;
	
		//	near plane
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
	}
#endif
}

/*
 brief : Tests whether this bounding box intersects the specified bounding object.
*/
bool H3Object::intersect ( const H3AABB& aabb ) 
{

	bool ret = false ;

	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	for ( SCClList* walker = this->orignalAABBRefList.next ; walker ; walker = walker->next  ) {
	
		H3AABB tranformed = ((H3AABB* ) walker->element)->transform ( mat ) ;
		bool ret = tranformed.intersect ( aabb ) ;
		if ( true == ret )
			break ;
	}

	return ret ;
}

/*
 brief : Tests whether this bounding box intersects the specified ray.
		return mesh group id if intersected otherwise return -1
		return intersect point
*/
iS32 H3Object::intersect ( H3Ray ray , H3Vector3f* color , H3Vector3f& intersectPoint ) 
{

	iS32 ret = -1 ;

	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	iU32 looper = 0 ;
	for ( SCClList* walker = this->orignalAABBRefList.next ; walker ; walker = walker->next  ) {
	
		H3AABB tranformed = ((H3AABB* ) walker->element)->transform ( mat ) ;
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
			H3MeshGroup* group = ((H3Mesh* )this->meshRef)->meshGroupList[looper] ;
			if ( group->material ) {
				group->material->addColor ( H3_MATERIAL_COLOR_AMBIENT , color ) ;
			}
			ret = group->id ;
			//	caculate intersection point
			H3Vector3f dis = ( group->aabb.min - group->aabb.max ) ;
			dis = dis / 2.0f ;
			intersectPoint = dis + group->aabb.max ; 
			intersectPoint = intersectPoint + this->getPosition() + this->h3Transform.getBasics() ;
			break ;
		} 

		looper ++ ;

	}

	return ret ;
}

/*
 brief : check whether the point is in.
*/
bool H3Object::containPoint(const H3Vector3f& point ) 
{
	bool ret = false ;

	H3Matrix4x4f mat = this->finishTransformation ( this->isBillboard ) ; 
	for ( SCClList* walker = this->orignalAABBRefList.next ; walker ; walker = walker->next  ) {
	
		H3AABB tranformed = ((H3AABB* ) walker->element)->transform ( mat ) ;
		bool ret = tranformed.containPoint ( point ) ;
		if ( true == ret )
			break ;
	}

	return ret ;

}

/*
 brief : get object mesh
*/
H3Mesh* H3Object::getMesh () 
{
	return this->meshRef ;
}

/*
 breif : get mesh position witch MeshGroup Id
 Param : MeshGroup Id
*/
H3Vector3f H3Object::getMeshPosition ( iF32 id ) 
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
 brief : skip light shading
*/
void H3Object::skipLightShader () 
{
	this->isSkipLightShader = true ;
} 

/*
 brief : run an action
*/
void H3Object::startAction ( H3Action* action ) 
{
	H3ActionPool::getInstance()->addAction ( action ) ;
}

/*
 brief : 
*/
void H3Object::removeAllChildAndComponent ()
{
	//	delete childs
	for ( SCClList* walker = this->childs.next ; walker ; walker = walker->next ) {
		H3Object* child = (H3Object* )walker->element ;
		child->release () ;
	}
	SCClListClear ( &this->childs ) ;
	   
	//	delete components
	for ( SCClList* walker = this->components.next ; walker ; walker = walker->next ) {
		H3Component* component = (H3Component* )walker->element ;
		component->release () ;
	}
	SCClListClear ( &this->components ) ;
	
	//	delete light components
	for ( SCClList* walker = this->lightComponents.next ; walker ; walker = walker->next ) {
		H3Component* component = (H3Component* )walker->element ;
		component->release () ;
	}
	SCClListClear ( &this->lightComponents ) ;

}

/*
brief : for align origin (0,0,0) to center , enable = true as default
*/
void H3Object::setBasicsAlignEnable ( bool enable )
{
	this->basicsAlignEnable = enable;
}