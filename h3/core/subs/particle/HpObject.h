/*****************************************
 * @project  Mofun Physics
 * @file     HpObject.h
 * @brief    ����
 * @author   jelo
 * @date     2014-11-4
 * (C)Mofun All Rights Reserved Since 2014
 *****************************************/

#ifndef __HP_OBJECT_H__
#define __HP_OBJECT_H__

#include "HpAdapter.h"
#include "HpVector.h"

class HpObject
{
public :

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpObject () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	~HpObject () {}
	
	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	static HpObject* createObject () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setMass ( float mass ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setFriction ( float friction ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setDrag ( float drag ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setId ( void* id ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void* getId () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void applyForce ( HpVector& force ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpVector& getPosition () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setPosition ( HpVector& position ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void copy ( HpObject* object ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void update () ;

private :

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpVector& getFrictionForce () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpVector& getDragForce () ;


public :

	float m_mass ;
	float m_friction ;
	float m_drag ;

	void* m_id ;

	HpVector m_velocity ;
	HpVector m_position ;
	//	�������꣬���庬�壬�ɵ����߶���
	//	��ˣ�object �ڲ����꣬����ʹ����ֵ�������ⲿ����
	HpVector m_positionExtra ;


} ;
#endif