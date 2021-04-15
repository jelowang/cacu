/*****************************************
 * @project  Mofun Physics
 * @file     HpVector.cpp
 * @brief    Ê¸Á¿
 * @author   jelo
 * @date     2014-11-3
 * (C)Mofun All Rights Reserved Since 2014
 *****************************************/

#include "HpVector.h"
#include "math.h"

HpVector::HpVector()
{
	this->m_x = 0.0 ;
	this->m_y = 0.0 ;
}
/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
HpVector::HpVector ( float x , float y ):m_x(0.0),m_y(0.0)
{
	this->m_x = x ;
	this->m_y = y ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
void HpVector::init ( float x , float y ) 
{
	this->m_x = x ;
	this->m_y = y ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
void HpVector::add ( HpVector v ) 
{
	this->m_x = this->m_x + v.m_x ;
	this->m_y = this->m_y + v.m_y ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
void HpVector::sub ( HpVector v ) 
{
	this->m_x = this->m_x - v.m_x ;
	this->m_y = this->m_y - v.m_y ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
void HpVector::mul ( float n ) 
{
	this->m_x = this->m_x * n ;
	this->m_y = this->m_y * n ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
void HpVector::div ( float n ) 
{
	this->m_x = this->m_x / n ;
	this->m_y = this->m_y / n ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
float HpVector::meg () 
{
	return CarmSqrt(this->m_x*this->m_x + this->m_y*this->m_y) ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
 */
float HpVector::gradient () 
{
	if ( 0.00001 > this->m_x && this->m_x > -0.00001 ) {
		return 0xffffffff ;
	} else if ( 0.00001 > this->m_y && this->m_y > -0.00001 ) {
		return 0.0 ;
	}
	return (this->m_y / this->m_x) ;
}

/**
/* @brief
/*   
/* @param   
/*  
/* @return
/*  none
/*/
void HpVector::normalize () 
{
	float value = 0.0f ;
	value = meg () ;
	
	if ( 0 == value ) 
		return ;

	div (value) ;
}

/**
 * @brief
 *   
 * @param   
 *  
 * @return
 *  none
 */
void HpVector::limit ( float n ) 
{
	if ( meg() >= n ) {
		float value = 0.0f ;
		value = meg() / n ;
		div(value) ;
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
HpVector& HpVector::copy () 
{
	return *this;
}
