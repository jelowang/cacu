
/*
+	HpEmitterLogic.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef __HP_EMITTERLOGIC_H__
#define __HP_EMITTERLOGIC_H__
  
#include "HpAdapter.h"
#include "HpVector.h"
#include "HpObject.h"
#include "HpWorld.h"
#include "HpParticle.h"
#include "HpEmitter.h"
#include "HpParticleSystem.h"
#include "sccl.h"

typedef void (HpParticleSystem::*HpCB_EMITTER)(int status , void* emitter ) ;

typedef enum {

	HpeLogicStatus_Emitter_Dead ,

} HpeLogicStatus ;

/*
	brief : 
*/
typedef struct {
	bool m_dirty ;
	HpVector m_position ;
	float m_radius ;
} HpEmitterRadialForce ;

typedef struct {
	bool m_dirty ;
	HpVector m_position ;
	float m_force ;
} HpEmitterForce ;

/*
	brief : 
*/
class HpEmitterLogic
{
public :

	HpEmitterLogic () ;

	~HpEmitterLogic () ;

	/*
		brief : 
	*/
	void insertEmitter ( HpEmitter* et ) ;

	/*
		brief : 
	*/
	void removeEmitter ( HpEmitter* et ) ;

	/*
		brief : 
	*/
	void setEmitterPosition ( HpEmitter* et , HpVector position ) ;

	/*
	brief : 
	*/
	void setEmitterPositionEx ( HpEmitter* et , HpVector position ) ;

	/*
		brief : 
	*/
	void setEmitterCallback ( HpParticleSystem* target , HpCB_EMITTER callback ) ;

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
	*/
	virtual void update ( float dt ) ;

private :

	/*
		brief : 
	*/
	void walkEmitter ( HpEmitter* et ) ;

	/*
		brief : active an emitter tree node
	*/
	void updateEmitter ( HpEmitter* et ) ;

	/*
		brief : active a particle
	*/
	void activeParticle ( HpEmitter* et ) ;

	/*
		brief : update a particle
	*/
	void updateParticle ( HpEmitter* et ) ;

	/* 
	brief : caculate a particle
	*/
	void caculateParticle ( HpEmitter* et , HpParticle* particleData ) ;

	/*
		brief : init particle
	*/
	void initParticle ( HpEmitter* et , HpParticle* particle ) ;
	   
	/*
		brief : 
	*/
	HpCB_EMITTER m_callback ;
	HpParticleSystem* m_callback_target ;

	/*
		brief : 
	*/
	SCClList m_HpEmitterListSccl ;

	/*
		brief : 
	*/
	SCClList m_HpEmitterForceList ;

	/*
		brief : 
	*/
	SCClList m_HpEmitterRadialForceList ;

	/*
		brief : 
	*/
	float m_dt ;


} ;

#endif