#ifndef HP_ADAPTER_H
#define HP_ADAPTER_H

#if 0
//#if ((CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_IOS))
    #include "cocos2d.h"
    //#include "CCGL.h"
    USING_NS_CC;
	#define HP_COCOS2D_PORT
#else
	#define HP_H3_PORT
#ifndef WIN32
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>  
#endif
	#include "H3gl.h"

#endif

#include <assert.h>
#include <memory.h>
#include <string.h>
#include "H3Memory.h"
#include "H3Log.h"
#include "H3Type.h"
#include "HpUtil.h"

#ifdef HP_COCOS2D_PORT
//	vertex
#define HP_VERTEX_SET_TL_V(d,xx,yy,zz)\
	d->m_pQuads->tl.vertices.x = xx ; \
	d->m_pQuads->tl.vertices.y = yy ; \
	d->m_pQuads->tl.vertices.z = zz ;
#define HP_VERTEX_SET_TR_V(d,xx,yy,zz)\
	d->m_pQuads->tr.vertices.x = xx ; \
	d->m_pQuads->tr.vertices.y = yy ; \
	d->m_pQuads->tr.vertices.z = zz ;
#define HP_VERTEX_SET_BL_V(d,xx,yy,zz)\
	d->m_pQuads->bl.vertices.x = xx ; \
	d->m_pQuads->bl.vertices.y = yy ; \
	d->m_pQuads->bl.vertices.z = zz ;
#define HP_VERTEX_SET_BR_V(d,xx,yy,zz)\
	d->m_pQuads->br.vertices.x = xx ; \
	d->m_pQuads->br.vertices.y = yy ; \
	d->m_pQuads->br.vertices.z = zz ;

//	color
#define HP_COLOR_SET_TL_V(d,s)\
	d->m_pQuads->tl.colors.r = s->m_color.m_deltaColor.r * 255.0f ; \
	d->m_pQuads->tl.colors.g = s->m_color.m_deltaColor.g * 255.0f ; \
	d->m_pQuads->tl.colors.b = s->m_color.m_deltaColor.b * 255.0f ; \
	d->m_pQuads->tl.colors.a = s->m_color.m_deltaColor.a * 255.0f ;
#define HP_COLOR_SET_TR_V(d,s)\
	d->m_pQuads->tr.colors.r = s->m_color.m_deltaColor.r * 255.0f ; \
	d->m_pQuads->tr.colors.g = s->m_color.m_deltaColor.g * 255.0f ; \
	d->m_pQuads->tr.colors.b = s->m_color.m_deltaColor.b * 255.0f ; \
	d->m_pQuads->tr.colors.a = s->m_color.m_deltaColor.a * 255.0f ;
#define HP_COLOR_SET_BL_V(d,s)\
	d->m_pQuads->bl.colors.r = s->m_color.m_deltaColor.r * 255.0f ; \
	d->m_pQuads->bl.colors.g = s->m_color.m_deltaColor.g * 255.0f ; \
	d->m_pQuads->bl.colors.b = s->m_color.m_deltaColor.b * 255.0f ; \
	d->m_pQuads->bl.colors.a = s->m_color.m_deltaColor.a * 255.0f ;
#define HP_COLOR_SET_BR_V(d,s)\
	d->m_pQuads->br.colors.r = s->m_color.m_deltaColor.r * 255.0f ; \
	d->m_pQuads->br.colors.g = s->m_color.m_deltaColor.g * 255.0f ; \
	d->m_pQuads->br.colors.b = s->m_color.m_deltaColor.b * 255.0f ; \
	d->m_pQuads->br.colors.a = s->m_color.m_deltaColor.a * 255.0f ;

//	texCoord 
#define HP_TEXCOORD_SET_TL_V(d,x,y)\
	d->m_pQuads->tl.texCoords.u = x ; \
	d->m_pQuads->tl.texCoords.v = y ; 
#define HP_TEXCOORD_SET_TR_V(d,x,y)\
	d->m_pQuads->tr.texCoords.u = x ; \
	d->m_pQuads->tr.texCoords.v = y ; 
#define HP_TEXCOORD_SET_BL_V(d,x,y)\
	d->m_pQuads->bl.texCoords.u = x ; \
	d->m_pQuads->bl.texCoords.v = y ; 
#define HP_TEXCOORD_SET_BR_V(d,x,y)\
	d->m_pQuads->br.texCoords.u = x ; \
	d->m_pQuads->br.texCoords.v = y ; 

#endif

#ifdef HP_H3_PORT
//	vertex
#define HP_VERTEX_SET_TL_V(d,xx,yy,zz)\
	d->m_pQuads->tl.vertex.x = xx ; \
	d->m_pQuads->tl.vertex.y = yy ; \
	d->m_pQuads->tl.vertex.z = zz ;
#define HP_VERTEX_SET_TR_V(d,xx,yy,zz)\
	d->m_pQuads->tr.vertex.x = xx ; \
	d->m_pQuads->tr.vertex.y = yy ; \
	d->m_pQuads->tr.vertex.z = zz ;
#define HP_VERTEX_SET_BL_V(d,xx,yy,zz)\
	d->m_pQuads->bl.vertex.x = xx ; \
	d->m_pQuads->bl.vertex.y = yy ; \
	d->m_pQuads->bl.vertex.z = zz ;
#define HP_VERTEX_SET_BR_V(d,xx,yy,zz)\
	d->m_pQuads->br.vertex.x = xx ; \
	d->m_pQuads->br.vertex.y = yy ; \
	d->m_pQuads->br.vertex.z = zz ;

//	color
#define HP_COLOR_SET_TL_V(d,s)\
	d->m_pQuads->tl.color.r = s->m_color.m_deltaColor.r ; \
	d->m_pQuads->tl.color.g = s->m_color.m_deltaColor.g ; \
	d->m_pQuads->tl.color.b = s->m_color.m_deltaColor.b ; \
	d->m_pQuads->tl.color.a = s->m_color.m_deltaColor.a ;
#define HP_COLOR_SET_TR_V(d,s)\
	d->m_pQuads->tr.color.r = s->m_color.m_deltaColor.r ; \
	d->m_pQuads->tr.color.g = s->m_color.m_deltaColor.g ; \
	d->m_pQuads->tr.color.b = s->m_color.m_deltaColor.b ; \
	d->m_pQuads->tr.color.a = s->m_color.m_deltaColor.a ;
#define HP_COLOR_SET_BL_V(d,s)\
	d->m_pQuads->bl.color.r = s->m_color.m_deltaColor.r ; \
	d->m_pQuads->bl.color.g = s->m_color.m_deltaColor.g ; \
	d->m_pQuads->bl.color.b = s->m_color.m_deltaColor.b ; \
	d->m_pQuads->bl.color.a = s->m_color.m_deltaColor.a ;
#define HP_COLOR_SET_BR_V(d,s)\
	d->m_pQuads->br.color.r = s->m_color.m_deltaColor.r ; \
	d->m_pQuads->br.color.g = s->m_color.m_deltaColor.g ; \
	d->m_pQuads->br.color.b = s->m_color.m_deltaColor.b ; \
	d->m_pQuads->br.color.a = s->m_color.m_deltaColor.a ;

//	texCoord
#define HP_TEXCOORD_SET_TL_V(d,x,y)\
	d->m_pQuads->tl.texCoord.u = x ; \
	d->m_pQuads->tl.texCoord.v = y ; 
#define HP_TEXCOORD_SET_TR_V(d,x,y)\
	d->m_pQuads->tr.texCoord.u = x ; \
	d->m_pQuads->tr.texCoord.v = y ; 
#define HP_TEXCOORD_SET_BL_V(d,x,y)\
	d->m_pQuads->bl.texCoord.u = x ; \
	d->m_pQuads->bl.texCoord.v = y ; 
#define HP_TEXCOORD_SET_BR_V(d,x,y)\
	d->m_pQuads->br.texCoord.u = x ; \
	d->m_pQuads->br.texCoord.v = y ; 

#endif

#endif