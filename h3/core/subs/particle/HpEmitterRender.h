
/*
+	HpEmitterRender.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef __HP_EMITTERRENDER_H__
#define __HP_EMITTERRENDER_H__

using namespace std;

#include "HpAdapter.h"
#include "HpVector.h"
#include "HpObject.h"
#include "HpWorld.h"
#include "HpParticle.h"
#include "HpEmitter.h"
#include "sccl.h"

#ifdef HP_COCOS2D_PORT
#include "renderer/CCQuadCommand.h"
#endif

//#define ALWAYS_CREAT_VBO

typedef struct {

	//	update by HpEmitterLogic

	//	point to an emitter tree
	void* m_emitt_handle ;

#ifdef HP_COCOS2D_PORT
	V3F_C4B_T2F_Quad* m_pQuads ;
#endif

#ifdef HP_H3_PORT
	H3_V3F_T2F_C4F_Quad* m_pQuads ;
#endif

	int m_totallQuads ;

	unsigned short* m_pIndices ;
	int m_totallIndices ;

#ifdef HP_COCOS2D_PORT
	QuadCommand* quadCommand ;
#endif

} HpEmitterGeometry ; 

/*
	brief : 
*/
class HpEmitterRenderNode {
public :

	//	a handle to HpTexture2D
	void* m_handle ;	

	SCClList m_HpEmitterGeometryList ;

	//	0 : vertex  
	//	1 : indices
	unsigned int m_pBuffersVBO[2]; 

	//	texture frame
	HpeTextureAtom m_framelist[PARTICLE_MAX_FRAME] ;
	bool m_frameLoop ;
	int m_totallFrame ;
	int m_currentFrame ;
	float m_timecost ;

} ;

/*
	brief : 
*/
class HpEmitterRender
//	如果适配Cocos2dx，在此继承CCLayer，使用其draw机制
#ifdef HP_COCOS2D_PORT
: public CCLayer 
#endif
{
public :

	HpEmitterRender () ;

	~HpEmitterRender () ;

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
	void run (float dt) ;

	/*
		brief : 
	*/
#ifdef HP_COCOS2D_PORT
	virtual void draw (Renderer *renderer, const Mat4& transform, uint32_t flags) ;
#else
	virtual void draw ( float dt ) ;
#endif

private :

	/*
		brief : 
	*/
	void initRenderNode ( HpEmitter* et ) ;
	
	/*
		brief : 
	*/
	void deleteRenderNode ( HpEmitterRenderNode* render ) ;

	/*
		brief : 
	*/
	HpEmitterRenderNode* getRenderNode ( void* textureid ) ;  

	/*
		brief : 
	*/
	SCClList m_HpEmitterRenderNodeList ;

} ;

#endif