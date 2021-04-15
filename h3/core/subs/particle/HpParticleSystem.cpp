
/*
+	HpParticleSystem.cpp
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "schal.h"
#include "HpEmitterLogic.h"
#include "HpEmitterRender.h"
#include "HpParticleSystem.h"
#include "HpEmitter.h"

HpParticleSystem::HpParticleSystem () 
{

	m_DirtyEmitter = (SCClQueue* ) MMC_MALLOC ( sizeof(SCClQueue) ) ;
	assert(m_DirtyEmitter) ; 
	SCClQueueInit ( (SCClQueue* )m_DirtyEmitter ) ;
		 
	hpEtLogicHandle = NULL ;
	hpEtRenderHandle = NULL ;
	hpEtContainerHandle = NULL ;

	HpEmitterContainer* container = 0 ;
	container = new HpEmitterContainer () ;
	if ( NULL == container ) {
		iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
		return ;
	}
	hpEtContainerHandle = (void* ) container ;

	//	emitt logic
	HpEmitterLogic* logic = 0 ;
	logic = new HpEmitterLogic () ;
	if ( NULL == logic ) {
		iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
		return ;
	}
	hpEtLogicHandle = (void* ) logic ;

	//	emitt render
	HpEmitterRender* render = 0 ; 
	render = new HpEmitterRender () ;
	if ( NULL == render ) {
		iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
		return ;
	}
	hpEtRenderHandle = (void* ) render ;

#ifdef HP_COCOS2D_PORT
	this->addChild ( render ) ;
#endif

}

HpParticleSystem::~HpParticleSystem () 
{

	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	HpEmitterRender* render = (HpEmitterRender* )hpEtRenderHandle ;

	//	empty dirty queue
	SCClQueueDestroy ( (SCClQueue* )m_DirtyEmitter ) ;
	MMC_FREE ((SCClQueue* )m_DirtyEmitter) ;

	if ( NULL != container ) {
		delete container ;
	}

	if ( NULL != logic ) {
		delete logic ;
	} 
	   
	if ( NULL != render ) {
#ifdef HP_COCOS2D_PORT
		render->removeFromParent() ;
		render->release() ;
#endif
	}

	MMC_CHECK_MEMORY_OVERFLOW();
	MMC_CHECK_MEMORY_LEAK();

}

#ifdef HP_COCOS2D_PORT
/*
	brief : 
	param : 
*/	
void HpParticleSystem::onEnter() 
{
	start () ;
	CCLayer::onEnter () ;
}

/*
	brief : 
	param : 
*/
void HpParticleSystem::onExit () 
{
	stop () ;
	CCLayer::onExit () ;
}
#endif

/*
	brief : load emitter
	param : emitter file name
*/
void* HpParticleSystem::loadEmitter ( const char* filename ) 
{
	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;
	if ( NULL == container ) {
		return 0 ;
	}
	
	HpEmitter* tree = NULL ;
	tree = container->getEmitter ( filename ) ;
	if ( NULL != tree ) {
		return tree ;
	}

	tree = container->loadEmitter ( filename ) ;
	if ( NULL == tree ) {
		return 0 ;
	}
	//	insert tree
	for ( HpEmitter* walker = tree ; walker ; walker = walker->next ) {
		container->insertEmitter ( walker ) ;
	}
	
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;

	if ( NULL == logic ) {
		return 0 ;
	}
	
	//	只是缓存起来，不放入流水线
	//logic->insertEmitter ( tree ) ;
	//render->insertEmitter ( tree ) ;

	logic->setEmitterCallback ( this , (HpCB_EMITTER)&HpParticleSystem::hpEtCallback ) ;

	return (void* )tree ;

}

/*
	brief : load emitter tree
	param : emitter struct
*/
void* HpParticleSystem::loadEmitter ( void* tree ) 
{
	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;

	if ( NULL == container ) {
		return 0 ;
	}
	
	if ( true == container->findEmitter ( (HpEmitter* )tree ) ) {
		return tree ;
	}

	tree = container->loadEmitter((HpEmitter*)tree);
	if ( NULL == tree ) {
		return 0 ;
	}
	//	insert tree
	for (HpEmitter* walker = (HpEmitter*)tree; walker; walker = walker->next) {
		container->insertEmitter ( walker ) ;
	}
	
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;

	if ( NULL == logic ) {
		return 0 ;
	}
	
	//	只是缓存起来，不放入流水线
	//logic->insertEmitter ( tree ) ;
	//render->insertEmitter ( tree ) ;

	logic->setEmitterCallback ( this , (HpCB_EMITTER)&HpParticleSystem::hpEtCallback ) ;

	return (void* )tree ;
}

/*
	brief : 
	param : 
*/
void* HpParticleSystem::createEmitter ( void* handle ) 
{
	HpEmitter* tree = (HpEmitter* ) handle ;
	HpEmitter* treeNew = 0 ;
	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;

	if ( 0 == tree || 0 == container ) {
		return 0 ;
	}

	//	copy tree
	treeNew = container->copyEmitter ( tree ) ;
	//	insert tree
	
	for ( HpEmitter* walker = treeNew ; walker ; walker = walker->next ) {
		container->insertEmitter ( walker ) ;
	}

	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	HpEmitterRender* render = (HpEmitterRender* )hpEtRenderHandle ;

	if ( NULL == logic || NULL == render ) {
		return 0 ;
	}

	for ( HpEmitter* walker = treeNew ; walker ; walker = walker->next ) {
		logic->insertEmitter ( walker ) ;
		render->insertEmitter ( walker ) ;
	}

	logic->setEmitterCallback ( this , (HpCB_EMITTER)&HpParticleSystem::hpEtCallback ) ;

	return treeNew ;

}

/*
	brief : play emitter
	param : effect handle
*/
void HpParticleSystem::playEmitter ( void* handle ) 
{
	HpEmitter* tree = (HpEmitter* )handle ;
	if ( 0 == tree ) {
		return ;
	}
	if ( true == tree->m_static ) {
		iLog ( "Hp Error : Can not play a static emitter tree") ;
		assert(false == tree->m_static);
	}
	
	for ( HpEmitter* walker = tree ; walker ; walker = walker->next ) {
		walker->m_actived = true ;
	}

}

/*
	brief : stop emitter
	param : effect handle
*/
void HpParticleSystem::stopEmitter ( void* handle ) 
{

	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;
	if ( NULL == container ) {
		return ;
	}
	HpEmitter* tree = (HpEmitter* )handle ;
	if ( 0 == tree ) {
		return ;
	}

	for ( HpEmitter* walker = tree ; walker ; walker = walker->next ) {
		container->resetEmitter (walker) ;
	}

}

/*
	brief : stop emitter all
	param : emitter handle
*/
void HpParticleSystem::stopEmitterAll () 
{
	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;
	if ( NULL == container ) {
		return ;
	}
	container->resetEmitterEx () ;
}

/*
	brief : kill emitter
	param : effect handle
*/
void HpParticleSystem::killEmitter ( void* handle ) 
{
	HpEmitter* tree = (HpEmitter* )handle ;

	for ( HpEmitter* walker = tree ; walker ; walker = walker->next ) {
		//	强制删除
		//if ( HpParticleModeInfinite == tree->m_emittDuration ) {
		walker->m_emittDuration = 0.0f ;
		//}
	}

}

/*
	brief : is emitter playing
	param : 
*/
bool HpParticleSystem::aliveEmitter ( void* handle ) 
{
	HpEmitter* tree = (HpEmitter* ) handle ;

	if ( 0 == tree ) {
		return false ;
	}

	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;
	if ( NULL == container ) {
		return false ;
	}

	if ( false == container->findEmitter(tree) ) {
		return false ;
	}

	//	链表中有一个节点活跃，则认为整个链表是活跃，虽然此时有可能只有一个节点
	for ( HpEmitter* walker = tree ; walker ; walker = walker->next ) {
		if ( true == walker->m_actived ) {
			return true ;
		}
	}

	return false ;

}
  
#ifdef HP_COCOS2D_PORT
/*
	brief : set the last emitter-particle position in world space
	param : emitter handle
*/
void HpParticleSystem::setEmitterPosition ( void* handle , CCPoint position ) 
#else
/*
	brief : set the last emitter-particle position in world space
	param : emitter handle
*/
void HpParticleSystem::setEmitterPosition ( void* handle , HpPoint position ) 
#endif
{
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	if ( NULL == handle || NULL == logic ) {
		return ;
	}

	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;

	if ( NULL == container ) {
		return ;
	}

	//	整个emitter表中的所有emitter都要设置位置，因为他们是一个整体
	for ( HpEmitter* walker = (HpEmitter* )handle ; walker ; walker = walker->next ) {
		HpVector v ;
		v.init (position.x,position.y) ;
		logic->setEmitterPosition ( walker , v ) ;
	}

}

#ifdef HP_COCOS2D_PORT
/*
	brief : set the last emitter-particle position in world space
	param : emitter handle
*/
void HpParticleSystem::setEmitterPositionEx ( void* handle , CCPoint position ) 
#else
/*
	brief : set the last emitter-particle position in world space
	param : emitter handle
*/
void HpParticleSystem::setEmitterPositionEx ( void* handle , HpPoint position ) 
#endif
{
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	if ( NULL == handle || NULL == logic ) {
		return ;
	}

	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;

	if ( NULL == container ) {
		return ;
	}

	if ( false == container->findEmitter ( (HpEmitter* )handle ) ) {
		return ;
	}

	//	整个emitter表中的所有emitter都要设置位置，因为他们是一个整体
	for ( HpEmitter* walker = (HpEmitter* )handle ; walker ; walker = walker->next ) {
		HpVector v ;
		v.init (position.x,position.y) ;
		logic->setEmitterPositionEx ( walker , v ) ;
	}

}

/*
	brief : set emitter radius
	param : emitter handle
*/
void HpParticleSystem::setEmitterRadius ( void* handle , float start_r , float end_r ) 
{

	//	整个链表具有相同属性
	for ( HpEmitter* walker = (HpEmitter* )handle ; walker ; walker = walker->next ) {
		walker->m_orignal_data.m_revolution.m_startRadius = start_r ;
		walker->m_orignal_data.m_revolution.m_endRadius = end_r ;
	}

}

/*
	brief : set emitter duration
	param : emitter handle , sec
*/
void HpParticleSystem::setEmitterDuration ( void* handle , float duration ) 
{
	//	整个链表具有相同属性
	for ( HpEmitter* walker = (HpEmitter* )handle ; walker ; walker = walker->next ) {
		walker->m_emittDuration = duration ;
	}

}

/*
	brief : set emitter color
	param : emitter handle
*/
void HpParticleSystem::setEmitterColor ( void* handle , hpColor4F start_c , hpColor4F end_c ) 
{
	//	整个链表具有相同属性
	for ( HpEmitter* walker = (HpEmitter* )handle ; walker ; walker = walker->next ) {
		walker->m_orignal_data.m_color.m_startColor = start_c ;
		walker->m_orignal_data.m_color.m_endColor = end_c ;
	}
}

/*
	brief : set emitter blend mode
	param : emitter handle
*/
void HpParticleSystem::setEmitterBlend ( void* handle , int src , int dst ) 
{
	//	整个链表具有相同属性
	for ( HpEmitter* walker = (HpEmitter* )handle ; walker ; walker = walker->next ) {
		walker->m_BlendFuncSrc = src ;
		walker->m_BlendFuncDst = dst ;
	}
}

/*
	brief : set radia force
	param : 
*/
void HpParticleSystem::applyRadialForce ( HpVector position , float radius ) 
{
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	logic->applyRadialForce ( position , radius ) ;
}

/*
	brief : set force
	param : 
*/
void HpParticleSystem::applyForce ( HpVector position , float force ) 
{
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	logic->applyForce ( position , force ) ;
}

/*
	brief : 
	param : 
*/
void HpParticleSystem::update ( float dt ) 
{
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	logic->update ( dt ) ;

	HpEmitterRender* render = (HpEmitterRender* )hpEtRenderHandle ;
	render->run ( dt ) ;

	clearDirtyEmitter () ;
}

#ifdef HP_H3_PORT
/*
	brief : 
	param : 
*/
void HpParticleSystem::draw ( float dt ) 
{
	HpEmitterRender* render = (HpEmitterRender* )hpEtRenderHandle ;
	render->draw ( dt ) ;
}
#endif
/*
	brief : Emitter Tree
*/
void HpParticleSystem::hpEtCallback ( int status , void* node ) 
{
	//	放入diry队列，异步删除
	if ( HpeLogicStatus_Emitter_Dead == status ) {
		SCClQueueEnter ( (SCClQueue* )m_DirtyEmitter , node ) ;
	}
}

/*
	brief : kill effect
	param : effect handle
*/
void HpParticleSystem::killEmitterEx ( void* handle ) 
{

	HpEmitterContainer* container = (HpEmitterContainer* ) hpEtContainerHandle ;
	HpEmitterLogic* logic = (HpEmitterLogic* )hpEtLogicHandle ;
	HpEmitterRender* render = (HpEmitterRender* )hpEtRenderHandle ; 

	if ( NULL == container || NULL == logic || NULL == render ) {
		return ;
	}

	HpEmitter* walker = (HpEmitter* )handle ;
	if ( 0 != walker->pre ) {
		walker->pre->next = walker->next ;
		if ( walker->next ) {
			walker->next->pre = walker->pre ;
		}
	} else {
		if ( walker->next ) {
			walker->next->pre = 0 ;
		}
	}
	logic->removeEmitter ( walker ) ;
	render->removeEmitter ( walker ) ;
	container->removeEmitter ( walker ) ;

}

/*
	brief : start system
	param : 
*/
void HpParticleSystem::start () 
{
#ifdef HP_COCOS2D_PORT
	this->scheduleUpdate () ;
#endif
}

/* 
	brief : stop system
	param : 
*/
void HpParticleSystem::stop () 
{
	clearDirtyEmitter () ;
#ifdef HP_COCOS2D_PORT
	this->unscheduleUpdate () ;
#endif
}

/*
	brief : clear dirty emitter
	param : 
*/
void HpParticleSystem::clearDirtyEmitter () 
{
	while ( 0 == SCClQueueEmpty ( (SCClQueue* )m_DirtyEmitter ) ) {

		HpEmitter* node = 0 ;
		  
		node = ( HpEmitter* )SCClQueueOut ( (SCClQueue* )m_DirtyEmitter ) ;

		killEmitterEx ( node ) ;

	}
}