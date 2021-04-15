
/*
+	HpEmitterRender.cpp
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "schal.h"
#include "HpAdapter.h"
#include "HpGeometry.h"
#include "HpEmitterLogic.h"
#include "HpEmitterRender.h"
#include "HpParticleSystem.h"
#include "HpTexture2D.h"

#ifdef HP_H3_PORT
#include "H3Shader.h"
#include "H3ShaderCache.h"
static H3Shader* gShader = 0 ;
static H3_C4F gColor = {0} ;
static iS32 gPositionLocation = -1 ;
static iS32 gTexCoordLocation = -1 ;
static iS32 gColorLocation = -1 ;
static iU32 gVao = 0 ;
static iU32 gPositionVbo = 0 ;
#endif

extern HpEmitterLogic* _____mainlogic ;

HpEmitterRender::HpEmitterRender () 
{

	SCClListInit ( &m_HpEmitterRenderNodeList ) ;

    #ifdef HP_COCOS2D_PORT
	//	默认设置在父节点原点
	CCNode::setPosition(ccp(0,0)) ;		
	setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    #endif

	#ifdef HP_H3_PORT

		h3glGenVertexArrays ( 1 , &gVao ) ;
		h3glGenBuffers ( 1 , &gPositionVbo ) ;

		gShader = H3ShaderCache::getInstance()->getShader ( H3_SHADER_CACHE_PARTICLE ) ;

		#ifdef WIN32
			gPositionLocation = 0 ;
			gTexCoordLocation = 1 ;
			gColorLocation = 2 ;
		#else
			gPositionLocation = gShader->getUniformLocation ( "vertexPosition" ) ;
			gTexCoordLocation = gShader->getUniformLocation ( "vertexTexCoord" ) ;
			gColorLocation = gShader->getUniformLocation ( "vertexColor" ) ;
		#endif

	#endif
}

HpEmitterRender::~HpEmitterRender () 
{

	HpEmitterRenderNode* render = 0 ;

	for ( SCClList* walker = m_HpEmitterRenderNodeList.next ; walker ; walker = walker->next ) {

		render = (HpEmitterRenderNode* )walker->element ;

		if ( 0 == render ) 
			continue ;

		
		for ( SCClList* quawalker = render->m_HpEmitterGeometryList.next ; quawalker ; quawalker = quawalker->next ) {

			HpEmitterGeometry* geometry = (HpEmitterGeometry* )quawalker->element ;
			
			if ( 0 == geometry ) {
				continue ;
			}

		#ifdef HP_COCOS2D_PORT
			delete geometry->quadCommand ;
		#endif
			
			MMC_FREE(geometry->m_pIndices) ;
			MMC_FREE(geometry->m_pQuads) ;
			MMC_FREE(geometry) ;
			
		}
	
		SCClListClear( &render->m_HpEmitterGeometryList ) ;

		glDeleteBuffers(2, &render->m_pBuffersVBO[0]);

		delete render ;


	}

	SCClListClear( &m_HpEmitterRenderNodeList ) ;

	#ifdef HP_H3_PORT
		h3glDeleteVertexArrays ( 1 , &gVao ) ;
	#endif

}

/*
	brief : 
*/
void HpEmitterRender::insertEmitter ( HpEmitter* et ) 
{	
	initRenderNode ( et ) ;
}

/*
	brief : 
*/
void HpEmitterRender::removeEmitter ( HpEmitter* et ) 
{

	HpEmitter* tree = (HpEmitter* )et ;
	if ( NULL != tree ) {
	
		HpEmitterGeometry* geometry = (HpEmitterGeometry* )tree->geometry ;
		HpEmitterRenderNode* render = (HpEmitterRenderNode* )tree->m_render ;	
	
	#ifdef HP_COCOS2D_PORT
		delete geometry->quadCommand ;
	#endif

		MMC_FREE(geometry->m_pIndices) ;
		MMC_FREE(geometry->m_pQuads) ;
		MMC_FREE(geometry) ;
	
		SCClListDelete ( &render->m_HpEmitterGeometryList , (void* )geometry ) ;			
		//	当前render最后一个emitt，则删除render，以及纹理
		if ( 1 == SCClListEmpty ( &render->m_HpEmitterGeometryList ) ) {
			deleteRenderNode(render) ;
			SCClListDelete ( &m_HpEmitterRenderNodeList , (void* )render ) ;
		}
	
	}

}

/*
	brief : 
*/
void HpEmitterRender::run (float dt)
{

	for ( SCClList* walker = m_HpEmitterRenderNodeList.next ; walker ; walker = walker->next ) {

		HpEmitterRenderNode* render = (HpEmitterRenderNode* )walker->element ;

		if ( 0 == render ) 
			continue ;  

		if ( render->m_timecost >= render->m_framelist[render->m_currentFrame].m_duration ) {

			if ( false == render->m_frameLoop ) {
				if ( render->m_currentFrame == render->m_totallFrame-1 ) {
					render->m_timecost = 0 ;
					continue ; 
				} else {
					render->m_timecost = 0 ;
					render->m_currentFrame ++ ;
				}
			} else {
				render->m_currentFrame = ( render->m_currentFrame + 1 ) % render->m_totallFrame ;
				render->m_timecost = 0 ;
			}

			render->m_handle = (void* )render->m_framelist[render->m_currentFrame].m_handle ;

		} else {
			render->m_timecost = render->m_timecost + dt ;
		}		

	}


#if 0
#ifdef ALWAYS_CREAT_VBO

	for ( SCClList* walker = m_HpEmitterRenderNodeList.next ; walker ; walker = walker->next ) {

		HpEmitterRenderNode* render = (HpEmitterRenderNode* )walker->element ;

		if ( 0 == render ) 
			continue ;  

		CCTexture2D* texture = (CCTexture2D* )render->m_handle ;
		ccGLBindTexture2D( texture->getName() );

		for ( SCClList* quawalker = render->m_HpEmitterGeometryList.next ; quawalker ; quawalker = quawalker->next ) {

			HpEmitterGeometry* geometry = (HpEmitterGeometry* )quawalker->element ;
			HpEmitter* et = 0 ;

			if ( 0 == geometry ) 
				continue ;   

			et = (HpEmitter* )geometry->m_emitt_handle ;

			if ( 0 == et ) 
				continue ;
			   
			if ( 0 == et->m_emittCounter ) 
				continue ;
	
			if ( false == et->m_actived ) 
				continue ;  

			glBindBuffer(GL_ARRAY_BUFFER, render->m_pBuffersVBO[0]);
 
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ccV3F_C4B_T2F_Quad)*et->m_emittCounter , geometry->m_pQuads);			
    
			CHECK_GL_ERROR_DEBUG();

		}

	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

#endif
#endif
	  
}

/*
	brief : 
*/
#ifdef HP_COCOS2D_PORT
void HpEmitterRender::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
#else
void HpEmitterRender::draw( float dt )
#endif
{

#ifdef HP_COCOS2D_PORT
	//	for cocos2dx 3.x.x
	for ( SCClList* walker = m_HpEmitterRenderNodeList.next ; walker ; walker = walker->next ) {

		HpEmitterRenderNode* render = (HpEmitterRenderNode* )walker->element ;

		if ( 0 == render ) 
			continue ;

		HpTexture2D* texture = (HpTexture2D* )render->m_handle ;
		ccGLBindTexture2D( texture->getId() );

		for ( SCClList* quawalker = render->m_HpEmitterGeometryList.next ; quawalker ; quawalker = quawalker->next ) {

			HpEmitterGeometry* geometry = (HpEmitterGeometry* )quawalker->element ;
			HpEmitter* et = (HpEmitter* ) geometry->m_emitt_handle ;

			if ( 0 == geometry || 0 == et ) 
				continue ;

			if ( 0 == et->m_emittCounter ) 
				continue ;

			if ( false == et->m_actived ) 
				continue ;

			if ( false == et->m_visible )
				continue ;

			//ccGLBlendFunc( et->m_BlendFuncSrc , et->m_BlendFuncDst );
			BlendFunc _blendFunc = { (GLenum)et->m_BlendFuncSrc , (GLenum)et->m_BlendFuncDst } ;
			//BlendFunc _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
			geometry->quadCommand->init(_globalZOrder, (unsigned int)texture->getId(), getGLProgramState(), _blendFunc,(V3F_C4B_T2F_Quad*)geometry->m_pQuads, et->m_emittCounter, transform, flags);
			renderer->addCommand(geometry->quadCommand);
		


		}

	}

#endif

#ifdef HP_H3_PORT

	h3glBindVertexArray ( gVao ) ;
	//h3glClearColor(0.5, 0.5, 0.5, 1.0);
	h3glClearColor(0.0, 0.0, 0.0, 1.0);
	h3glClear(GL_COLOR_BUFFER_BIT);
	h3glEnable(GL_BLEND);
	glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_SRC_ALPHA);
			
	for ( SCClList* walker = m_HpEmitterRenderNodeList.next ; walker ; walker = walker->next ) {

		HpEmitterRenderNode* render = (HpEmitterRenderNode* )walker->element ;

		if ( 0 == render ) 
			continue ;

		gShader->use () ;
		gShader->setUniformsForBuiltins () ;
		iS32 slot = gShader->getUniformLocation ( "diffuseTexture" ) ; 
		if ( -1 != slot ) {
		
			HpTexture2D* texture = (HpTexture2D* )render->m_handle ;
			h3glActiveTexture ( GL_TEXTURE0 ) ;
			h3glBindTexture ( GL_TEXTURE_2D , texture->getId () );
			gShader->setUniformLocationWith1i ( slot , GL_TEXTURE0-GL_TEXTURE0 ) ;

		}  
		
		h3glEnableVertexAttribArray ( gPositionLocation ) ;
		h3glEnableVertexAttribArray ( gTexCoordLocation ) ; 
		h3glEnableVertexAttribArray ( gColorLocation ) ;

		for ( SCClList* quawalker = render->m_HpEmitterGeometryList.next ; quawalker ; quawalker = quawalker->next ) {

			HpEmitterGeometry* geometry = (HpEmitterGeometry* )quawalker->element ;
			HpEmitter* et = (HpEmitter* ) geometry->m_emitt_handle ;

			if ( 0 == geometry || 0 == et ) 
				continue ;
			if ( 0 == et->m_emittCounter ) 
				continue ;

			if ( false == et->m_actived ) 
				continue ;

			if ( false == et->m_visible )
				continue ;

			h3glBlendFunc ( et->m_BlendFuncSrc , et->m_BlendFuncDst ) ;  

			#define kQuadSize sizeof(H3_Quad_C4F_3)

			glBindBuffer(GL_ARRAY_BUFFER, render->m_pBuffersVBO[0]);	
#ifndef ALWAYS_CREAT_VBO
			glBufferData(GL_ARRAY_BUFFER, sizeof(geometry->m_pQuads[0]) * et->m_emittCounter , geometry->m_pQuads , GL_DYNAMIC_DRAW);
#else
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(geometry->m_pQuads[0])*et->m_emittCounter , geometry->m_pQuads);
#endif
			// vertices
			glVertexAttribPointer(gPositionLocation, 3, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( H3_Quad_C4F_3, vertex));
			// colors
			glVertexAttribPointer(gTexCoordLocation, 2, GL_FLOAT, GL_TRUE, kQuadSize, (GLvoid*) offsetof( H3_Quad_C4F_3, texCoord));
			// tex coords
			glVertexAttribPointer(gColorLocation, 4, GL_FLOAT, GL_FALSE, kQuadSize, (GLvoid*) offsetof( H3_Quad_C4F_3, color));
 
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render->m_pBuffersVBO[1]);
#ifndef ALWAYS_CREAT_VBO
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(geometry->m_pIndices[0]) * et->m_emittCounter * 6 , geometry->m_pIndices, GL_STATIC_DRAW);
#endif
			glDrawElements(GL_TRIANGLES, (GLsizei) et->m_emittCounter*6, GL_UNSIGNED_SHORT, 0);

		}

		h3glDisableVertexAttribArray ( gPositionLocation ) ;
		h3glDisableVertexAttribArray ( gTexCoordLocation ) ; 
		h3glDisableVertexAttribArray ( gColorLocation ) ;

	}

    //  release
	h3glActiveTexture ( 0 ) ;
	h3glBindTexture ( GL_TEXTURE_2D , 0 ) ;
    h3glBindBuffer ( GL_ARRAY_BUFFER , 0 ) ;
    h3glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER , 0 ) ;
	h3glBindVertexArray ( 0 ) ;

	H3_GL_DRAW_RECORD_DRAWCALL(1)
	H3_GL_ERROR_CHECK()


#endif

}

/* 
	brief : 
*/
void HpEmitterRender::initRenderNode ( HpEmitter* et )
{

	HpEmitterRenderNode* renderNode = 0 ; 
	
	if ( NULL == et ) {
		return ; 
	}

	if ( NULL == et->m_data ) {
		return ;
	}

#ifndef ALWAYS_CREAT_VBO
	renderNode = getRenderNode ( HpEmitterContainer::getTexture(&et->m_hpeTexture) ) ;
#else
	renderNode = NULL ;
#endif

	//	met a new texture
	//	insert a new data here
	if ( NULL == renderNode ) {
		
		renderNode = new HpEmitterRenderNode () ;
		if ( 0 == renderNode ) {
			iLog ( "Hp Error : Momory Malloc Failed , file:%s,line : %d" ,__FILE__,__LINE__) ;
			return ;
		}
		
		renderNode->m_handle = HpEmitterContainer::getTexture(&et->m_hpeTexture) ;

		//	generate VBO 
		glGenBuffers(2, &renderNode->m_pBuffersVBO[0]);

		//	frame
		memcpy ( renderNode->m_framelist , et->m_hpeTexture.m_framelist , sizeof(et->m_hpeTexture.m_framelist) ) ;
		renderNode->m_totallFrame = et->m_hpeTexture.m_totallFrame ;
		renderNode->m_frameLoop = et->m_hpeTexture.frameLoop ;
		renderNode->m_currentFrame = 0 ;
		renderNode->m_timecost = 0.0 ;

		//	add new render node
		SCClListInsert ( &m_HpEmitterRenderNodeList , (void* ) renderNode ) ;

	}

	HpEmitterGeometry* hpEmitterGeometry = 0 ;
	hpEmitterGeometry = (HpEmitterGeometry* ) MMC_MALLOC ( sizeof(HpEmitterGeometry) ) ;
	if ( 0 == hpEmitterGeometry ) {
		iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
		return ;
	}
	hpEmitterGeometry->m_emitt_handle = et ;

#ifdef HP_COCOS2D_PORT
	hpEmitterGeometry->m_pQuads = (V3F_C4B_T2F_Quad* ) MMC_MALLOC ( sizeof(V3F_C4B_T2F_Quad)*et->m_emittTotall ) ;
#endif
#ifdef HP_H3_PORT
	hpEmitterGeometry->m_pQuads = (H3_V3F_T2F_C4F_Quad* ) MMC_MALLOC ( sizeof(H3_V3F_T2F_C4F_Quad)*et->m_emittTotall ) ;
#endif
	if ( 0 == hpEmitterGeometry->m_pQuads ) {
		iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
		return ;
	}
	hpEmitterGeometry->m_totallQuads = et->m_emittTotall ;
#ifdef HP_COCOS2D_PORT	
	memset ( hpEmitterGeometry->m_pQuads , 0 , sizeof(V3F_C4B_T2F_Quad)*et->m_emittTotall ) ;
#endif
#ifdef HP_H3_PORT
	memset ( hpEmitterGeometry->m_pQuads , 0 , sizeof(H3_V3F_T2F_C4F_Quad)*et->m_emittTotall ) ;
#endif
	
	hpEmitterGeometry->m_pIndices = (GLushort* ) MMC_MALLOC ( sizeof(GLushort)*et->m_emittTotall*6 ) ;
	if ( 0 == hpEmitterGeometry->m_pIndices ) {
		iLog ( "Hp Error : Momory Malloc Failed , file:%s , line : %d" ,__FILE__,__LINE__) ;
		return ;
	}
	hpEmitterGeometry->m_totallIndices = et->m_emittTotall*6 ;
	memset ( hpEmitterGeometry->m_pIndices , 0 , sizeof(GLushort)*et->m_emittTotall*6 ) ;

#ifdef HP_COCOS2D_PORT
	hpEmitterGeometry->quadCommand = new QuadCommand () ;
#endif

	for ( int looper = 0 ; looper < et->m_hpeTexture.m_totallFrame ; looper ++ ) {

		HpTexture2D* texture = (HpTexture2D* )et->m_hpeTexture.m_framelist[looper].m_handle ;
		texture->addRef () ;
		
	}
	//jelo just for temporatory
	HpTexture2D* texture = (HpTexture2D* )HpEmitterContainer::getTexture(&et->m_hpeTexture) ;

	HpeTextureAtom* atom = (HpeTextureAtom* )HpEmitterContainer::getTextureAtom ( &et->m_hpeTexture ) ;
	HpSize size = texture->getContentSize () ;
	HpRect rect( atom->m_rect.x1 , atom->m_rect.y1 , atom->m_rect.x2 , atom->m_rect.y2 ) ;
	
	GLfloat left = rect.origin.x / size.width;
	GLfloat bottom = rect.origin.y / size.height;
	GLfloat right = left + rect.size.width / size.width;
	GLfloat top = bottom + rect.size.height / size.height;

	//	bind quads point to particle
	//	and init texCoord
	for ( int looper = 0 ; looper < et->m_emittTotall ; looper ++ ) {
		
		HpParticle* particle = &et->m_data[looper] ;
		//	bind
		particle->m_pQuads = &hpEmitterGeometry->m_pQuads[looper] ;

		if ( 0 == particle || 0 == particle->m_pQuads ) {
			break ;
		}

		HP_TEXCOORD_SET_BL_V(particle,left,bottom) ;
		HP_TEXCOORD_SET_BR_V(particle,right,bottom) ;
		HP_TEXCOORD_SET_TL_V(particle,left,top) ;
		HP_TEXCOORD_SET_TR_V(particle,right,top) ;

	}

	//	init indices
	for(int i = 0; i < et->m_emittTotall ; ++i)
    {
        const unsigned int i6 = i*6;
        const unsigned int i4 = i*4;
        hpEmitterGeometry->m_pIndices[i6+0] = (GLushort) i4+0;
        hpEmitterGeometry->m_pIndices[i6+1] = (GLushort) i4+1;
        hpEmitterGeometry->m_pIndices[i6+2] = (GLushort) i4+2;

        hpEmitterGeometry->m_pIndices[i6+5] = (GLushort) i4+1;
        hpEmitterGeometry->m_pIndices[i6+4] = (GLushort) i4+2;
        hpEmitterGeometry->m_pIndices[i6+3] = (GLushort) i4+3;
    }

	//	add new Geometry Node
	SCClListInsert ( &renderNode->m_HpEmitterGeometryList , (void* ) hpEmitterGeometry ) ;

	et->m_render = (void* )renderNode ;
	et->geometry = (void* )hpEmitterGeometry ;

#ifdef ALWAYS_CREAT_VBO
	glBindBuffer(GL_ARRAY_BUFFER, renderNode->m_pBuffersVBO[0]);		
	glBufferData(GL_ARRAY_BUFFER, sizeof(hpEmitterGeometry->m_pQuads[0]) * et->m_emittTotall , hpEmitterGeometry->m_pQuads , GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderNode->m_pBuffersVBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(hpEmitterGeometry->m_pIndices[0]) * et->m_emittTotall * 6 , hpEmitterGeometry->m_pIndices, GL_STATIC_DRAW) ;
#endif

}

/*
	brief : 
*/
void HpEmitterRender::deleteRenderNode ( HpEmitterRenderNode* render ) 
{
	glDeleteBuffers(2, &render->m_pBuffersVBO[0]);
	delete render ;
}

/*
	brief : 
*/
HpEmitterRenderNode* HpEmitterRender::getRenderNode ( void* textureid ) 
{

	HpEmitterRenderNode* render = 0 ;

	for ( SCClList* walker = m_HpEmitterRenderNodeList.next ; walker ; walker = walker->next ) {

		render = (HpEmitterRenderNode* )walker->element ;

		if ( 0 == render ) 
			continue ;
        
		if ( textureid == render->m_handle ) {
			return render ;
		}
	
	}

	return 0 ;

} 