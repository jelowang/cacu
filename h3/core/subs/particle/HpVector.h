/*****************************************
 * @project  Mofun Physics
 * @file     HpVector.h
 * @brief    Ê¸Á¿
 * @author   jelo
 * @date     2014-11-3
 * (C)Mofun All Rights Reserved Since 2014
 *****************************************/
  
#ifndef __HP_ADAPTER_HVECTOR_H__
#define __HP_ADAPTER_HVECTOR_H__

#include "HpAdapter.h"
#include "HpMath.h"

#define HpVectorCreate(x,y) HpVector((float )x,(float )y)
#define HpVector2ccp(p) HpPoint((float )p.m_x,(float )p.m_y)
#define ccp2HpVector(p) HpVector((float )p.x,(float )p.y)

class HpVector 
{
public :

	HpVector () ;
	HpVector ( float x , float y ) ;
	~HpVector () {}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	static HpVector& add ( HpVector& v , HpVector& u ) 
	{
		static HpVector n ;
		n.init( v.m_x+u.m_x , v.m_y+u.m_y ) ;
		return n ;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	static HpVector& sub ( HpVector& v , HpVector& u ) 
	{
		static HpVector n ;
		n.init( v.m_x-u.m_x , v.m_y-u.m_y ) ;
		return n ;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	static HpVector& Perpendicular ( HpVector& v ) 
	{
		static HpVector n ;
		n.init( -v.m_y , v.m_x ) ;
		return n ;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	static HpVector& average ( HpVector& v , HpVector& u ) 
	{
		static HpVector n ;
		n.init( v.m_x+u.m_x , v.m_y+u.m_y ) ;
		n.div (2.0f) ;
		return n ;
	}

	static HpVector& rotate ( HpVector v , float degree )
	{

		static HpVector n ;

		n = v ;

		float r = n.meg () ;

		n.m_x = r * HpCosine ( HP_ADAPTER_HDEGREES_TO_RADIANS(degree) ) ;
		n.m_y = r * HpSine ( HP_ADAPTER_HDEGREES_TO_RADIANS(degree) ) ;

		return n ;

	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	HpVector& operator = (const HpVector& other)
	{
		init(other.m_x,other.m_y) ;
		return *this;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	HpVector& operator = (float value)
	{
		init(value,value) ;
		return *this;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	HpVector& operator - (HpVector& v)
	{
		static HpVector n ;
		n.init( this->m_x-v.m_x , this->m_y-v.m_y ) ;
		return n ;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	HpVector& operator + (HpVector& v)
	{
		static HpVector n ;
		n.init( this->m_x+v.m_x , this->m_y+v.m_y ) ;
		return n ;
	}

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	void init ( float x , float y ) ;
	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	void add ( HpVector v ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	void sub ( HpVector v ) ;
	
	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */	
	void mul ( float n ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	void div ( float n ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	float meg () ;


	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	float gradient () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	void normalize () ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	void limit ( float n ) ;

	/**
	 * @brief
	 *   
	 * @param   
	 *  
	 * @return
	 *  none
	 */
	HpVector& copy () ;

public :

	float m_x ;
	float m_y ;

} ;
#endif