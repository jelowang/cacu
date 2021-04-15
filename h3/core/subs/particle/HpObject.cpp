/*****************************************
 * @project   Mofun Physics
 * @file     HpObject.cpp
 * @brief    物体
 * @author   jelo
 * @date     2014-11-4
 * (C)Mofun All Rights Reserved Since 2014
 *****************************************/

#include "HpObject.h"
#include "math.h"

HpObject::HpObject()
{
	//	A = F/M
	m_mass = 1.0f ;
	m_friction = 0.0f ; 
	m_drag = 0.0f ;
	m_id = 0 ;
	m_velocity.init ( 0.0f , 0.0f ) ;
	m_positionExtra = 0.0f ;
	m_position = 0.0f ;
}
 
/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpObject* HpObject::createObject () 
{
	HpObject* object = new HpObject () ;

	if ( NULL != object ) {
		object->m_id = object ;
		return object ;
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
void HpObject::setMass ( float mass ) 
{
	this->m_mass = mass ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::setId ( void* id ) 
{
	this->m_id = id ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::setFriction ( float friction ) 
{
	m_friction = friction ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::setDrag ( float drag ) 
{
	m_drag = drag ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void* HpObject::getId () 
{
	return this->m_id ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::applyForce ( HpVector& force ) 
{

	HpVector acceler ;

	acceler.add ( force ) ;
	acceler.add ( getFrictionForce() ) ;

	//	关闭阻力模拟
	//acceler.add ( getDragForce() ) ;

	//	F = A*M
	//	A = F/M
	assert(!HP_IS_ZERO(m_mass));
	acceler.div ( m_mass ) ;

	m_velocity.add ( acceler ) ;
	m_position.add ( m_velocity ) ;
	m_position = m_position ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpVector& HpObject::getPosition () 
{
	return this->m_position ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::setPosition ( HpVector& position ) 
{
	this->m_position = position ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::copy ( HpObject* object ) 
{
	
	this->m_mass = object->m_mass ;
	this->m_friction = object->m_friction  ;
	this->m_drag = object->m_drag  ;

	this->m_id = object->m_id  ;

	this->m_velocity = object->m_velocity  ;
	this->m_position = object->m_position  ;

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
void HpObject::update () 
{

	HpVector acceler ;
	
	acceler.add ( getFrictionForce() ) ;
	//	关闭阻力模拟
	//acceler.add ( getDragForce() ) ;

	//	F = A*M
	//	A = F/M
	acceler.div ( m_mass ) ;

	m_velocity.add ( acceler ) ;
	m_position.add ( m_velocity ) ;

	//	微小速率设为静止
	//if ( 0.01 > m_velocity.meg () && m_velocity.meg () > -0.01 ) {
	//	m_velocity.mul ( 0.0f ) ;
	//}

}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpVector& HpObject::getFrictionForce () 
{
	//	f = -1 * u * N * v~

	static HpVector fforce ;

	//	正交力假设为1
	float N = 1.0f ;
	fforce = m_velocity.copy () ;	
	fforce.normalize () ;
	fforce.mul(-1.0f) ;
	fforce.mul( m_friction*N ) ;

	return fforce ;

}
 
/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
*/
HpVector& HpObject::getDragForce ()
{

	//	fd = ||V||^2 * Cd * V~ * -1
	static HpVector fdorce ;
	fdorce = m_velocity.copy () ;	
	fdorce.mul ( -1 ) ;
	fdorce.normalize () ;
	fdorce.mul ( m_drag ) ;
	
	float speed = m_velocity.meg () ;	
	
	fdorce.mul ( speed*speed ) ;	

	return fdorce ;

}