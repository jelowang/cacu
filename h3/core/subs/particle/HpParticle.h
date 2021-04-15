
/*
+	HpParticle.h
+	Jelo Wang

+	Hello Particle 
+	realtime VFX engine
																				
																												
+	(C)	量子动力Quantum Dynamic
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef HP_PARTICLE_H
#define HP_PARTICLE_H

#include "HpAdapter.h"
#include "HpVector.h"
#include "HpObject.h"
#include "HpWorld.h"

using namespace std;

/*
	brief : emitt mode
*/
typedef enum {

	HpParticleModeGravity = 0 ,
	HpParticleModeRadius ,
	HpParticleModeInfinite = -1 ,

} HpeMODE ;

/*
	brief : emitt size mode
*/
typedef enum {

	HpSizeModeVHAsSameRatio ,
	HpSizeModeVHAsDiffRatio ,

} HpeSIZEMODE ;

/*
	brief : emitt rand mode
*/
typedef enum {

	//	rand once
	HpRandomModeOnce ,
	//	rand every times when a particle reborn
	HpRandomModeEvery ,

} HpeRANDMODE ;

/*
+	struct : HpParticleData

+	brief : defination of particle

*/
typedef struct {

	//*	已过去时间
	float m_elapsedTime ;
	//*	单个粒子生命时长，秒
	float m_lifeSpan ;
	//*	生命时长变化率，指定一个随机范围
	float m_lifeSpanVariance ;
	//*	m_lifeSpanResults =	m_lifeSpan + (m_lifeSpanVariance*CCRANDOM_MINUS1_1())
	float m_lifeSpanResults ;

	//*	颜色变化
	struct {
		hpColor4F m_startColor ;
		hpColor4F m_startColorVariance ;
		//*	m_startColorResults =	m_startColor + (m_startColorVariance*CCRANDOM_MINUS1_1())
		hpColor4F m_startColorResults ;
		hpColor4F m_endColor ;
		hpColor4F m_endColorVariance ;
		//*	m_endColorResults =	m_endColor + (m_m_endColorVariance*CCRANDOM_MINUS1_1())
		hpColor4F m_endColorResults ;
		hpColor4F m_deltaColor ;
		//*	颜色渐变速率
		//*	四个通道变化率不同
		hpColor4F m_colorGradualAccel ; 
	} m_color ;
	
	//*	尺寸变化
	struct {

		//*	MpSizeModeVHAsSameRatio 模式时，只用Horizontal记录开始尺寸跟结束尺
		float m_startSizeHorizontal ;
		float m_startSizeHorizontalVariance ;
		//*	m_startSizeHorizontalResults =	m_startSizeHorizontal + (m_startSizeHorizontalVariance*CCRANDOM_MINUS1_1())
		float m_startSizeHorizontalResults ;
		float m_endSizeHorizontal ;
		float m_endSizeHorizontalVariance ;
		//*	m_endSizeHorizontal =	m_endSizeHorizontal + (m_endSizeHorizontalVariance*CCRANDOM_MINUS1_1())
		float m_endSizeHorizontalResults ;
		float m_sizeHorizontalGradualAccel ;

		float m_startSizeVertical ;		
		float m_startSizeVerticalVariance ;	
		//*	m_startSizeVerticalResults =	m_startSizeVertical + (m_startSizeVerticalVariance*CCRANDOM_MINUS1_1())
		float m_startSizeVerticalResults ;
		float m_endSizeVertical ;	
		float m_endSizeVerticalVariance ;		
		//*	m_endSizeVerticalResults =	m_endSizeVertical + (m_endSizeVerticalVariance*CCRANDOM_MINUS1_1())
		float m_endSizeVerticalResults ;
		float m_sizeVerticalGradualAccel ;		

	} m_size ;

#if 0
	//*	起始位置
	HpVector m_startPosition ;
	HpVector m_startPositionVariance ;
	//*	终点位置
	HpVector m_endPosition ;
	HpVector m_endPositionVariance ;
#endif

	//*	当前位置
	HpVector m_deltaPosition ;

	//*	喷口
	struct {
		
		float startAngle ;
		float angleVariance ;
		//*	startAngleResults =	startAngle + (angleVariance*CCRANDOM_MINUS1_1())
		float startAngleResults ;

	} m_emitter ;

	//*	自转
	struct {
		float m_startAngle ;
		float m_startAngleVariance ;
		//*	m_startAngleResults =	m_startAngle + (m_startAngleVariance*CCRANDOM_MINUS1_1())
		float m_startAngleResults ;
		float m_endAngle ;
		float m_endAngleVariance ;
		//*	m_startAngleResults =	m_endAngle + (m_endAngleVariance*CCRANDOM_MINUS1_1())
		float m_endAngleResults ;
		float m_angularAccel ;
	} m_rotation ;

	//*	公转
	//*	公转的喷口有emitter指定
	struct {
		
		float m_angularSpeed ;
		float m_angularSpeedVariance ;
		//*	m_angularSpeedResults =	m_angularSpeed + (m_angularSpeedVariance*CCRANDOM_MINUS1_1())
		float m_angularSpeedResults ;
		float m_deltaAngle ;

		float m_startRadius ;
		float m_startRadiusVariance ;
		//*	m_startRadiusResults =	m_startRadius + (m_startRadiusVariance*CCRANDOM_MINUS1_1())
		float m_startRadiusResults ;

		float m_endRadius ;
		float m_endRadiusVariance ;
		//*	m_endRadiusResults =	m_endRadius + (m_endRadiusVariance*CCRANDOM_MINUS1_1())
		float m_endRadiusResults ;

		float m_radiusSpeed ;
		float m_deltaRadius ;

	} m_revolution ;

#ifdef HP_COCOS2D_PORT
	V3F_C4B_T2F_Quad* m_pQuads ;
#endif

#ifdef HP_H3_PORT
	H3_V3F_T2F_C4F_Quad* m_pQuads ;
#endif

	//*	物理对象
	void* m_physicsObject ;
	//*	初始力
	HpVector m_force ;	
	float m_friction ;
	float m_mass ;
	
} HpParticle ;

#endif

