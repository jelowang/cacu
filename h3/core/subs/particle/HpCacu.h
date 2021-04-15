
/*
+	HpCacu.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef HpCacu_H
#define HpCacu_H

#include "HpParticle.h"
#include "HpEmitter.h"

#define HpCacuFileMagic "cacu"
#define HpCacuFileVersion 0x1 

/*
	brief : header of cacu file
*/
typedef struct {

	char magic[6] ;
	iU32 version ;
	
	//	totall parent emitter , not include child
	iU32 totallEmitter ;
	iU32 emitterOffset ; 

} HpCacuFileHeader ;

/*
	brief : structure of h3p file
*/
typedef struct {
	
	char m_name [32] ;
	
	HpeMODE m_mode ;
	HpeSIZEMODE m_sizemode ;
	HpeRANDMODE m_randmode ;
	
	//	in sec
	float m_starttime ;
	//	in sec
	float m_emittDuration ;
	//	totall particle but not include child
	int m_emittTotall ;
	//	world gravity 
	float m_gravity ;

	int m_BlendFuncSrc ;
	int m_BlendFuncDst ;
	HpeTexture m_hpeTexture ;

	//	particle bellow
	struct {
		//*	单个粒子生命时长，秒
		float m_lifeSpan ;
		//*	生命时长变化率，指定一个随机范围
		float m_lifeSpanVariance ;
	
		//*	颜色变化
		struct {
			hpColor4F m_startColor ;
			hpColor4F m_startColorVariance ;

			hpColor4F m_endColor ;
			hpColor4F m_endColorVariance ;
		} m_color ;
	
		//*	尺寸变化
		struct {

			//*	MpSizeModeVHAsSameRatio 模式时，只用Horizontal记录开始尺寸跟结束尺
			float m_startSizeHorizontal ;
			float m_startSizeHorizontalVariance ;

			float m_endSizeHorizontal ;
			float m_endSizeHorizontalVariance ;

			float m_startSizeVertical ;		
			float m_startSizeVerticalVariance ;	

			float m_endSizeVertical ;	
			float m_endSizeVerticalVariance ;		

		} m_size ;

		//*	喷口
		struct {
		
			float startAngle ;
			float angleVariance ;

		} m_emitter ;

		//*	自转
		struct {
			float m_startAngle ;
			float m_startAngleVariance ;

			float m_endAngle ;
			float m_endAngleVariance ;
		} m_rotation ;

		//*	公转
		//*	公转的喷口有emitter指定
		struct {
		
			float m_angularSpeed ;
			float m_angularSpeedVariance ;

			float m_startRadius ;
			float m_startRadiusVariance ;

			float m_endRadius ;
			float m_endRadiusVariance ;

		} m_revolution ;

		//*	初始力
		HpVector m_force ;	
		float m_friction ;
		float m_mass ;
	} particle ;
	//	particle end

} HpCacuEmitter ;

//	for 0x0 version
/*
	brief : emitt texture frame
*/
typedef struct {

	char m_name [ 64 ] ;
	//	a handle to HpTexture2D
	void* m_handle ;
	//	texture id of rendering pipeline
	void* m_id ;
	
	//#	per fps duration
	float m_duration ;

	struct {
		int x1 ;
		int y1 ;
		int x2 ;
		int y2 ;
	} m_rect ;

	int m_widthInPixels ;
	int m_heightInPixels ;

} HpeTextureAtom0x0 ;

/*
	brief : emitt texture frame list
*/
typedef struct {

	HpeTextureAtom0x0 m_framelist[PARTICLE_MAX_FRAME] ;
	int m_totallFrame ;

} HpeTexture0x0 ;
/*
	brief : structure of h3p file
*/
typedef struct {
	
	char m_name [32] ;
	
	HpeMODE m_mode ;
	HpeSIZEMODE m_sizemode ;
	HpeRANDMODE m_randmode ;
	
	//	in sec
	float m_starttime ;
	//	in sec
	float m_emittDuration ;
	//	totall particle but not include child
	int m_emittTotall ;
	//	world gravity 
	float m_gravity ;

	long m_BlendFuncSrc ;
	long m_BlendFuncDst ;
	HpeTexture0x0 m_hpeTexture ;

	//	particle bellow
	struct {
		//*	单个粒子生命时长，秒
		float m_lifeSpan ;
		//*	生命时长变化率，指定一个随机范围
		float m_lifeSpanVariance ;
	
		//*	颜色变化
		struct {
			hpColor4F m_startColor ;
			hpColor4F m_startColorVariance ;

			hpColor4F m_endColor ;
			hpColor4F m_endColorVariance ;
		} m_color ;
	
		//*	尺寸变化
		struct {

			//*	MpSizeModeVHAsSameRatio 模式时，只用Horizontal记录开始尺寸跟结束尺
			float m_startSizeHorizontal ;
			float m_startSizeHorizontalVariance ;

			float m_endSizeHorizontal ;
			float m_endSizeHorizontalVariance ;

			float m_startSizeVertical ;		
			float m_startSizeVerticalVariance ;	

			float m_endSizeVertical ;	
			float m_endSizeVerticalVariance ;		

		} m_size ;

		//*	喷口
		struct {
		
			float startAngle ;
			float angleVariance ;

		} m_emitter ;

		//*	自转
		struct {
			float m_startAngle ;
			float m_startAngleVariance ;

			float m_endAngle ;
			float m_endAngleVariance ;
		} m_rotation ;

		//*	公转
		//*	公转的喷口有emitter指定
		struct {
		
			float m_angularSpeed ;
			float m_angularSpeedVariance ;

			float m_startRadius ;
			float m_startRadiusVariance ;

			float m_endRadius ;
			float m_endRadiusVariance ;

		} m_revolution ;

		//*	初始力
		HpVector m_force ;	
		float m_friction ;
		float m_mass ;
	} particle ;
	//	particle end

} HpCacuEmitter0x0 ;
//

extern HpEmitter* HpCacuFileCacuEmitter2HpEmitter ( HpCacuEmitter* emitter , HpEmitter* et ) ;
extern void HpCacuFileSave ( const iS8* file , SCClList* emitterListIn ) ;
extern void HpCacuFileLoad ( const iS8* file , SCClList* emitterListOut ) ;
extern void HpCacuFileLoad0x0 ( const iS8* file , SCClList* emitterListOut ) ;

#endif