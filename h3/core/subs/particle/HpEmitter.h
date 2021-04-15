
/*
+	HpEmitter.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef HP_EMITTER_H
#define HP_EMITTER_H

#include "HpAdapter.h"
#include "HpVector.h"
#include "HpObject.h"
#include "HpWorld.h"
#include "HpParticle.h"
#include "sccl.h"

using namespace std;

#define PARTICLE_MAX_FRAME 128

/*
	brief : emitt texture frame
*/
typedef struct {

	char m_name [ 64 ] ;
	//	a handle to HpTexture2D
	long long m_handle ;
	//	texture id of rendering pipeline
	long long m_id ;
	
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

} HpeTextureAtom ;

/*
	brief : emitt texture frame list
*/
typedef struct {

	HpeTextureAtom m_framelist[PARTICLE_MAX_FRAME] ;
	//	play frame loop
	int frameLoop ;
	int m_totallFrame ;

} HpeTexture ;

/*
	brief : structure of an emitter tree
*/
typedef struct EtNODE {

	char m_name [32] ;

	//	its can not deleted if a emitt is static 
	bool m_static ;
	//	emitter already play , but particle isnt emitted maybe , based on m_starttime
	bool m_actived ;
	bool m_dead ;
	bool m_visible ;

	HpeMODE m_mode ;
	HpeSIZEMODE m_sizemode ;
	HpeRANDMODE m_randmode ;

	//	elapsed time since run
	float m_elapsedTotallTime ;
	//	elapsed time since emitt
	float m_elapsedEmittedTotallTime ;
	
	//	origin position of emitter at world space
	//	0 as default
	HpVector m_orgCoordinate ;
	//	current position of emitter at world space
	HpVector m_emittPosition ;
	
	//	in sec
	float m_starttime ;
	//	in sec
	float m_emittDuration ;
	//	totall particle but not include child
	int m_emittTotall ;
	//	already emitted
	int m_emittCounter ;
	//	needs emitt per dt
	float m_emittTotallPerSec ;
	//*	neds emitt per sec
	float m_emittRate ;
	//	world gravity 
	float m_gravity ;

	int m_BlendFuncSrc ;
	int m_BlendFuncDst ;

	//	capacity = m_emittTotall*sizeof(HpPARTICLE)
	HpParticle* m_data ;
	//	the first particle data
	HpParticle m_orignal_data ;
	
	HpWorld* m_HpWorld ;

	HpeTexture m_hpeTexture ;

	//	render handle
	void* m_render ;
	//	geometry handle
	void* geometry ;

	struct EtNODE* pre ;
	struct EtNODE* next ;

} HpEmitter ;

/*
	class : HpEmitterContainer
	brief : load emitt tree(et) and cache
*/
class HpEmitterContainer
{
public :
	HpEmitterContainer () ;
	~HpEmitterContainer () ;

	/*
		brief : load et file
	*/
	HpEmitter* loadEmitter ( const char* file ) ;

	/*
		brief : load et tree
	*/
	HpEmitter* loadEmitter ( HpEmitter* tree ) ;
	
	/*
		brief : get an et
	*/
	HpEmitter* getEmitter ( const char* file ) ;

	/*
		brief : find an et
	*/
	bool findEmitter ( HpEmitter* tree ) ;

	/*
		brief : 
	*/
	void resetEmitter ( HpEmitter* tree ) ;

	/*
		brief : copy a emitter and new
	*/
	HpEmitter* copyEmitter ( HpEmitter* tree ) ;

	/*
		brief : 
	*/
	void insertEmitter ( HpEmitter* tree ) ;

	/*
		brief : 
	*/
	void removeEmitter ( HpEmitter* tree ) ;

	/*
		brief : 
	*/
	void resetEmitterEx () ;

	/*
		brief : get texture
	*/
	static void* getTexture ( HpeTexture* source ) ;

	/*
		brief : get texture atom
	*/
	static void* getTextureAtom ( HpeTexture* source ) ;

private :

	/*
		brief : find hpeTexture
	*/
	void* getTextureHandle ( const char* file ) ;

	/*
		brief : insert hpeTexture
	*/
	void insertTexture ( HpeTexture* hpeTexture ) ;

	/*
		brief : copy hpeTexture
	*/
	void copyHpeTexture ( HpeTexture* dest , HpeTexture* source ) ;

	/*
		brief : destroy et
	*/
	void destroyEmitter ( HpEmitter* et ) ;

	/*
		brief : 
	*/
	void resetEmitterNode ( HpEmitter* node ) ;

	/*
		brief : et list
	*/
	SCClList m_HpEmitterList ;
	
	/*
		brief : texture list
	*/
	SCClList m_HpeTextureList ;

} ;

#endif

