
/*
+	HpCacu.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "schal.h"
#include "HpAdapter.h"
#include "HpCacu.h"
#include "HpTexture2D.h"
#include "HpTextureCache.h"

void HpCacuFileGetEmitter ( HpCacuEmitter* emitter , HpEmitter* et ) {

	assert(et&&emitter) ;

	strcpy ( emitter->m_name , et->m_name ) ;
	
	emitter->m_mode = et->m_mode ;
	emitter->m_sizemode = et->m_sizemode ;
	emitter->m_randmode = et->m_randmode ;
	
	emitter->m_starttime = et->m_starttime ;
	emitter->m_emittDuration = et->m_emittDuration ;
	emitter->m_emittTotall = et->m_emittTotall ;
	emitter->m_gravity = et->m_gravity ;

	emitter->m_BlendFuncSrc = et->m_BlendFuncSrc ;
	emitter->m_BlendFuncDst = et->m_BlendFuncDst ;
	memcpy ( &emitter->m_hpeTexture , &et->m_hpeTexture , sizeof(et->m_hpeTexture) ) ;

	emitter->particle.m_lifeSpan = et->m_orignal_data.m_lifeSpan ;
	emitter->particle.m_lifeSpanVariance = et->m_orignal_data.m_lifeSpanVariance ;
	
	emitter->particle.m_color.m_startColor = et->m_orignal_data.m_color.m_startColor ;
	emitter->particle.m_color.m_startColorVariance = et->m_orignal_data.m_color.m_startColorVariance ;
	emitter->particle.m_color.m_endColor = et->m_orignal_data.m_color.m_endColor ;
	emitter->particle.m_color.m_endColorVariance = et->m_orignal_data.m_color.m_endColorVariance ;

	emitter->particle.m_size.m_startSizeHorizontal = et->m_orignal_data.m_size.m_startSizeHorizontal ;
	emitter->particle.m_size.m_startSizeHorizontalVariance = et->m_orignal_data.m_size.m_startSizeHorizontalVariance ;
	emitter->particle.m_size.m_endSizeHorizontal = et->m_orignal_data.m_size.m_endSizeHorizontal ;
	emitter->particle.m_size.m_endSizeHorizontalVariance = et->m_orignal_data.m_size.m_endSizeHorizontalVariance ;

	emitter->particle.m_size.m_startSizeVertical = et->m_orignal_data.m_size.m_startSizeVertical ;
	emitter->particle.m_size.m_startSizeVerticalVariance = et->m_orignal_data.m_size.m_startSizeVerticalVariance ;
	emitter->particle.m_size.m_endSizeVertical = et->m_orignal_data.m_size.m_endSizeVertical ;
	emitter->particle.m_size.m_endSizeVerticalVariance = et->m_orignal_data.m_size.m_endSizeVerticalVariance ;

	emitter->particle.m_emitter.startAngle = et->m_orignal_data.m_emitter.startAngle ;
	emitter->particle.m_emitter.angleVariance = et->m_orignal_data.m_emitter.angleVariance ;

	emitter->particle.m_rotation.m_startAngle = et->m_orignal_data.m_rotation.m_startAngle ;
	emitter->particle.m_rotation.m_startAngleVariance = et->m_orignal_data.m_rotation.m_startAngleVariance ;	
	emitter->particle.m_rotation.m_endAngle = et->m_orignal_data.m_rotation.m_endAngle ;
	emitter->particle.m_rotation.m_endAngleVariance = et->m_orignal_data.m_rotation.m_endAngleVariance ;	

	emitter->particle.m_revolution.m_angularSpeed = et->m_orignal_data.m_revolution.m_angularSpeed ;
	emitter->particle.m_revolution.m_angularSpeedVariance = et->m_orignal_data.m_revolution.m_angularSpeedVariance ;	
	emitter->particle.m_revolution.m_startRadius = et->m_orignal_data.m_revolution.m_startRadius ;
	emitter->particle.m_revolution.m_startRadiusVariance = et->m_orignal_data.m_revolution.m_startRadiusVariance ;	
	emitter->particle.m_revolution.m_endRadius = et->m_orignal_data.m_revolution.m_endRadius ;
	emitter->particle.m_revolution.m_endRadiusVariance = et->m_orignal_data.m_revolution.m_endRadiusVariance ;	

	emitter->particle.m_force = et->m_orignal_data.m_force = et->m_orignal_data.m_force ;	
	emitter->particle.m_friction = et->m_orignal_data.m_friction = et->m_orignal_data.m_friction ;	
	emitter->particle.m_mass = et->m_orignal_data.m_mass = et->m_orignal_data.m_mass ;	

}

HpEmitter* HpCacuFileCacuEmitter2HpEmitter ( HpCacuEmitter* emitter , HpEmitter* et ) {

	assert(et&&emitter) ;

	strcpy ( et->m_name , emitter->m_name ) ;
	
	et->m_mode = emitter->m_mode ;
	et->m_sizemode = emitter->m_sizemode ;
	et->m_randmode = emitter->m_randmode ;
	
	et->m_starttime = emitter->m_starttime ;
	et->m_emittDuration = emitter->m_emittDuration ;
	et->m_emittTotall = emitter->m_emittTotall ;
	et->m_gravity = emitter->m_gravity ;

	et->m_BlendFuncSrc = emitter->m_BlendFuncSrc ;
	et->m_BlendFuncDst = emitter->m_BlendFuncDst ;
	memcpy ( &et->m_hpeTexture , &emitter->m_hpeTexture , sizeof(emitter->m_hpeTexture)) ;

	et->m_orignal_data.m_lifeSpan = emitter->particle.m_lifeSpan ;
	et->m_orignal_data.m_lifeSpanVariance = emitter->particle.m_lifeSpanVariance ;
	
	et->m_orignal_data.m_color.m_startColor = emitter->particle.m_color.m_startColor ;
	et->m_orignal_data.m_color.m_startColorVariance = emitter->particle.m_color.m_startColorVariance ;
	et->m_orignal_data.m_color.m_endColor = emitter->particle.m_color.m_endColor ;
	et->m_orignal_data.m_color.m_endColorVariance = emitter->particle.m_color.m_endColorVariance ;

	et->m_orignal_data.m_size.m_startSizeHorizontal = emitter->particle.m_size.m_startSizeHorizontal ;
	et->m_orignal_data.m_size.m_startSizeHorizontalVariance = emitter->particle.m_size.m_startSizeHorizontalVariance ;
	et->m_orignal_data.m_size.m_endSizeHorizontal = emitter->particle.m_size.m_endSizeHorizontal ;
	et->m_orignal_data.m_size.m_endSizeHorizontalVariance = emitter->particle.m_size.m_endSizeHorizontalVariance ;

	et->m_orignal_data.m_size.m_startSizeVertical = emitter->particle.m_size.m_startSizeVertical ;
	et->m_orignal_data.m_size.m_startSizeVerticalVariance = emitter->particle.m_size.m_startSizeVerticalVariance ;
	et->m_orignal_data.m_size.m_endSizeVertical = emitter->particle.m_size.m_endSizeVertical ;
	et->m_orignal_data.m_size.m_endSizeVerticalVariance = emitter->particle.m_size.m_endSizeVerticalVariance ;

	et->m_orignal_data.m_emitter.startAngle = emitter->particle.m_emitter.startAngle ;
	et->m_orignal_data.m_emitter.angleVariance = emitter->particle.m_emitter.angleVariance ;

	et->m_orignal_data.m_rotation.m_startAngle = emitter->particle.m_rotation.m_startAngle ;
	et->m_orignal_data.m_rotation.m_startAngleVariance = emitter->particle.m_rotation.m_startAngleVariance ;	
	et->m_orignal_data.m_rotation.m_endAngle = emitter->particle.m_rotation.m_endAngle ;
	et->m_orignal_data.m_rotation.m_endAngleVariance = emitter->particle.m_rotation.m_endAngleVariance ;	

	et->m_orignal_data.m_revolution.m_angularSpeed = emitter->particle.m_revolution.m_angularSpeed ;
	et->m_orignal_data.m_revolution.m_angularSpeedVariance = emitter->particle.m_revolution.m_angularSpeedVariance ;	
	et->m_orignal_data.m_revolution.m_startRadius = emitter->particle.m_revolution.m_startRadius ;
	et->m_orignal_data.m_revolution.m_startRadiusVariance = emitter->particle.m_revolution.m_startRadiusVariance ;	
	et->m_orignal_data.m_revolution.m_endRadius = emitter->particle.m_revolution.m_endRadius ;
	et->m_orignal_data.m_revolution.m_endRadiusVariance = emitter->particle.m_revolution.m_endRadiusVariance ;	

	et->m_orignal_data.m_force = emitter->particle.m_force = emitter->particle.m_force ;	
	et->m_orignal_data.m_friction = emitter->particle.m_friction = emitter->particle.m_friction ;	
	et->m_orignal_data.m_mass = emitter->particle.m_mass = emitter->particle.m_mass ;	

	return et ;

}

void HpCacuFileHpEmitter2CacuEmitter ( HpCacuEmitter* et , HpEmitter* emitter ) {

	assert(et&&emitter) ;

	strcpy ( et->m_name , emitter->m_name ) ;
	
	et->m_mode = emitter->m_mode ;
	et->m_sizemode = emitter->m_sizemode ;
	et->m_randmode = emitter->m_randmode ;
	
	et->m_starttime = emitter->m_starttime ;
	et->m_emittDuration = emitter->m_emittDuration ;
	et->m_emittTotall = emitter->m_emittTotall ;
	et->m_gravity = emitter->m_gravity ;

	et->m_BlendFuncSrc = emitter->m_BlendFuncSrc ;
	et->m_BlendFuncDst = emitter->m_BlendFuncDst ;
	memcpy ( &et->m_hpeTexture , &emitter->m_hpeTexture , sizeof(emitter->m_hpeTexture)) ;

	et->particle.m_lifeSpan = emitter->m_orignal_data.m_lifeSpan ;
	et->particle.m_lifeSpanVariance = emitter->m_orignal_data.m_lifeSpanVariance ;
	
	et->particle.m_color.m_startColor = emitter->m_orignal_data.m_color.m_startColor ;
	et->particle.m_color.m_startColorVariance = emitter->m_orignal_data.m_color.m_startColorVariance ;
	et->particle.m_color.m_endColor = emitter->m_orignal_data.m_color.m_endColor ;
	et->particle.m_color.m_endColorVariance = emitter->m_orignal_data.m_color.m_endColorVariance ;

	et->particle.m_size.m_startSizeHorizontal = emitter->m_orignal_data.m_size.m_startSizeHorizontal ;
	et->particle.m_size.m_startSizeHorizontalVariance = emitter->m_orignal_data.m_size.m_startSizeHorizontalVariance ;
	et->particle.m_size.m_endSizeHorizontal = emitter->m_orignal_data.m_size.m_endSizeHorizontal ;
	et->particle.m_size.m_endSizeHorizontalVariance = emitter->m_orignal_data.m_size.m_endSizeHorizontalVariance ;

	et->particle.m_size.m_startSizeVertical = emitter->m_orignal_data.m_size.m_startSizeVertical ;
	et->particle.m_size.m_startSizeVerticalVariance = emitter->m_orignal_data.m_size.m_startSizeVerticalVariance ;
	et->particle.m_size.m_endSizeVertical = emitter->m_orignal_data.m_size.m_endSizeVertical ;
	et->particle.m_size.m_endSizeVerticalVariance = emitter->m_orignal_data.m_size.m_endSizeVerticalVariance ;

	et->particle.m_emitter.startAngle = emitter->m_orignal_data.m_emitter.startAngle ;
	et->particle.m_emitter.angleVariance = emitter->m_orignal_data.m_emitter.angleVariance ;

	et->particle.m_rotation.m_startAngle = emitter->m_orignal_data.m_rotation.m_startAngle ;
	et->particle.m_rotation.m_startAngleVariance = emitter->m_orignal_data.m_rotation.m_startAngleVariance ;	
	et->particle.m_rotation.m_endAngle = emitter->m_orignal_data.m_rotation.m_endAngle ;
	et->particle.m_rotation.m_endAngleVariance = emitter->m_orignal_data.m_rotation.m_endAngleVariance ;	

	et->particle.m_revolution.m_angularSpeed = emitter->m_orignal_data.m_revolution.m_angularSpeed ;
	et->particle.m_revolution.m_angularSpeedVariance = emitter->m_orignal_data.m_revolution.m_angularSpeedVariance ;	
	et->particle.m_revolution.m_startRadius = emitter->m_orignal_data.m_revolution.m_startRadius ;
	et->particle.m_revolution.m_startRadiusVariance = emitter->m_orignal_data.m_revolution.m_startRadiusVariance ;	
	et->particle.m_revolution.m_endRadius = emitter->m_orignal_data.m_revolution.m_endRadius ;
	et->particle.m_revolution.m_endRadiusVariance = emitter->m_orignal_data.m_revolution.m_endRadiusVariance ;	

	et->particle.m_force = emitter->m_orignal_data.m_force  ;	
	et->particle.m_friction = emitter->m_orignal_data.m_friction ;	
	et->particle.m_mass = emitter->m_orignal_data.m_mass ;	

}


void HpCacuFileSave ( const iS8* file , SCClList* emitterListIn ) 
{
	HpCacuFileHeader header = { 0 } ;
	HpCacuEmitter emitter = { 0 } ;
	void* fp = SCHalFileOpen ( file , "wb+" ) ;
	assert(file) ;

	strcpy ( header.magic , HpCacuFileMagic ) ;
	header.version = HpCacuFileVersion ;
	header.emitterOffset = sizeof( HpCacuFileHeader) ;
	SCHalFileWrite ( fp , &header , sizeof(HpCacuFileHeader) ) ;

	for ( SCClList* walker = emitterListIn->next ; walker ; walker = walker->next ) { 
		HpEmitter* et = (HpEmitter* )walker->element ;
		HpCacuFileHpEmitter2CacuEmitter ( &emitter , et ) ;
		SCHalFileWrite ( fp , &emitter , sizeof(HpCacuEmitter) ) ;
		header.totallEmitter ++ ;
	}
	SCHalFileSeek ( fp , 0 , SEEK_SET ) ;
	SCHalFileWrite ( fp , &header , sizeof(HpCacuFileHeader) ) ;
	
	SCHalFileClose ( fp ) ;

}

void HpCacuFileLoad ( const iS8* file , SCClList* emitterListOut )
{
	HpCacuFileHeader header = { 0 } ;
	HpCacuEmitter emitter = { 0 } ;
	void* fp = SCHalFileOpen ( file , "rb" ) ;
	
	if ( 0 == fp ) {
		iLog ( "file %s is not exist" , file ) ;
		return ;
	}

	SCHalFileRead ( fp , &header , sizeof(HpCacuFileHeader) ) ;

	if ( 0 != strcmp ( header.magic , HpCacuFileMagic ) ) {
		SCHalFileClose ( fp ) ;
		iLog ( "file %s is not legal" , file ) ;
		return ; 
	}

	if ( header.version > HpCacuFileVersion ) {
		SCHalFileClose ( fp ) ;
		iLog ( "file %s version %d is not legal" , file , header.version ) ;
		return ;
	}
	SCHalFileSeek ( fp , header.emitterOffset , SEEK_SET ) ;

	for ( iU32 looper = 0 ; looper < header.totallEmitter ; looper ++ ) 
	{
		HpCacuEmitter emitter = { 0 } ;
		HpEmitter* et = (HpEmitter* )MMC_MALLOC ( sizeof(HpEmitter) ) ;
		assert(et) ;
		SCHalFileRead ( fp , &emitter , sizeof(emitter) ) ;
		HpCacuFileCacuEmitter2HpEmitter ( &emitter , et ) ;

		for ( iU32 counter = 0 ; counter < emitter.m_hpeTexture.m_totallFrame ; counter ++ ) {
			HpTexture2D* texture = 0 ;
			texture = HpTextureCache::getInstance()->getTexture ( 
				H3UtilString2Number(et->m_hpeTexture.m_framelist[counter].m_name).getValue() ) ;

			if ( 0 == texture ) {
				texture = MMC_NEW HpTexture2D () ;
				bool backup = SCHalGetAbsoluteMode () ;
				SCHalSetAbsoluteMode ( false ) ;
				texture->initWithPNGFile ( et->m_hpeTexture.m_framelist[counter].m_name ) ;
				SCHalSetAbsoluteMode ( backup ) ;
				HpTextureCache::getInstance()->addTexture (
					H3UtilString2Number(et->m_hpeTexture.m_framelist[counter].m_name).getValue() ,
					texture 
				) ;			
			}	
			texture->addRef () ;

			et->m_hpeTexture.m_framelist[counter].m_widthInPixels = texture->getContentSize().width ;
			et->m_hpeTexture.m_framelist[counter].m_heightInPixels = texture->getContentSize().height ;
			et->m_hpeTexture.m_framelist[counter].m_rect.x1 = 0 ;
			et->m_hpeTexture.m_framelist[counter].m_rect.y1 = 0 ;
			et->m_hpeTexture.m_framelist[counter].m_rect.x2 = et->m_hpeTexture.m_framelist[counter].m_widthInPixels ;
			et->m_hpeTexture.m_framelist[counter].m_rect.y2 = et->m_hpeTexture.m_framelist[counter].m_heightInPixels ;
			et->m_hpeTexture.m_framelist[counter].m_handle = (long long )texture ;
			et->m_hpeTexture.m_framelist[counter].m_id = (long long )texture->getId () ;			

		}

		SCClListInsert ( emitterListOut , et ) ;
	}

	SCHalFileClose ( fp ) ;

}

//	load previous version
HpEmitter* HpCacuFileCacuEmitter2HpEmitter0x0 ( HpCacuEmitter0x0* emitter , HpEmitter* et ) {

	assert(et&&emitter) ;

	strcpy ( et->m_name , emitter->m_name ) ;
	
	et->m_mode = emitter->m_mode ;
	et->m_sizemode = emitter->m_sizemode ;
	et->m_randmode = emitter->m_randmode ;
	
	et->m_starttime = emitter->m_starttime ;
	et->m_emittDuration = emitter->m_emittDuration ;
	et->m_emittTotall = emitter->m_emittTotall ;
	et->m_gravity = emitter->m_gravity ;

	et->m_BlendFuncSrc = emitter->m_BlendFuncSrc ;
	et->m_BlendFuncDst = emitter->m_BlendFuncDst ;
	
	et->m_hpeTexture.m_totallFrame = emitter->m_hpeTexture.m_totallFrame ;
	for ( int looper = 0 ; looper < et->m_hpeTexture.m_totallFrame ; looper ++ ) {
		
		strcpy ( et->m_hpeTexture.m_framelist[looper].m_name , emitter->m_hpeTexture.m_framelist[looper].m_name ) ;

		et->m_hpeTexture.m_framelist[looper].m_handle = (long long )emitter->m_hpeTexture.m_framelist[looper].m_handle ;
		et->m_hpeTexture.m_framelist[looper].m_id = (long long )emitter->m_hpeTexture.m_framelist[looper].m_id ;
		et->m_hpeTexture.m_framelist[looper].m_duration = emitter->m_hpeTexture.m_framelist[looper].m_duration ;
		et->m_hpeTexture.m_framelist[looper].m_rect.x1 = emitter->m_hpeTexture.m_framelist[looper].m_rect.x1 ;
		et->m_hpeTexture.m_framelist[looper].m_rect.y1 = emitter->m_hpeTexture.m_framelist[looper].m_rect.y1 ;
		et->m_hpeTexture.m_framelist[looper].m_rect.x2 = emitter->m_hpeTexture.m_framelist[looper].m_rect.x2 ;
		et->m_hpeTexture.m_framelist[looper].m_rect.y2 = emitter->m_hpeTexture.m_framelist[looper].m_rect.y2 ;
		et->m_hpeTexture.m_framelist[looper].m_widthInPixels = emitter->m_hpeTexture.m_framelist[looper].m_widthInPixels ; 
		et->m_hpeTexture.m_framelist[looper].m_heightInPixels = emitter->m_hpeTexture.m_framelist[looper].m_heightInPixels ;

	}

	if ( emitter->m_hpeTexture.m_totallFrame > 1 ) {
		et->m_hpeTexture.frameLoop = 0 ;
	} else {
		et->m_hpeTexture.frameLoop = 1 ;
	}

	et->m_orignal_data.m_lifeSpan = emitter->particle.m_lifeSpan ;
	et->m_orignal_data.m_lifeSpanVariance = emitter->particle.m_lifeSpanVariance ;
	
	et->m_orignal_data.m_color.m_startColor = emitter->particle.m_color.m_startColor ;
	et->m_orignal_data.m_color.m_startColorVariance = emitter->particle.m_color.m_startColorVariance ;
	et->m_orignal_data.m_color.m_endColor = emitter->particle.m_color.m_endColor ;
	et->m_orignal_data.m_color.m_endColorVariance = emitter->particle.m_color.m_endColorVariance ;

	et->m_orignal_data.m_size.m_startSizeHorizontal = emitter->particle.m_size.m_startSizeHorizontal ;
	et->m_orignal_data.m_size.m_startSizeHorizontalVariance = emitter->particle.m_size.m_startSizeHorizontalVariance ;
	et->m_orignal_data.m_size.m_endSizeHorizontal = emitter->particle.m_size.m_endSizeHorizontal ;
	et->m_orignal_data.m_size.m_endSizeHorizontalVariance = emitter->particle.m_size.m_endSizeHorizontalVariance ;

	et->m_orignal_data.m_size.m_startSizeVertical = emitter->particle.m_size.m_startSizeVertical ;
	et->m_orignal_data.m_size.m_startSizeVerticalVariance = emitter->particle.m_size.m_startSizeVerticalVariance ;
	et->m_orignal_data.m_size.m_endSizeVertical = emitter->particle.m_size.m_endSizeVertical ;
	et->m_orignal_data.m_size.m_endSizeVerticalVariance = emitter->particle.m_size.m_endSizeVerticalVariance ;

	et->m_orignal_data.m_emitter.startAngle = emitter->particle.m_emitter.startAngle ;
	et->m_orignal_data.m_emitter.angleVariance = emitter->particle.m_emitter.angleVariance ;

	et->m_orignal_data.m_rotation.m_startAngle = emitter->particle.m_rotation.m_startAngle ;
	et->m_orignal_data.m_rotation.m_startAngleVariance = emitter->particle.m_rotation.m_startAngleVariance ;	
	et->m_orignal_data.m_rotation.m_endAngle = emitter->particle.m_rotation.m_endAngle ;
	et->m_orignal_data.m_rotation.m_endAngleVariance = emitter->particle.m_rotation.m_endAngleVariance ;	

	et->m_orignal_data.m_revolution.m_angularSpeed = emitter->particle.m_revolution.m_angularSpeed ;
	et->m_orignal_data.m_revolution.m_angularSpeedVariance = emitter->particle.m_revolution.m_angularSpeedVariance ;	
	et->m_orignal_data.m_revolution.m_startRadius = emitter->particle.m_revolution.m_startRadius ;
	et->m_orignal_data.m_revolution.m_startRadiusVariance = emitter->particle.m_revolution.m_startRadiusVariance ;	
	et->m_orignal_data.m_revolution.m_endRadius = emitter->particle.m_revolution.m_endRadius ;
	et->m_orignal_data.m_revolution.m_endRadiusVariance = emitter->particle.m_revolution.m_endRadiusVariance ;	

	et->m_orignal_data.m_force = emitter->particle.m_force = emitter->particle.m_force ;	
	et->m_orignal_data.m_friction = emitter->particle.m_friction = emitter->particle.m_friction ;	
	et->m_orignal_data.m_mass = emitter->particle.m_mass = emitter->particle.m_mass ;	

	return et ;

}

void HpCacuFileLoad0x0 ( const iS8* file , SCClList* emitterListOut )
{
	HpCacuFileHeader header = { 0 } ;
	HpCacuEmitter0x0 emitter = { 0 } ;
	void* fp = SCHalFileOpen ( file , "rb" ) ;
	
	if ( 0 == fp ) {
		iLog ( "file %s is not exist" , file ) ;
		return ;
	}

	SCHalFileRead ( fp , &header , sizeof(HpCacuFileHeader) ) ;

	if ( 0 != strcmp ( header.magic , HpCacuFileMagic ) ) {
		SCHalFileClose ( fp ) ;
		iLog ( "file %s is not legal" , file ) ;
		return ; 
	}

	if ( header.version > HpCacuFileVersion ) {
		SCHalFileClose ( fp ) ;
		iLog ( "file %s version %d is not legal" , file , header.version ) ;
		return ;
	}
	SCHalFileSeek ( fp , header.emitterOffset , SEEK_SET ) ;

	for ( iU32 looper = 0 ; looper < header.totallEmitter ; looper ++ ) 
	{
		HpCacuEmitter0x0 emitter = { 0 } ;
		HpEmitter* et = (HpEmitter* )MMC_MALLOC ( sizeof(HpEmitter) ) ;
		assert(et) ;
		SCHalFileRead ( fp , &emitter , sizeof(emitter) ) ;
		HpCacuFileCacuEmitter2HpEmitter0x0 ( &emitter , et ) ;

		for ( iU32 counter = 0 ; counter < emitter.m_hpeTexture.m_totallFrame ; counter ++ ) {
			HpTexture2D* texture = 0 ;
			texture = HpTextureCache::getInstance()->getTexture ( 
				H3UtilString2Number(et->m_hpeTexture.m_framelist[counter].m_name).getValue() ) ;

			if ( 0 == texture ) {
				texture = MMC_NEW HpTexture2D () ;
				bool backup = SCHalGetAbsoluteMode () ;
				SCHalSetAbsoluteMode ( false ) ;
				texture->initWithPNGFile ( et->m_hpeTexture.m_framelist[counter].m_name ) ;
				SCHalSetAbsoluteMode ( backup ) ;
				HpTextureCache::getInstance()->addTexture (
					H3UtilString2Number(et->m_hpeTexture.m_framelist[counter].m_name).getValue() ,
					texture 
				) ;			
			}	
			texture->addRef () ;

			et->m_hpeTexture.m_framelist[counter].m_widthInPixels = texture->getContentSize().width ;
			et->m_hpeTexture.m_framelist[counter].m_heightInPixels = texture->getContentSize().height ;
			et->m_hpeTexture.m_framelist[counter].m_rect.x1 = 0 ;
			et->m_hpeTexture.m_framelist[counter].m_rect.y1 = 0 ;
			et->m_hpeTexture.m_framelist[counter].m_rect.x2 = et->m_hpeTexture.m_framelist[counter].m_widthInPixels ;
			et->m_hpeTexture.m_framelist[counter].m_rect.y2 = et->m_hpeTexture.m_framelist[counter].m_heightInPixels ;
			et->m_hpeTexture.m_framelist[counter].m_handle = (long long )texture ;
			et->m_hpeTexture.m_framelist[counter].m_id = (long long )texture->getId () ;			

		}

		SCClListInsert ( emitterListOut , et ) ;
	}

	SCHalFileClose ( fp ) ;

}