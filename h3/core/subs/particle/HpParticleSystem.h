
/*
+	HpParticleSystem.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef __HP_PARTICLESYSTEM_H__
#define __HP_PARTICLESYSTEM_H__

#include "HpAdapter.h"
#include "HpGeometry.h"
#include "HpVector.h"

/*  
	brief : 
*/
class HpParticleSystem
//	如果适配Cocos2dx，在此继承CCLayer，使用其draw机制
#ifdef HP_COCOS2D_PORT
: public CCLayer
#endif
{
public :

	HpParticleSystem () ;

	~HpParticleSystem () ;

#ifdef HP_COCOS2D_PORT
	/*
		brief : 
		param : 
	*/	
	void onEnter() ;
	
	/*
		brief : 
		param : 
	*/
	void onExit () ;
#endif

	/*
		brief : load emitter
		param : emitter file name
	*/
	void* loadEmitter ( const char* filename ) ;

	/*
		brief : load emitter tree
		param : emitter struct
	*/
	void* loadEmitter ( void* tree ) ;

	/*
		brief : 
		param : 
	*/
	void* createEmitter ( void* handle ) ;

	/*
		brief : play emitter
		param : emitter handle
	*/
	void playEmitter ( void* handle ) ;

	/*
		brief : stop emitter
		param : emitter handle
	*/
	void stopEmitter ( void* handle ) ;

	/*
		brief : stop emitter all
		param : emitter handle
	*/
	void stopEmitterAll () ;

	/*
		brief : kill emitter
		param : emitter handle
	*/
	void killEmitter ( void* handle ) ;

	/*
		brief : is emitter playing
		param : 
	*/
	bool aliveEmitter ( void* handle ) ;

#ifdef HP_COCOS2D_PORT
	/*
		brief : set the last emitter-particle position in world space
		param : emitter handle
	*/
	void setEmitterPosition ( void* handle , CCPoint position ) ;
#else
	/*
		brief : set the last emitter-particle position in world space
		param : emitter handle
	*/
	void setEmitterPosition ( void* handle , HpPoint position ) ;
#endif

#ifdef HP_COCOS2D_PORT
	/*
		brief : set the last emitter-particle position in world space
		param : emitter handle
	*/
	void setEmitterPositionEx ( void* handle , CCPoint position ) ;
#else
	/*
		brief : set the last emitter-particle position in world space
		param : emitter handle
	*/
	void setEmitterPositionEx ( void* handle , HpPoint position ) ;
#endif

	/*
		brief : set emitter radius
		param : emitter handle
	*/
	void setEmitterRadius ( void* handle , float start_r , float end_r ) ;

	/*
		brief : set emitter duration
		param : emitter handle , sec
	*/
	void setEmitterDuration ( void* handle , float duration ) ;

	/*
		brief : set emitter color
		param : emitter handle
	*/
	void setEmitterColor ( void* handle , hpColor4F start_c , hpColor4F end_c ) ;
	
	/*
		brief : set emitter blend mode
		param : emitter handle
	*/
	void setEmitterBlend ( void* handle , int src , int dst ) ;

	/*
		brief : set radial force
		param : 
	*/
	void applyRadialForce ( HpVector position , float radius ) ;

	/*
		brief : set force
		param : 
	*/
	void applyForce ( HpVector position , float force ) ;	

	/*
		brief : 
		param : 
	*/
	virtual void update ( float dt ) ;
	
#ifdef HP_H3_PORT
	/*
		brief : 
		param : 
	*/
	virtual void draw ( float dt ) ;
#endif

private :

	/*
		brief : start system
		param : 
	*/
	void start () ;

	/*
		brief : stop system
		param : 
	*/
	void stop () ;

	/*
		brief : stop system
		param : 
	*/
	void clearDirtyEmitter () ;

	/*
		brief : Emitter Tree Callback
	*/
	void hpEtCallback ( int status , void* node ) ;

	/*
		brief : kill emitter
		param : effect handle
	*/
	void killEmitterEx ( void* handle ) ;

	/*
		brief : Emitter Tree Container
	*/
	void* hpEtContainerHandle ; 

	/*
		brief : Logic of Emitter Tree
	*/
	void* hpEtLogicHandle ;

	/*
		brief : Render of Emitter Tree
	*/
	void* hpEtRenderHandle ;

	/*
		brief : Dirty Emitter Tree
	*/
	void* m_DirtyEmitter ;

} ;

#endif