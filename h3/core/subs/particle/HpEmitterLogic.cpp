
/*
+	HpEmitterLogic.cpp
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "HpMath.h"
#include "HpAdapter.h"
#include "HpGeometry.h"
#include "HpEmitterLogic.h"
#include "HpThread.h"

#ifdef HP_USE_THREAD
static HpThreadPool gThreadPool ;
#endif

HpEmitterLogic::HpEmitterLogic () 
{
	m_dt = 0.0f ;
	   
	SCClListInit( &m_HpEmitterListSccl ) ;
	SCClListInit( &m_HpEmitterForceList ) ;
	SCClListInit( &m_HpEmitterRadialForceList ) ;
	#ifdef HP_USE_THREAD
	gThreadPool.init(30) ;
	#endif
	   
}  

HpEmitterLogic::~HpEmitterLogic () 
{
	SCClListClear( &m_HpEmitterListSccl ) ;
	SCClListClear( &m_HpEmitterForceList ) ;
	SCClListClear( &m_HpEmitterRadialForceList ) ;
	#ifdef HP_USE_THREAD
	gThreadPool.exit () ;
	#endif
}

/*
	brief : 
*/
void HpEmitterLogic::insertEmitter ( HpEmitter* et ) 
{
	SCClListInsert( &m_HpEmitterListSccl , et ) ;
}

/*
	brief : 
*/
void HpEmitterLogic::removeEmitter ( HpEmitter* et ) 
{
	//	logic 不需要删除树，因为结构存放在et container中，在et container删除结构
	//	这里只需要删除父节点，这里删除的一定是et->m_layer == 0的节点
	SCClListDelete( &m_HpEmitterListSccl , et ) ;
}

/*
	brief : 
*/
void HpEmitterLogic::setEmitterPosition ( HpEmitter* et , HpVector position ) 
{

	HpEmitter* tree ;

	//	原点位置
	HpVector substract = HpVector::sub(position,et->m_emittPosition) ;
	et->m_emittPosition = HpVector::add(et->m_emittPosition,substract) ;

}

/*
	brief : 
*/
void HpEmitterLogic::setEmitterPositionEx ( HpEmitter* et , HpVector position ) 
{

	HpEmitter* tree ;

	//	原点位置
	HpVector substract = HpVector::sub(position,et->m_orgCoordinate) ;
	et->m_orgCoordinate = HpVector::add(et->m_orgCoordinate,substract) ;
	et->m_HpWorld->setOrgCoordinate ( et->m_orgCoordinate ) ;

}

/*
	brief : 
*/
void HpEmitterLogic::setEmitterCallback ( HpParticleSystem* target , HpCB_EMITTER callback ) 
{
	m_callback = callback ;
	m_callback_target = target ;
}

/*
	brief : set radial force
	param : 
*/
void HpEmitterLogic::applyRadialForce ( HpVector position , float radius ) 
{
	HpEmitterRadialForce* radial = (HpEmitterRadialForce* )MMC_MALLOC ( sizeof(HpEmitterRadialForce) ) ;
	assert(radial) ;
	radial->m_position = position ;
	radial->m_radius = radius ;
	SCClListInsert ( &m_HpEmitterRadialForceList , (void* )radial ) ;
}

/*
	brief : set force
	param : 
*/
void HpEmitterLogic::applyForce ( HpVector position , float force ) 
{
	HpEmitterForce* eforce = (HpEmitterForce* )MMC_MALLOC ( sizeof(HpEmitterForce) ) ;
	assert(eforce) ;
	eforce->m_position = position ;
	eforce->m_force = force ;
	SCClListInsert ( &m_HpEmitterForceList , (void* )eforce ) ;
}

/*
	brief : 
*/
void HpEmitterLogic::update ( float dt ) 
{

	this->m_dt = dt ;

	for ( SCClList* walker = m_HpEmitterListSccl.next ; walker ; walker = walker->next ) {

		HpEmitter* node = 0 ;
		node = (HpEmitter* )walker->element ;

		if ( false == node->m_actived ) { 
			continue ;
		}

		if ( true == node->m_dead ) {
			continue ;
		}

		walkEmitter ( node ) ;

	}

	SCClListClear ( &m_HpEmitterForceList ) ;
	SCClListClear ( &m_HpEmitterRadialForceList ) ;

}

/*
	brief : 
*/
void HpEmitterLogic::walkEmitter ( HpEmitter* et ) 
{
	if ( 0 == et ) {
		return ;
	}

	updateEmitter ( et ) ;

}

/*
	brief : 
*/
void HpEmitterLogic::updateEmitter ( HpEmitter* et ) 
{
	
	et->m_elapsedTotallTime = et->m_elapsedTotallTime + this->m_dt ;

	if ( 0 != et->m_starttime ) {
		//	not the time
		if ( et->m_starttime > et->m_elapsedTotallTime ) {
			return ;
		}
	} 

	et->m_elapsedEmittedTotallTime = et->m_elapsedEmittedTotallTime + this->m_dt ;

	if ( et->m_elapsedEmittedTotallTime >= et->m_emittDuration 
		&& et->m_emittDuration != HpParticleModeInfinite ) {

		// update the last particle
		updateParticle ( et ) ;

		//	stop system
		if ( 0 == et->m_emittCounter ) {
			if ( NULL != m_callback ) {
				(m_callback_target->*m_callback)(HpeLogicStatus_Emitter_Dead,et);
				et->m_dead = true ;
			}
			return ;
		}			

	} else {		
		activeParticle ( et ) ; 
		updateParticle ( et ) ;			
	}
	
	//	some particle has dead here maybe , we skip here
	//et->m_HpWorld->run (this->m_dt) ;
	 
 }

/*
	brief : 
*/
void HpEmitterLogic::activeParticle ( HpEmitter* et ) 
{
	HpParticle* particleData = 0 ;

	// 求每毫秒速率
	float rate = 1.0f / et->m_emittRate ;
	et->m_emittTotallPerSec = et->m_emittTotallPerSec + this->m_dt ;

	while ( et->m_emittTotallPerSec > rate && et->m_emittCounter < et->m_emittTotall ) {

		particleData = &et->m_data[et->m_emittCounter] ;
	
		initParticle ( et , particleData ) ;

		//!	放入物理引擎
		//!	计算加速度
		et->m_HpWorld->scaleObjectVelocity ( particleData->m_physicsObject , 0.0f ) ;			

		//!	调整喷口位置，已喷发的粒子不更新位置
		if ( HpParticleModeGravity == et->m_mode ) {
			particleData->m_deltaPosition = et->m_emittPosition ;
			et->m_HpWorld->setObjectPosition ( particleData->m_physicsObject , particleData->m_deltaPosition ) ;				
			et->m_HpWorld->applyForce( particleData->m_physicsObject , particleData->m_force ) ;
			et->m_HpWorld->rotateObjectHeadDirection ( particleData->m_physicsObject , particleData->m_emitter.startAngleResults ) ;
		} else if (HpParticleModeRadius == et->m_mode) {
			particleData->m_deltaPosition.init ( 0.0f , 0.0f ) ;
			et->m_HpWorld->setObjectPosition ( particleData->m_physicsObject , (particleData->m_deltaPosition) ) ;
		}

		et->m_emittTotallPerSec = et->m_emittTotallPerSec - rate ;
		et->m_emittCounter ++ ;				

	}

}

/*
	brief : 
*/
void HpEmitterLogic::updateParticle ( HpEmitter* et ) 
{

	HpParticle* particleData = 0 ;

	//* 更新已经喷发的粒子
	for ( int counter = 0 ; counter < et->m_emittCounter ; counter ++ ) {	

		particleData = &et->m_data[counter] ;
		//*	m_elapsedTime累加放在更新粒子状态之间，可以过滤一些生命周期非常小的粒子，比如周期小于dt的粒子
		//*	否则，(e-s) / lifespan = a , m_elapsedTime*a 会溢出，最终值远大于或远小于(e-s)，如此很合理，因为小于生命周期dt的粒子，在(0,dt)时间内已经死亡，不需要显示。
		particleData->m_elapsedTime = particleData->m_elapsedTime + this->m_dt ;

		//*	过滤
		if ( (particleData->m_elapsedTime >= particleData->m_lifeSpanResults) ) {
	
			HpParticle swapdata ;				
	
			//*	前后换，数据不能清除，因为粒子死亡只是时间重置，其它数据还需要
			swapdata = et->m_data[counter] ;
			et->m_data[counter] = et->m_data[et->m_emittCounter-1] ;
			et->m_data[et->m_emittCounter-1] = swapdata ;

			//*	死亡
			et->m_data[et->m_emittCounter-1].m_elapsedTime = 0.0f ;
			et->m_data[et->m_emittCounter-1].m_revolution.m_deltaAngle = 0.0f ;
			et->m_data[et->m_emittCounter-1].m_revolution.m_deltaRadius = 0.0f ;

			particleData = &et->m_data[counter] ;	
			et->m_emittCounter-- ;	

		}

		#ifdef HP_USE_THREAD
		gThreadPool.addTask( this , (HpCB_ThreadTask)&HpEmitterLogic::caculateParticle, et, particleData ) ;
		#else
		caculateParticle (et,particleData) ;
		#endif
		
	}
	  

}

/*
brief : caculate a particle
*/
void HpEmitterLogic::caculateParticle ( HpEmitter* et , HpParticle* particleData )
{


	//	workthrough force
	for (SCClList* walker = m_HpEmitterForceList.next; walker; walker = walker->next) {
		HpEmitterForce* eforce = (HpEmitterForce*)walker->element;
		et->m_HpWorld->applyForce((HpObject*)particleData->m_physicsObject, eforce->m_position, eforce->m_force); 
	}

	for (SCClList* walker = m_HpEmitterRadialForceList.next; walker; walker = walker->next) {
		HpEmitterRadialForce* eforce = (HpEmitterRadialForce*)walker->element;
		et->m_HpWorld->applyRadialForce((HpObject*)particleData->m_physicsObject, eforce->m_position, eforce->m_radius);
	}
	//	force end

	//	for optimizing purpose , we use update here
	et->m_HpWorld->updateObject((HpObject*)particleData->m_physicsObject);

	//	从物理引擎获取当前位置
	if (HpParticleModeGravity == et->m_mode) {

		particleData->m_deltaPosition = et->m_HpWorld->getObjectPosition(particleData->m_physicsObject);

	}
	else if (HpParticleModeRadius == et->m_mode) {

		//	物理世界锚点坐标
		//	计算重力等等
		HpVector position = et->m_HpWorld->getObjectPosition(particleData->m_physicsObject);

		//	计算极坐标
		particleData->m_revolution.m_deltaAngle = (particleData->m_elapsedTime*particleData->m_revolution.m_angularSpeedResults) + particleData->m_emitter.startAngleResults;
		particleData->m_revolution.m_deltaRadius = (particleData->m_elapsedTime*particleData->m_revolution.m_radiusSpeed) + particleData->m_revolution.m_startRadiusResults;
		particleData->m_deltaPosition.m_x = -cosf(particleData->m_revolution.m_deltaAngle) * particleData->m_revolution.m_deltaRadius + et->m_emittPosition.m_x;
		particleData->m_deltaPosition.m_y = -sinf(particleData->m_revolution.m_deltaAngle) * particleData->m_revolution.m_deltaRadius + et->m_emittPosition.m_y;
		//	add the orginal position

		//	极坐标保存在m_positionExtra
		//	含义见HpWorld::run
		HpObject* object = (HpObject*)particleData->m_physicsObject;
		object->m_positionExtra = particleData->m_deltaPosition;

		particleData->m_deltaPosition.add(position);

	}

	//	update position 
	//	and color , size
	{

		float verticalSize = 0.0f;
		float horizontalSize = 0.0f;

		if (HpSizeModeVHAsSameRatio == et->m_sizemode) {

			float size = particleData->m_size.m_startSizeHorizontalResults + (particleData->m_elapsedTime*particleData->m_size.m_sizeHorizontalGradualAccel);
			verticalSize = size;
			horizontalSize = size;

		}
		else if (HpSizeModeVHAsDiffRatio == et->m_sizemode) {
			verticalSize = particleData->m_size.m_startSizeVerticalResults + (particleData->m_elapsedTime*particleData->m_size.m_sizeVerticalGradualAccel);
			horizontalSize = particleData->m_size.m_startSizeHorizontalResults + (particleData->m_elapsedTime*particleData->m_size.m_sizeHorizontalGradualAccel);
		}

		verticalSize = verticalSize / 2.0f;
		horizontalSize = horizontalSize / 2.0f;

		//	caculate color
		particleData->m_color.m_deltaColor.a = particleData->m_color.m_startColorResults.a + (particleData->m_elapsedTime*particleData->m_color.m_colorGradualAccel.a);

		//	start-end = 0 , 说明前后颜色相同，不需要变化alpha
		if (0 != particleData->m_color.m_colorGradualAccel.a) {

			float alpha = particleData->m_color.m_deltaColor.a / 1.0f;

			particleData->m_color.m_deltaColor.r = (particleData->m_color.m_startColorResults.r + particleData->m_color.m_colorGradualAccel.r*particleData->m_elapsedTime)*alpha;
			particleData->m_color.m_deltaColor.g = (particleData->m_color.m_startColorResults.g + particleData->m_color.m_colorGradualAccel.g*particleData->m_elapsedTime)*alpha;
			particleData->m_color.m_deltaColor.b = (particleData->m_color.m_startColorResults.b + particleData->m_color.m_colorGradualAccel.b*particleData->m_elapsedTime)*alpha;

		}
		else {

			particleData->m_color.m_deltaColor.r = (particleData->m_color.m_startColorResults.r + particleData->m_color.m_colorGradualAccel.r*particleData->m_elapsedTime);
			particleData->m_color.m_deltaColor.g = (particleData->m_color.m_startColorResults.g + particleData->m_color.m_colorGradualAccel.g*particleData->m_elapsedTime);
			particleData->m_color.m_deltaColor.b = (particleData->m_color.m_startColorResults.b + particleData->m_color.m_colorGradualAccel.b*particleData->m_elapsedTime);

		}

		//	caculate the size
		//	and position
		float tlx = particleData->m_deltaPosition.m_x - horizontalSize;
		float tly = particleData->m_deltaPosition.m_y - verticalSize;

		float trx = particleData->m_deltaPosition.m_x + horizontalSize;
		float trry = particleData->m_deltaPosition.m_y - verticalSize;

		float blx = particleData->m_deltaPosition.m_x - horizontalSize;
		float bly = particleData->m_deltaPosition.m_y + verticalSize;

		float brx = particleData->m_deltaPosition.m_x + horizontalSize;
		float bry = particleData->m_deltaPosition.m_y + verticalSize;

		if (particleData->m_rotation.m_startAngleResults > 0.0001)
		{

			float lx = -horizontalSize;
			float ty = -verticalSize;

			float rx = horizontalSize;
			float by = verticalSize;

			float potx = particleData->m_deltaPosition.m_x;
			float poty = particleData->m_deltaPosition.m_y;

			float radians = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_elapsedTime*particleData->m_rotation.m_angularAccel);
			float cos = HpCosine(radians);
			float sin = HpSine(radians);

			//	左上角
			tlx = lx * cos - ty * sin + potx;
			tly = lx * sin + ty * cos + poty;

			//	右上角
			trx = rx * cos - ty * sin + potx;
			trry = rx * sin + ty * cos + poty;

			//	左下角
			blx = lx * cos - by * sin + potx;
			bly = lx * sin + by * cos + poty;

			//	右下角
			brx = rx * cos - by * sin + potx;
			bry = rx * sin + by * cos + poty;

		}


		HP_VERTEX_SET_TL_V(particleData,tlx,tly,0);
		HP_VERTEX_SET_TR_V(particleData,trx,trry,0);
		HP_VERTEX_SET_BL_V(particleData,blx,bly,0);
		HP_VERTEX_SET_BR_V(particleData,brx,bry,0);

		HP_COLOR_SET_BL_V(particleData,particleData) ;
		HP_COLOR_SET_BR_V(particleData,particleData) ;
		HP_COLOR_SET_TL_V(particleData,particleData) ;
		HP_COLOR_SET_TR_V(particleData,particleData) ;


	}

}

/*
	brief : 
*/
void HpEmitterLogic::initParticle ( HpEmitter* et , HpParticle* particleData ) 
{
	  
	//	每次震动，可以得到更好的震动频率
	particleData->m_elapsedTime = 0.0f ;
	particleData->m_lifeSpanResults = et->m_orignal_data.m_lifeSpan + ( et->m_orignal_data.m_lifeSpanVariance*HPRANDOM_MINUS1_1() ) ;	
	particleData->m_lifeSpanResults = HpMAX(0,particleData->m_lifeSpanResults) ;

	if ( 0 == particleData->m_lifeSpanResults ) {
		particleData->m_lifeSpanResults = et->m_orignal_data.m_lifeSpan + et->m_orignal_data.m_lifeSpanVariance ;
	}

	//!	颜色
	//*	开始颜色
	particleData->m_color.m_startColorResults.r = HpClampf( et->m_orignal_data.m_color.m_startColor.r + (et->m_orignal_data.m_color.m_startColorVariance.r*HPRANDOM_MINUS1_1()),0,1.0f) ;
	particleData->m_color.m_startColorResults.g = HpClampf( et->m_orignal_data.m_color.m_startColor.g + (et->m_orignal_data.m_color.m_startColorVariance.g*HPRANDOM_MINUS1_1()),0,1.0f) ;
	particleData->m_color.m_startColorResults.b = HpClampf( et->m_orignal_data.m_color.m_startColor.b + (et->m_orignal_data.m_color.m_startColorVariance.b*HPRANDOM_MINUS1_1()),0,1.0f) ;
	particleData->m_color.m_startColorResults.a = HpClampf( et->m_orignal_data.m_color.m_startColor.a + (et->m_orignal_data.m_color.m_startColorVariance.a*HPRANDOM_MINUS1_1()),0,1.0f) ;

	//*	结束颜色
	particleData->m_color.m_endColorResults.r = HpClampf( et->m_orignal_data.m_color.m_endColor.r + (et->m_orignal_data.m_color.m_endColorVariance.r*HPRANDOM_MINUS1_1()),0,1.0f) ;
	particleData->m_color.m_endColorResults.g = HpClampf( et->m_orignal_data.m_color.m_endColor.g + (et->m_orignal_data.m_color.m_endColorVariance.g*HPRANDOM_MINUS1_1()),0,1.0f) ;
	particleData->m_color.m_endColorResults.b = HpClampf( et->m_orignal_data.m_color.m_endColor.b + (et->m_orignal_data.m_color.m_endColorVariance.b*HPRANDOM_MINUS1_1()),0,1.0f) ;
	particleData->m_color.m_endColorResults.a = HpClampf( et->m_orignal_data.m_color.m_endColor.a + (et->m_orignal_data.m_color.m_endColorVariance.a*HPRANDOM_MINUS1_1()),0,1.0f) ;

	//*	颜色变化率
	particleData->m_color.m_colorGradualAccel.r = (particleData->m_color.m_endColorResults.r-particleData->m_color.m_startColorResults.r) / particleData->m_lifeSpanResults ;
	particleData->m_color.m_colorGradualAccel.g = (particleData->m_color.m_endColorResults.g-particleData->m_color.m_startColorResults.g) / particleData->m_lifeSpanResults ;
	particleData->m_color.m_colorGradualAccel.b = (particleData->m_color.m_endColorResults.b-particleData->m_color.m_startColorResults.b) / particleData->m_lifeSpanResults ;
	particleData->m_color.m_colorGradualAccel.a = (particleData->m_color.m_endColorResults.a-particleData->m_color.m_startColorResults.a) / particleData->m_lifeSpanResults ;

	//!	尺寸变化
	//	end size 横向 

	particleData->m_size.m_startSizeHorizontalResults = et->m_orignal_data.m_size.m_startSizeHorizontal + ( et->m_orignal_data.m_size.m_startSizeHorizontalVariance*HPRANDOM_MINUS1_1() ) ;
	particleData->m_size.m_endSizeHorizontalResults = et->m_orignal_data.m_size.m_endSizeHorizontal + ( et->m_orignal_data.m_size.m_endSizeHorizontalVariance*HPRANDOM_MINUS1_1() ) ;
	//	横向变化率
	particleData->m_size.m_sizeHorizontalGradualAccel = (particleData->m_size.m_endSizeHorizontalResults-particleData->m_size.m_startSizeHorizontalResults) / particleData->m_lifeSpanResults ;

	//	start size 纵向
	particleData->m_size.m_startSizeVerticalResults = et->m_orignal_data.m_size.m_startSizeVertical + ( et->m_orignal_data.m_size.m_startSizeVerticalVariance*HPRANDOM_MINUS1_1() ) ;
	//	end size 纵向
	particleData->m_size.m_endSizeVerticalResults = et->m_orignal_data.m_size.m_endSizeVertical + ( et->m_orignal_data.m_size.m_endSizeVerticalVariance*HPRANDOM_MINUS1_1() ) ;
	//	纵向变化率
	particleData->m_size.m_sizeVerticalGradualAccel = (particleData->m_size.m_endSizeVerticalResults-particleData->m_size.m_startSizeVerticalResults) / particleData->m_lifeSpanResults ;
	 
	//*	喷口
	if ( 0 != et->m_orignal_data.m_emitter.angleVariance ) {
		particleData->m_emitter.startAngleResults = (float )(rand()%(int)et->m_orignal_data.m_emitter.angleVariance)+et->m_orignal_data.m_emitter.startAngle ; 	
	} else {
		particleData->m_emitter.startAngleResults = et->m_orignal_data.m_emitter.startAngle ; 	
	}
	particleData->m_emitter.startAngleResults = (float )((int )particleData->m_emitter.startAngleResults % 360) ;
	particleData->m_emitter.startAngleResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_emitter.startAngleResults) ;
	 
	//*	自转
	particleData->m_rotation.m_startAngleResults = et->m_orignal_data.m_rotation.m_startAngle + ( et->m_orignal_data.m_rotation.m_startAngleVariance*HPRANDOM_MINUS1_1() ) ; 
	particleData->m_rotation.m_endAngleResults = et->m_orignal_data.m_rotation.m_endAngle + ( et->m_orignal_data.m_rotation.m_endAngleVariance*HPRANDOM_MINUS1_1() ) ; 
	particleData->m_rotation.m_angularAccel = ( particleData->m_rotation.m_endAngleResults - particleData->m_rotation.m_startAngleResults ) / particleData->m_lifeSpanResults ;
	//*	转弧度
	particleData->m_rotation.m_startAngleResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_rotation.m_startAngleResults) ;
	particleData->m_rotation.m_endAngleResults = HP_ADAPTER_HDEGREES_TO_RADIANS(particleData->m_rotation.m_endAngleResults) ;

	//*	公转
	particleData->m_revolution.m_angularSpeedResults = HP_ADAPTER_HDEGREES_TO_RADIANS(et->m_orignal_data.m_revolution.m_angularSpeed + et->m_orignal_data.m_revolution.m_angularSpeedVariance * HPRANDOM_MINUS1_1());
	particleData->m_revolution.m_deltaAngle = particleData->m_emitter.startAngleResults ;

	particleData->m_revolution.m_startRadiusResults = et->m_orignal_data.m_revolution.m_startRadius + (et->m_orignal_data.m_revolution.m_startRadiusVariance*HPRANDOM_MINUS1_1()) ; 
	particleData->m_revolution.m_endRadiusResults = et->m_orignal_data.m_revolution.m_endRadius + (et->m_orignal_data.m_revolution.m_endRadiusVariance*HPRANDOM_MINUS1_1()) ;

	particleData->m_revolution.m_radiusSpeed = (particleData->m_revolution.m_endRadiusResults-particleData->m_revolution.m_startRadiusResults) / particleData->m_lifeSpanResults ;
	particleData->m_revolution.m_deltaRadius = 0.0f ;
	particleData->m_force.init ( et->m_orignal_data.m_force.m_x , et->m_orignal_data.m_force.m_y ) ;

	if ( 0 == particleData->m_physicsObject ) {
		particleData->m_physicsObject = et->m_HpWorld->createObject( particleData ) ;
		HpObject* ob = (HpObject*)particleData->m_physicsObject ;
		if ( HpParticleModeGravity == et->m_mode ) {
			ob->setFriction ( et->m_orignal_data.m_friction ) ;			
		} else {   
			et->m_HpWorld->setGravity(+0.01f) ;
			ob->setFriction ( 0.0f ) ;
			if ( 0 < et->m_orignal_data.m_mass ) {
				ob->setMass ( et->m_orignal_data.m_mass ) ;
			}
			particleData->m_force.init(0.0,0.0);
		}
		et->m_HpWorld->setGravity(et->m_gravity) ;
	}
	
	if ( -1 == et->m_emittDuration ) {
	//	et->m_emittRate = et->m_emittTotall / particleData->m_lifeSpanResults ;
	} else {
	//	et->m_emittRate = et->m_emittTotall / et->m_emittDuration ;
	}
	et->m_emittRate = et->m_emittTotall / particleData->m_lifeSpanResults ;

}