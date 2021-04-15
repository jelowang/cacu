
/*
+	HpParticle.h
+	Jelo Wang

+	Hello Particle 
+	realtime VFX engine
																				
																												
+	(C)	���Ӷ���Quantum Dynamic
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

	//*	�ѹ�ȥʱ��
	float m_elapsedTime ;
	//*	������������ʱ������
	float m_lifeSpan ;
	//*	����ʱ���仯�ʣ�ָ��һ�������Χ
	float m_lifeSpanVariance ;
	//*	m_lifeSpanResults =	m_lifeSpan + (m_lifeSpanVariance*CCRANDOM_MINUS1_1())
	float m_lifeSpanResults ;

	//*	��ɫ�仯
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
		//*	��ɫ��������
		//*	�ĸ�ͨ���仯�ʲ�ͬ
		hpColor4F m_colorGradualAccel ; 
	} m_color ;
	
	//*	�ߴ�仯
	struct {

		//*	MpSizeModeVHAsSameRatio ģʽʱ��ֻ��Horizontal��¼��ʼ�ߴ��������
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
	//*	��ʼλ��
	HpVector m_startPosition ;
	HpVector m_startPositionVariance ;
	//*	�յ�λ��
	HpVector m_endPosition ;
	HpVector m_endPositionVariance ;
#endif

	//*	��ǰλ��
	HpVector m_deltaPosition ;

	//*	���
	struct {
		
		float startAngle ;
		float angleVariance ;
		//*	startAngleResults =	startAngle + (angleVariance*CCRANDOM_MINUS1_1())
		float startAngleResults ;

	} m_emitter ;

	//*	��ת
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

	//*	��ת
	//*	��ת�������emitterָ��
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

	//*	�������
	void* m_physicsObject ;
	//*	��ʼ��
	HpVector m_force ;	
	float m_friction ;
	float m_mass ;
	
} HpParticle ;

#endif

