
/*
+	HpEmitter.cpp
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include <string.h>
#include "stdio.h" 
#include "schal.h" 
#include "HpCacu.h" 
#include "HpGeometry.h"
#include "HpEmitter.h"
#include "HpTexture2D.h"
#include "HpTextureCache.h"

HpEmitterContainer::HpEmitterContainer () 
{
	SCClListInit ( &m_HpEmitterList ) ;
	SCClListInit ( &m_HpeTextureList ) ;
}

HpEmitterContainer::~HpEmitterContainer () 
{	

	for ( SCClList* walker = m_HpEmitterList.next ; walker ; walker = walker->next ) {

		HpEmitter* node = 0 ;
		node = (HpEmitter* )walker->element ; 
		//	改为非静态，删除节点
		node->m_static = false ;
		destroyEmitter (node) ;

	}
	  
	SCClListClear ( &m_HpEmitterList ) ;
	SCClListClear ( &m_HpeTextureList ) ;

}

/*
	brief : just load one et from file
*/
HpEmitter* HpEmitterContainer::loadEmitter ( const char* file ) 
{

	HpCacuFileHeader header = { 0 } ;
	HpCacuEmitter emitter = { 0 } ;

	HpEmitter* pre = 0 ;
	HpEmitter* theFirst = 0 ;

	#ifdef HP_COCOS2D_PORT
		iU32 fileSeek = 0;
		string m_sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(file);
		ssize_t length = 0;
		unsigned char* dataMemory = CCFileUtils::getInstance()->getFileData(file, "rb", &length);
		if (0 == dataMemory) {
			iLog("not enough memory for %s , %d" , file , length );
			return 0;
		}

		memcpy(&header, dataMemory, sizeof(HpCacuFileHeader));

		if ( 0 != strcmp ( header.magic , HpCacuFileMagic ) ) {
			iLog ( "file %s is not legal" , file ) ;
			return 0 ; 
		}

		if ( header.version > HpCacuFileVersion ) {
			iLog ( "file %s version %d is not legal" , file , header.version ) ;
			return 0 ;
		}

		fileSeek = sizeof(HpCacuFileHeader);

		for ( iU32 looper = 0 ; looper < header.totallEmitter ; looper ++ ) 
		{
			HpCacuEmitter cacuEmitter = { 0 } ;
			HpEmitter* tree = { 0 } ;
			HpEmitter et = {0} ;
			memcpy( &cacuEmitter,((void*)((iU64)dataMemory + fileSeek)), sizeof(cacuEmitter));
			HpCacuFileCacuEmitter2HpEmitter ( &cacuEmitter , &et ) ;

			tree = loadEmitter ( &et ) ;

			//	loadd all textures
			for ( int count = 0 ; count < tree->m_hpeTexture.m_totallFrame ; count ++ ) {
				HpTexture2D* texture = 0 ;
				texture = HpTextureCache::getInstance()->getTexture ( 
					H3UtilString2Number(tree->m_hpeTexture.m_framelist[count].m_name).getValue() ) ;

				if ( 0 == texture ) {

					texture = MMC_NEW HpTexture2D () ;
					bool backup = SCHalGetAbsoluteMode () ;
					SCHalSetAbsoluteMode ( false ) ;
					texture->initWithPNGFile ( tree->m_hpeTexture.m_framelist[count].m_name ) ;
					SCHalSetAbsoluteMode ( backup ) ;

					HpTextureCache::getInstance()->addTexture (
						H3UtilString2Number(tree->m_hpeTexture.m_framelist[count].m_name).getValue() ,
						texture 
						) ;
				}
				texture->addRef () ;
				tree->m_actived = true ;
				tree->m_visible = true ;
				tree->m_hpeTexture.m_framelist[count].m_widthInPixels = texture->getContentSize().width ;
				tree->m_hpeTexture.m_framelist[count].m_heightInPixels = texture->getContentSize().height ;
				tree->m_hpeTexture.m_framelist[count].m_rect.x1 = 0 ;
				tree->m_hpeTexture.m_framelist[count].m_rect.y1 = 0 ;
				tree->m_hpeTexture.m_framelist[count].m_rect.x2 = tree->m_hpeTexture.m_framelist[count].m_widthInPixels ;
				tree->m_hpeTexture.m_framelist[count].m_rect.y2 = tree->m_hpeTexture.m_framelist[count].m_heightInPixels ;
				tree->m_hpeTexture.m_framelist[count].m_handle = (long long )texture ;
				tree->m_hpeTexture.m_framelist[count].m_id = (long long )texture->getId () ;

			}

			if ( 0 == looper ) theFirst = tree ;
			if ( 0 != pre ) pre->next = tree ;
			tree->pre = pre ;
			pre = tree ;

			fileSeek = fileSeek + sizeof (HpCacuEmitter) ;

		}

		return theFirst ;

	#else
		void* fp = SCHalFileOpen ( file , "rb" ) ;
	
	if ( 0 == fp ) {
		iLog ( "file %s is not exist" , file ) ;
		return 0 ;
	}

	SCHalFileRead ( fp , &header , sizeof(HpCacuFileHeader) ) ;

	if ( 0 != strcmp ( header.magic , HpCacuFileMagic ) ) {
		SCHalFileClose ( fp ) ;
		iLog ( "file %s is not legal" , file ) ;
		return 0 ; 
	}

	if ( header.version > HpCacuFileVersion ) {
		SCHalFileClose ( fp ) ;
		iLog ( "file %s version %d is not legal" , file , header.version ) ;
		return 0 ;
	}
	SCHalFileSeek ( fp , header.emitterOffset , SEEK_SET ) ;

	for ( iU32 looper = 0 ; looper < header.totallEmitter ; looper ++ ) 
	{
		HpCacuEmitter cacuEmitter = { 0 } ;
		HpEmitter* tree = { 0 } ;
		HpEmitter et = {0} ;
		SCHalFileRead ( fp , &cacuEmitter , sizeof(cacuEmitter) ) ;
		HpCacuFileCacuEmitter2HpEmitter ( &cacuEmitter , &et ) ;

		tree = loadEmitter ( &et ) ;

			//	loadd all textures
		for ( int count = 0 ; count < tree->m_hpeTexture.m_totallFrame ; count ++ ) {
			HpTexture2D* texture = 0 ;
			texture = HpTextureCache::getInstance()->getTexture ( 
				H3UtilString2Number(tree->m_hpeTexture.m_framelist[count].m_name).getValue() ) ;

			if ( 0 == texture ) {

				texture = MMC_NEW HpTexture2D () ;
				bool backup = SCHalGetAbsoluteMode () ;
				SCHalSetAbsoluteMode ( false ) ;
				texture->initWithPNGFile ( tree->m_hpeTexture.m_framelist[count].m_name ) ;
				SCHalSetAbsoluteMode ( backup ) ;

				HpTextureCache::getInstance()->addTexture (
					H3UtilString2Number(tree->m_hpeTexture.m_framelist[count].m_name).getValue() ,
					texture 
				) ;

			}
			texture->addRef () ;
			tree->m_actived = true ;
			tree->m_visible = true ;
			tree->m_hpeTexture.m_framelist[count].m_widthInPixels = texture->getContentSize().width ;
			tree->m_hpeTexture.m_framelist[count].m_heightInPixels = texture->getContentSize().height ;
			tree->m_hpeTexture.m_framelist[count].m_rect.x1 = 0 ;
			tree->m_hpeTexture.m_framelist[count].m_rect.y1 = 0 ;
			tree->m_hpeTexture.m_framelist[count].m_rect.x2 = tree->m_hpeTexture.m_framelist[count].m_widthInPixels ;
			tree->m_hpeTexture.m_framelist[count].m_rect.y2 = tree->m_hpeTexture.m_framelist[count].m_heightInPixels ;
			tree->m_hpeTexture.m_framelist[count].m_handle = (long long )texture ;
			tree->m_hpeTexture.m_framelist[count].m_id = (long long )texture->getId () ;

		}

		if ( 0 == looper ) theFirst = tree ;
		if ( 0 != pre ) pre->next = tree ;
		tree->pre = pre ;
		pre = tree ;
		
	}

	SCHalFileClose ( fp ) ;

	return theFirst ;

	#endif

}

/*
	brief : load et node
*/
HpEmitter* HpEmitterContainer::loadEmitter ( HpEmitter* tree ) 
{

	//	load
	HpEmitter* et = 0 ;
	{

		bool bRet = false;
		unsigned char *buffer = NULL;
		unsigned char *deflated = NULL;
		void* textureHandle = 0 ;
		HpeTexture* hpeTexture = 0 ;

		et = (HpEmitter* ) MMC_MALLOC ( sizeof(HpEmitter) );
		if ( 0 == et ) {
			iLog ( "Hp Error : Momory Malloc Failed , file:%s,line : %d" ,__FILE__,__LINE__) ;
			return 0 ;
		}
		memset ( et , 0 , sizeof(HpEmitter) ) ;
	
		strcpy ( et->m_name , tree->m_name ) ;
		
		et->m_HpWorld = new HpWorld () ;
		if ( NULL == et->m_HpWorld ) {
			iLog ( "Hp Error : Momory Malloc Failed , file:%s,line : %d" ,__FILE__,__LINE__) ;
			MMC_FREE(et);
			return 0 ;
		}

		//	load 产生的et都是static，不允许删除，因为用来做cache
		et->m_static = true ;
		et->m_mode = tree->m_mode ;
		et->m_sizemode = tree->m_sizemode ;
		et->m_visible = tree->m_visible ;

		et->m_emittDuration = tree->m_emittDuration ;
		et->m_emittTotall = tree->m_emittTotall ;

		//	喷口位置，位于原点
		et->m_orgCoordinate.init ( 0.0 , 0.0 ) ;
		et->m_emittPosition.init ( 0.0 , 0.0 ) ;

		et->m_elapsedTotallTime = 0.0f ;
		et->m_emittCounter = 0 ;
		et->m_emittTotallPerSec = 0.0f ;
		et->m_BlendFuncSrc = tree->m_BlendFuncSrc ;
		et->m_BlendFuncDst = tree->m_BlendFuncDst ;
		et->m_gravity = tree->m_gravity ;
		et->m_starttime = tree->m_starttime ;		 
		et->m_emittRate = et->m_emittTotall / tree->m_orignal_data.m_lifeSpan ;
		et->m_HpWorld->setGravity ( tree->m_gravity ) ;

		et->m_data = (HpParticle* )MMC_MALLOC( sizeof(HpParticle) * et->m_emittTotall ) ;
		if ( NULL == et->m_data ) {		
			delete et->m_HpWorld ;
			MMC_FREE(et);
			iLog ( "Hp Error : Momory Malloc Failed , file:%s,line : %d" ,__FILE__,__LINE__) ;
			return 0 ;
		}
		memset ( et->m_data , 0 , sizeof(HpParticle) * et->m_emittTotall ) ;
	
		memcpy ( &et->m_hpeTexture , &tree->m_hpeTexture , sizeof(et->m_hpeTexture) ) ;

		//	particle
		HpParticle* particleData = &et->m_orignal_data ;

		particleData->m_elapsedTime = 0.0f ;
		particleData->m_lifeSpan = tree->m_orignal_data.m_lifeSpan;
		particleData->m_lifeSpanVariance = tree->m_orignal_data.m_lifeSpanVariance;	
		//*	防止产生负数		
		particleData->m_lifeSpanResults = particleData->m_lifeSpan + ( particleData->m_lifeSpanVariance*HPRANDOM_MINUS1_1() ) ;	
		particleData->m_lifeSpanResults = HpMAX(0,particleData->m_lifeSpanResults) ;
		//*	防止除0
		if ( 0 == particleData->m_lifeSpanResults ) {
			particleData->m_lifeSpanResults = particleData->m_lifeSpan + particleData->m_lifeSpanVariance ;
		}

		//!	颜色
		//*	开始颜色
		particleData->m_color.m_startColor.r = tree->m_orignal_data.m_color.m_startColor.r;
		particleData->m_color.m_startColor.g = tree->m_orignal_data.m_color.m_startColor.g;
		particleData->m_color.m_startColor.b = tree->m_orignal_data.m_color.m_startColor.b;
		particleData->m_color.m_startColor.a = tree->m_orignal_data.m_color.m_startColor.a;

		particleData->m_color.m_startColorVariance.r = tree->m_orignal_data.m_color.m_startColorVariance.r;
		particleData->m_color.m_startColorVariance.g = tree->m_orignal_data.m_color.m_startColorVariance.g;
		particleData->m_color.m_startColorVariance.b = tree->m_orignal_data.m_color.m_startColorVariance.b;
		particleData->m_color.m_startColorVariance.a = tree->m_orignal_data.m_color.m_startColorVariance.a;

		particleData->m_color.m_startColorResults.r = HpClampf( particleData->m_color.m_startColor.r + (particleData->m_color.m_startColorVariance.r*HPRANDOM_MINUS1_1()),0,255.0f) ;
		particleData->m_color.m_startColorResults.g = HpClampf( particleData->m_color.m_startColor.g + (particleData->m_color.m_startColorVariance.g*HPRANDOM_MINUS1_1()),0,255.0f) ;
		particleData->m_color.m_startColorResults.b = HpClampf( particleData->m_color.m_startColor.b + (particleData->m_color.m_startColorVariance.b*HPRANDOM_MINUS1_1()),0,255.0f) ;
		particleData->m_color.m_startColorResults.a = HpClampf( particleData->m_color.m_startColor.a + (particleData->m_color.m_startColorVariance.a*HPRANDOM_MINUS1_1()),0,255.0f) ;

		//*	结束颜色
		particleData->m_color.m_endColor.r = tree->m_orignal_data.m_color.m_endColor.r;
		particleData->m_color.m_endColor.g = tree->m_orignal_data.m_color.m_endColor.g;
		particleData->m_color.m_endColor.b = tree->m_orignal_data.m_color.m_endColor.b;
		particleData->m_color.m_endColor.a = tree->m_orignal_data.m_color.m_endColor.a;

		particleData->m_color.m_endColorVariance.r = tree->m_orignal_data.m_color.m_endColorVariance.r;
		particleData->m_color.m_endColorVariance.g = tree->m_orignal_data.m_color.m_endColorVariance.g;
		particleData->m_color.m_endColorVariance.b = tree->m_orignal_data.m_color.m_endColorVariance.b;
		particleData->m_color.m_endColorVariance.a = tree->m_orignal_data.m_color.m_endColorVariance.a;

		particleData->m_color.m_endColorResults.r = HpClampf( particleData->m_color.m_endColor.r + (particleData->m_color.m_endColorVariance.r*HPRANDOM_MINUS1_1()),0,255.0f) ;
		particleData->m_color.m_endColorResults.g = HpClampf( particleData->m_color.m_endColor.g + (particleData->m_color.m_endColorVariance.g*HPRANDOM_MINUS1_1()),0,255.0f) ;
		particleData->m_color.m_endColorResults.b = HpClampf( particleData->m_color.m_endColor.b + (particleData->m_color.m_endColorVariance.b*HPRANDOM_MINUS1_1()),0,255.0f) ;
		particleData->m_color.m_endColorResults.a = HpClampf( particleData->m_color.m_endColor.a + (particleData->m_color.m_endColorVariance.a*HPRANDOM_MINUS1_1()),0,255.0f) ;

		//*	颜色变化率
		particleData->m_color.m_colorGradualAccel.r = (particleData->m_color.m_endColorResults.r-particleData->m_color.m_startColorResults.r) / particleData->m_lifeSpanResults ;
		particleData->m_color.m_colorGradualAccel.g = (particleData->m_color.m_endColorResults.g-particleData->m_color.m_startColorResults.g) / particleData->m_lifeSpanResults ;
		particleData->m_color.m_colorGradualAccel.b = (particleData->m_color.m_endColorResults.b-particleData->m_color.m_startColorResults.b) / particleData->m_lifeSpanResults ;
		particleData->m_color.m_colorGradualAccel.a = (particleData->m_color.m_endColorResults.a-particleData->m_color.m_startColorResults.a) / particleData->m_lifeSpanResults ;

		//!	尺寸变化
		//	end size 横向
		particleData->m_size.m_startSizeHorizontal = tree->m_orignal_data.m_size.m_startSizeHorizontal;
		particleData->m_size.m_startSizeHorizontalVariance = tree->m_orignal_data.m_size.m_startSizeHorizontalVariance;
		particleData->m_size.m_startSizeHorizontalResults = particleData->m_size.m_startSizeHorizontal + ( particleData->m_size.m_startSizeHorizontalVariance*HPRANDOM_MINUS1_1() ) ;

		particleData->m_size.m_endSizeHorizontal = tree->m_orignal_data.m_size.m_endSizeHorizontal;
		particleData->m_size.m_endSizeHorizontalVariance = tree->m_orignal_data.m_size.m_endSizeHorizontalVariance;
		particleData->m_size.m_endSizeHorizontalResults = particleData->m_size.m_endSizeHorizontal + ( particleData->m_size.m_endSizeHorizontalVariance*HPRANDOM_MINUS1_1() ) ;
		//	横向变化率
		particleData->m_size.m_sizeHorizontalGradualAccel = (particleData->m_size.m_endSizeHorizontalResults-particleData->m_size.m_startSizeHorizontalResults) / particleData->m_lifeSpanResults ;

		//	start size 纵向
		particleData->m_size.m_startSizeVertical = tree->m_orignal_data.m_size.m_startSizeVertical;
		particleData->m_size.m_startSizeVerticalVariance = tree->m_orignal_data.m_size.m_startSizeVerticalVariance ;
		particleData->m_size.m_startSizeVerticalResults = particleData->m_size.m_startSizeVertical + ( particleData->m_size.m_startSizeVerticalVariance*HPRANDOM_MINUS1_1() ) ;
		//particleData->m_size.m_startSizeVerticalResults = MAX(0, particleData->m_size.m_startSizeVerticalResults);
		//	end size 纵向
		particleData->m_size.m_endSizeVertical = tree->m_orignal_data.m_size.m_endSizeVertical;
		particleData->m_size.m_endSizeVerticalVariance = tree->m_orignal_data.m_size.m_endSizeVerticalVariance; ;
		particleData->m_size.m_endSizeVerticalResults = particleData->m_size.m_endSizeVertical + ( particleData->m_size.m_endSizeVerticalVariance*HPRANDOM_MINUS1_1() ) ;
		//particleData->m_size.m_endSizeVerticalResults = MAX(0, particleData->m_size.m_endSizeVerticalResults);
		//	纵向变化率
		particleData->m_size.m_sizeVerticalGradualAccel = (particleData->m_size.m_endSizeVerticalResults-particleData->m_size.m_startSizeVerticalResults) / particleData->m_lifeSpanResults ;

		//*	喷口
		particleData->m_emitter.startAngle = tree->m_orignal_data.m_emitter.startAngle ;
		particleData->m_emitter.angleVariance = tree->m_orignal_data.m_emitter.angleVariance ;
		if ( 0 != particleData->m_emitter.angleVariance ) {
			particleData->m_emitter.startAngleResults = (float )(rand()%(int)particleData->m_emitter.angleVariance)+particleData->m_emitter.startAngle ; 	
		} else {
			particleData->m_emitter.startAngleResults = particleData->m_emitter.startAngle ; 	
		}
		particleData->m_emitter.startAngleResults = (float )((int )particleData->m_emitter.startAngleResults % 360) ;
		particleData->m_emitter.startAngleResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_emitter.startAngleResults) ;

		//*	自转
		particleData->m_rotation.m_startAngle = tree->m_orignal_data.m_rotation.m_startAngle ;
		particleData->m_rotation.m_startAngleVariance = tree->m_orignal_data.m_rotation.m_startAngleVariance ;
		particleData->m_rotation.m_startAngleResults = particleData->m_rotation.m_startAngle + ( particleData->m_rotation.m_startAngleVariance*HPRANDOM_MINUS1_1() ) ; 

		particleData->m_rotation.m_endAngle = tree->m_orignal_data.m_rotation.m_endAngle ;
		particleData->m_rotation.m_endAngleVariance = tree->m_orignal_data.m_rotation.m_endAngleVariance ;
		particleData->m_rotation.m_endAngleResults = particleData->m_rotation.m_endAngle + ( particleData->m_rotation.m_endAngleVariance*HPRANDOM_MINUS1_1() ) ; 

		particleData->m_rotation.m_angularAccel = ( particleData->m_rotation.m_endAngleResults - particleData->m_rotation.m_startAngleResults ) / particleData->m_lifeSpanResults ;
		//*	转弧度
		particleData->m_rotation.m_startAngleResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_rotation.m_startAngleResults) ;
		particleData->m_rotation.m_endAngleResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_rotation.m_endAngleResults) ;

		//*	加速度
		//particleData->m_physicsObject = et->m_HpWorld->createObject( (void* )particleData ) ;
		particleData->m_force.init ( tree->m_orignal_data.m_force.m_x , tree->m_orignal_data.m_force.m_y ) ;
		
		particleData->m_friction = tree->m_orignal_data.m_friction ;
		particleData->m_mass = tree->m_orignal_data.m_mass ;

		particleData->m_revolution.m_angularSpeed = tree->m_orignal_data.m_revolution.m_angularSpeed ;
		particleData->m_revolution.m_angularSpeedVariance = tree->m_orignal_data.m_revolution.m_angularSpeedVariance ;
		particleData->m_revolution.m_angularSpeedResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_revolution.m_angularSpeed + particleData->m_revolution.m_angularSpeedVariance * HPRANDOM_MINUS1_1());
		particleData->m_revolution.m_deltaAngle = particleData->m_emitter.startAngleResults ;

		particleData->m_revolution.m_startRadius = tree->m_orignal_data.m_revolution.m_startRadius ;
		particleData->m_revolution.m_startRadiusVariance = tree->m_orignal_data.m_revolution.m_startRadiusVariance ;
		particleData->m_revolution.m_startRadiusResults = particleData->m_revolution.m_startRadius + (particleData->m_revolution.m_startRadiusVariance*HPRANDOM_MINUS1_1()) ; 

		particleData->m_revolution.m_endRadius = tree->m_orignal_data.m_revolution.m_endRadius ;
		particleData->m_revolution.m_endRadiusVariance = tree->m_orignal_data.m_revolution.m_endRadiusVariance ;
		particleData->m_revolution.m_endRadiusResults = particleData->m_revolution.m_endRadius + (particleData->m_revolution.m_endRadiusVariance*HPRANDOM_MINUS1_1()) ;

		particleData->m_revolution.m_radiusSpeed = (particleData->m_revolution.m_endRadiusResults-particleData->m_revolution.m_startRadiusResults) / particleData->m_lifeSpanResults ;
		particleData->m_revolution.m_deltaRadius = 0.0f ;

		//if ( HpParticleModeGravity == et->m_mode ){
		//	float gravity = (float )tree->m_gravity ;
		//	if ( 0 == gravity ) {
		//		et->m_gravity = 0.001 ;
		//	} else {
		//		et->m_gravity = gravity ;
		//	}
		//} else {
		//	//	cocos2d工具的sourcePositionVariancex只能传1，在此除100
		//	et->m_gravity = 0.001f ;
		//}

		et->m_gravity = (float )tree->m_gravity ;
		if ( 0 == et->m_gravity ) {
			et->m_gravity = 0.000001 ;
		}
		
		et->m_starttime = 0.0f ;
		 
		if ( -1 == et->m_emittDuration ) {
		//	et->m_emittRate = et->m_emittTotall / particleData->m_lifeSpanResults ;
		} else {
		//	et->m_emittRate = et->m_emittTotall / et->m_emittDuration ;
		}
		//et->m_emittRate = et->m_emittTotall / particleData->m_lifeSpanResults ;
		et->m_emittRate = et->m_emittTotall / particleData->m_lifeSpan ;

	

	} 

	return et ;

}

/*
	brief : get an et
*/
HpEmitter* HpEmitterContainer::getEmitter ( const char* file ) 
{
	HpEmitter* et = 0 ;

	for ( SCClList* walker = m_HpEmitterList.next ; walker ; walker = walker->next ) {

		et = (HpEmitter* )walker->element ; 	

		if ( NULL != et ) {
			if ( 0 == strcmp ( et->m_name , file ) ) {
				return et ;
			}
		}

	} 

	return 0 ;

}

/*
	brief : find an et
*/
bool HpEmitterContainer::findEmitter ( HpEmitter* tree ) 
{
	HpEmitter* et = 0 ;

	for ( SCClList* walker = m_HpEmitterList.next ; walker ; walker = walker->next ) {

		et = (HpEmitter* )walker->element ; 	

		if ( NULL != et ) {
			if ( et == tree ) {
				return true ;
			}
		}

	} 

	return false ;
}

/*
	brief : get an et
*/
void HpEmitterContainer::resetEmitter ( HpEmitter* tree ) 
{
	resetEmitterNode ( tree ) ;
}

/*
	brief : copy a emitter and new
*/
HpEmitter* HpEmitterContainer::copyEmitter ( HpEmitter* tree ) 
{

	HpEmitter* pre = 0 ;
	HpEmitter* firstEmitter = 0 ;
	HpEmitter* treeNew = 0 ;
	iU32 totall = 0 ;

	for ( HpEmitter* walker = tree ; walker ; walker = walker->next ) {

		resetEmitterNode ( walker ) ;

		treeNew = (HpEmitter* ) MMC_MALLOC ( sizeof(HpEmitter) ) ;
		if ( NULL == treeNew ) {
			iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
			return 0 ;
		}
		memcpy ( treeNew , walker , sizeof(HpEmitter) ) ;
	
		//	复制的都是非static
		treeNew->m_static = false ;
	
		copyHpeTexture ( &treeNew->m_hpeTexture , &walker->m_hpeTexture ) ;

		//	new Hp world
		treeNew->m_HpWorld = new HpWorld() ;
		if ( NULL == treeNew->m_HpWorld ) {
			iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
			return 0 ;
		}
		if ( NULL != walker->m_HpWorld ) {
			treeNew->m_HpWorld->setGravity ( walker->m_HpWorld->getGravity() ) ;
		}

		//	copy data
		treeNew->m_data = (HpParticle* ) MMC_MALLOC ( (walker->m_emittTotall)*sizeof(HpParticle) ) ;
		if ( NULL == treeNew->m_data ) {
			iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
			return 0 ;
		}
		memcpy ( treeNew->m_data , walker->m_data , walker->m_emittTotall*sizeof(HpParticle) ) ;
		memcpy ( &treeNew->m_orignal_data  , &walker->m_orignal_data , sizeof(HpParticle) ) ;

		if ( 0 == totall ) firstEmitter = treeNew ;
		if ( 0 != pre ) pre->next = treeNew ;
		treeNew->pre = pre ;
		pre = treeNew ;

		totall ++ ;

	}

	return firstEmitter ;

}

/*
	brief : 
*/
void HpEmitterContainer::insertEmitter ( HpEmitter* tree ) 
{
	SCClListInsert ( & m_HpEmitterList , (void* )tree ) ;
}

/*
	brief : 
*/
void HpEmitterContainer::removeEmitter ( HpEmitter* tree ) 
{
	destroyEmitter ( tree ) ;
	SCClListDelete ( &m_HpEmitterList , (void* )tree ) ;
}

/*
	brief : 
*/
void HpEmitterContainer::resetEmitterEx ( ) 
{

	HpEmitter* et = 0 ;

	for ( SCClList* walker = m_HpEmitterList.next ; walker ; walker = walker->next ) {

		et = (HpEmitter* )walker->element ; 	

		if ( NULL != et ) {
			resetEmitter ( et ) ;
		}

	} 
}

/*
	brief : get texture
*/
void* HpEmitterContainer::getTexture ( HpeTexture* source ) {

	//	return the first frame
	return (void* )source->m_framelist[0].m_handle ;

}


/*
	brief : get texture
*/
void* HpEmitterContainer::getTextureAtom ( HpeTexture* source ) {

	HpeTextureAtom* atom = 0 ; 

	atom = (HpeTextureAtom* )&source->m_framelist[0] ;

	return atom ;

}

/*
	brief : get HpeTexture
*/
void* HpEmitterContainer::getTextureHandle ( const char* name ) 
{
	HpeTexture* node = 0 ;

	for ( SCClList* walker = m_HpeTextureList.next ; walker ; walker = walker->next ) {

		node = (HpeTexture* )walker->element ; 

		if ( 0 == node->m_totallFrame ) {
			return 0 ;
		}

		for ( int looper = 0 ; looper < node->m_totallFrame ; looper ++ ) {

			if ( 0 == strcmp ( node->m_framelist[looper].m_name , name ) ) {
					return (void *)node->m_framelist[looper].m_handle ;
			}

		}

	} 

	return 0 ;

}

/*
	brief : insert hpeTexture
*/
void HpEmitterContainer::insertTexture ( HpeTexture* hpeTexture ) 
{
	SCClListInsert ( &m_HpeTextureList , (void* )hpeTexture ) ;
}

/*
	brief : copy hpeTexture
*/
void HpEmitterContainer::copyHpeTexture ( HpeTexture* dest , HpeTexture* source ) 
{
	memcpy ( dest , source , sizeof(HpeTexture) ) ; 
}

/*
	brief : destroy et
*/
void HpEmitterContainer::destroyEmitter ( HpEmitter* et ) 
{

	//	判断父节点是否为static，则整个树都不允许删除
	if ( true == et->m_static ) {
		return ;
	}

	HpEmitter* tree = (HpEmitter* )et ;
	if ( 0 == tree )
		return ;

	if ( NULL != tree->m_HpWorld ) {
		delete tree->m_HpWorld ;
	}
	if ( NULL != tree->m_data ) {
		MMC_FREE(tree->m_data) ;   
	}
	
	//	jelo edited
	//if ( NULL != tree->m_hpeTexture ) {		
	{
	//	end

		for ( int looper = 0 ; looper < tree->m_hpeTexture.m_totallFrame ; looper ++ ) {
	
			HpTexture2D* texture = (HpTexture2D* )tree->m_hpeTexture.m_framelist[looper].m_handle ;
			texture->release () ;
	
		}
	
		//	jelo edited
		//MMC_FREE(tree->m_hpeTexture) ;
		//	end
	}
	MMC_FREE ( tree ) ;

}

/*
	brief : 
*/
void HpEmitterContainer::resetEmitterNode ( HpEmitter* node ) 
{
	node->m_dead = false ;
	node->m_actived = false ;
	node->m_elapsedEmittedTotallTime = 0.0f ;
	node->m_elapsedTotallTime = 0.0f ;
	node->m_emittTotallPerSec = 0.0f ;
	node->m_orgCoordinate.init(0.0,0.0) ;
	node->m_emittPosition.init(0.0,0.0) ;
	node->m_emittCounter = 0 ;
}