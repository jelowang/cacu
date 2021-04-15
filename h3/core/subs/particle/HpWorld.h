/*****************************************
 * @project   Mofun Physics
 * @file     HpWorld.h
 * @brief    世界
 * @author   jelo
 * @date     2014-11-4
 * (C)Mofun All Rights Reserved Since 2014
 *****************************************/

#ifndef __HP_WORLD_H__
#define __HP_WORLD_H__

#include "HpAdapter.h"
#include "HpGeometry.h"
#include "sccl.h"

#include "HpVector.h"
#include "HpObject.h"

#define MAX_OBJECT 64

class HpWorld 
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
	HpWorld () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	~HpWorld () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void* createObject ( void* node ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setOrgCoordinate ( HpVector coor ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void deleteObject ( void* obj ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setObjectPosition ( void* obj , HpVector& position ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setObjectPositionEx ( void* obj , const HpPoint &positon ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpVector& getObjectPosition ( void* obj ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpObject* getObject ( void* node ) ;
	
		/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void setGravity ( float gravity ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	float getGravity () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void applyForce ( void* obj , HpVector& force ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return radians
	 *  none
	*/
	float getObjectHeadDirection ( void* obj ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return degree
	 *  none
	*/
	float getObjectHeadDirection ( HpVector& u , HpVector& v ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void rotateObjectHeadDirection ( void* obj , float radians ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void scaleObjectVelocity ( void* obj , float scale ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpVector& getDirectionForce ( HpVector& from , HpVector& to ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return 
	 *  none
	*/
	void swapObject ( void* obj1 , void* obj2 ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void destroy () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void applyRadialForce ( HpVector position , float radius ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void applyForce ( HpVector position , float force ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void applyRadialForce ( HpObject* object , HpVector position , float radius ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void applyForce ( HpObject* object , HpVector position , float force ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void updateObject ( HpObject* object ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void run ( float dt ) ;

private :
	
	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	HpVector& getAttractForce ( HpObject u , HpObject v ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	*/
	void simulateGravitationalForce () ;

private :

	float m_G ;

	//	原点位置，可以不为0，默认为0
	HpVector m_orgCoordinate ;

	SCClList m_objectList ;

} ;
#endif