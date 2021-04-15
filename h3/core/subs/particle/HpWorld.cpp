/*****************************************
 * @project  Mofun Physics
 * @file     HpWorld.h
 * @brief    世界
 * @author   jelo
 * @date     2014-11-4
 * (C)Mofun All Rights Reserved Since 2014
 *****************************************/

#include "HpWorld.h"

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpWorld::HpWorld () 
{
	SCClListInit ( &m_objectList ) ;
	m_orgCoordinate = 0 ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpWorld::~HpWorld () 
{
	destroy () ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void* HpWorld::createObject ( void* node ) 
{
	HpObject* object = HpObject::createObject() ;
	object->setId ( node ) ;

	SCClListInsert ( &m_objectList , (void* )object ) ;

	return object ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::setOrgCoordinate ( HpVector coor ) 
{
	m_orgCoordinate = coor ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::deleteObject ( void* obj ) 
{
	
	HpObject* object = (HpObject *)obj ;
	assert (object) ;
	delete object ;
	
	SCClListDelete ( &m_objectList , (void* )object ) ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::setObjectPosition ( void* obj , HpVector& position ) 
{
	HpObject* object = (HpObject *)obj ;
	assert (object) ;
	object->setPosition ( position ) ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::setObjectPositionEx ( void* obj , const HpPoint& position ) 
{
	HpObject* object = (HpObject *)obj ;
	assert (object) ;
	HpVector v ; 
	v.m_x = position.x ;
	v.m_y = position.y ;
	object->setPosition ( v ) ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpVector& HpWorld::getObjectPosition ( void* obj ) 
{
	static HpVector v ;
	HpObject* object = (HpObject *)obj ;
	assert (object) ;
	v = object->getPosition () + m_orgCoordinate ;
	return v ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpObject* HpWorld::getObject ( void* node ) 
{

	HpObject* object = 0 ;

	for ( SCClList* walker = m_objectList.next ; walker ; walker = walker->next )
	{
		object = (HpObject* )walker->element ;
		assert ( object ) ;
		if ( node == object->getId() ) 
		{
			break ;
		}
	}

	return object ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::setGravity ( float gravity ) 
{
	m_G = gravity ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
float HpWorld::getGravity () 
{
	return m_G ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::applyForce ( void* obj , HpVector& force ) 
{
	HpObject* object = (HpObject *)obj ;
	assert(object) ;
	object->applyForce ( force ) ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return radians
 *  none
*/
float HpWorld::getObjectHeadDirection ( void* obj ) 
{

	float radians = 0.0f ;
	HpObject* object = (HpObject *)obj ;
	assert(object) ;

	radians = atan2 ( object->getPosition().m_y , object->getPosition().m_x ) ;
	//radians = (radians*180.0f) / 3.1415926 ;

	return radians ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return degree
 *  none
*/
float HpWorld::getObjectHeadDirection ( HpVector& u , HpVector& v ) 
{

	float radians = 0.0f ;
	HpVector direction ;

	direction = HpVector::sub(u,v) ;

	radians = atan2 ( direction.m_y , direction.m_x ) ;
	radians = (radians*180.0f) / 3.1415926 ;

	return radians ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::rotateObjectHeadDirection ( void* obj , float radians ) 
{

	float r = 0.0f ;
	HpObject* object = (HpObject *)obj ;
	assert(object) ;

	r = object->m_velocity.meg () ;

	object->m_velocity.m_x = r * HpCosine ( radians  ) ;
	object->m_velocity.m_y = r * HpSine ( radians ) ;

	return ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::scaleObjectVelocity ( void* obj , float scale ) 
{
	HpObject* object = (HpObject *)obj ;
	assert(object) ;
	object->m_velocity.mul ( scale ) ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpVector& HpWorld::getDirectionForce ( HpVector& from , HpVector& to ) 
{

	static HpVector force ;

	force = HpVector::sub ( to , from ) ;
	force.normalize () ;
	
	return force ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::swapObject ( void* obj1 , void* obj2 ) 
{
	HpObject* object1 = (HpObject *)obj1 ;
	HpObject* object2 = (HpObject *)obj2 ;
	HpObject object3 ;

	object3.copy ( object1 ) ;
	object1->copy ( object2 ) ;
	object2->copy ( &object3 ) ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::destroy () 
{

	for ( SCClList* walker = m_objectList.next ; walker ; walker = walker->next )
	{
		HpObject* object = (HpObject* )walker->element ;
		assert ( object ) ;	
		delete object ;
	}

	SCClListClear ( &m_objectList ) ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::applyRadialForce ( HpVector position , float radius ) 
{

	//	update object

	for ( SCClList* walker = m_objectList.next ; walker ; walker = walker->next )
	{
		HpObject* object = (HpObject* )walker->element ;
		assert ( object ) ;

		//	解析m_positionExtra
		//	HpParticle中有定义
		HpVector sub = (object->m_position+object->m_positionExtra)-position+m_orgCoordinate ;
		float coff = 1.0f - (sub.meg()/radius);

		if ( 0.0f <= coff && coff <= 1.0f ) {

			sub.normalize();
			sub.mul(coff*radius) ;
			object->applyForce ( sub ) ;

		}
		
	}

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::applyForce ( HpVector position , float force ) 
{
	//	update object

	for ( SCClList* walker = m_objectList.next ; walker ; walker = walker->next )
	{
		HpObject* object = (HpObject* )walker->element ;
		assert ( object ) ;

		//	解析m_positionExtra
		//	HpParticle中有定义
		HpVector sub = (object->m_position+object->m_positionExtra)-position+m_orgCoordinate ;

		sub.normalize();
		sub.mul(force) ;
		object->applyForce ( sub ) ;
	
	}
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::applyRadialForce ( HpObject* object , HpVector position , float radius ) 
{

	//	update object

	assert ( object ) ;

	//	解析m_positionExtra
	//	HpParticle中有定义
	HpVector sub = (object->m_position+object->m_positionExtra)-position+m_orgCoordinate ;
	float coff = 1.0f - (sub.meg()/radius);

	if ( 0.0f <= coff && coff <= 1.0f ) {

		sub.normalize();
		sub.mul(coff*radius) ;
		object->applyForce ( sub ) ;

	}

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::applyForce ( HpObject* object , HpVector position , float force ) 
{
	//	update object

	assert ( object ) ;

	//	解析m_positionExtra
	//	HpParticle中有定义
	HpVector sub = (object->m_position+object->m_positionExtra)-position+m_orgCoordinate ;

	sub.normalize();
	sub.mul(force) ;
	object->applyForce ( sub ) ;
	
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::updateObject ( HpObject* object ) 
{

	assert ( object ) ;

	HpVector gravity ;
	gravity.init ( 0.0f , m_G ) ;
	object->applyForce ( gravity ) ;

	object->update () ;			

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::run ( float dt ) 
{

	//	update object

	for ( SCClList* walker = m_objectList.next ; walker ; walker = walker->next )
	{
		HpObject* object = (HpObject* )walker->element ;
		assert ( object ) ;

		HpVector gravity ;
		gravity.init ( 0.0f , m_G ) ;
		object->applyForce ( gravity ) ;

		object->update () ;				

		//if ( 0 == rand()%20 ) {
		//	HpVector gravity ;
		//	gravity.init ( rand()%2*CCRANDOM_MINUS1_1() , rand()%2*CCRANDOM_MINUS1_1() ) ;
		//	object->applyForce ( gravity ) ;
		//}

	}

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpVector& HpWorld::getAttractForce ( HpObject u , HpObject v ) 
{

	float gravatition = 0.0f ;
	float distence = 0.0f ;
	static HpVector force ;
	force = HpVector::sub ( u.getPosition () , v.getPosition () ) ;
	distence = force.meg () ;

	force.normalize () ;
	//	根据万有引力公式
	gravatition = (m_G * u.m_mass * v.m_mass) / ( distence*distence ) ;
	force.mul ( gravatition ) ;

	return force ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpWorld::simulateGravitationalForce () 
{

	//	for every object is attracting each other
	//	just add g-force to velocity here
#if 0
	for ( list < HpObject* > ::iterator ito = m_objectList.begin(); ito != m_objectList.end(); ito++  ) {

		HpVector force ;
		HpObject* object = 0 ;

		object = (HpObject* )*ito ;
		assert (object) ;

		for ( list < HpObject* > ::iterator itoo = m_objectList.begin(); itoo != m_objectList.end(); itoo++ ) {
		
			HpObject* oo = 0 ;
			oo = (HpObject* )*itoo ;
		
			if ( oo == object ) 
				continue ;

			force = getAttractForce ( oo , object ) ;
			oo->applyForce ( force ) ;
			oo->update () ;

		}

	}
#endif

}
