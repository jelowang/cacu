
/*

	+	H3gl.h
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3_OPENGLAPI
#define H3_OPENGLAPI

#include "H3Std.h"

#ifdef H3_QT_5_4_2
	//	see the defination at H3Delegate
	#define glHandle Opengl.
#else
	#define glHandle 
#endif

#if defined(H3DEBUG)
	#define H3_GL_ERROR_CHECK() \
		{ \
			GLenum h3glerror = glGetError(); \
			if(h3glerror) { \
				iLog("OpenGL error 0x%x in %s %d", h3glerror, __FUNCTION__, __LINE__); \
			} \
		}
#else
	#define H3_GL_ERROR_CHECK()
#endif

#define h3glGetString(q) glHandle glGetString(q)
//	state
#define h3glClearColor(r,g,b,a) glHandle glClearColor(r,g,b,a)
#define h3glFrontFace(v) glHandle glFrontFace(v) 
#define h3glCullFace(v) glHandle glCullFace(v)
#define h3glEnable(v) glHandle glEnable(v) 
#define h3glDisable(v) glHandle glDisable(v) 
#define h3glClear(v) glHandle glClear(v) 
#define h3glBlendFunc(v1,v2) glHandle glBlendFunc(v1,v2)
#define h3glViewport(x,y,width,height) glHandle glViewport(x,y,width,height)
#define h3glDepthFunc(x) glHandle glDepthFunc(x) 

//	vao
#ifndef H3_ANDROID
#define h3glGenVertexArrays(t,v) glHandle glGenVertexArrays(t,v) 
#define h3glBindVertexArray(v) glHandle glBindVertexArray(v) 
#define h3glDeleteVertexArrays(t,v) glHandle glDeleteVertexArrays(t,v) 
#else
#define h3glGenVertexArrays(t,v) 
#define h3glBindVertexArray(v) 
#define h3glDeleteVertexArrays(t,v)  
#endif

//	vbo,fbo,rbo
#define h3glGenBuffers(t,v) glHandle glGenBuffers(t,v) 
#define h3glDeleteBuffers(t,v) glHandle glDeleteBuffers(t,v) 
#define h3glBindBuffer(t,v) glHandle glBindBuffer(t,v) 
#define h3glBufferData(t,l,d,s) glHandle glBufferData(t,l,d,s)
#define h3glEnableVertexAttribArray(v) glHandle glEnableVertexAttribArray(v)
#define h3glVertexAttribPointer(i,s,t,n,st,p) glHandle glVertexAttribPointer(i,s,t,n,st,p)
#define h3glDrawElements(m,c,t,i) glHandle glDrawElements(m,c,t,i)
#define h3glDisableVertexAttribArray(v) glHandle glDisableVertexAttribArray(v)
//	fbo
#define h3glGenFramebuffers(c,h) glHandle glGenFramebuffers(c,h)
#define h3glDeleteFramebuffers(c,h) glHandle glDeleteFramebuffers(c,h)
#define h3glFramebufferTexture2D(p1,p2,p3,p4,p5) glHandle glFramebufferTexture2D(p1,p2,p3,p4,p5)
#define h3glFramebufferRenderbuffer(p1,p2,p3,p4) glHandle glFramebufferRenderbuffer(p1,p2,p3,p4)
#define h3glBindFramebuffer(c,h) glHandle glBindFramebuffer(c,h)
#define h3glFramebufferTexture2D(target,attachment,textarget,texture,level) glHandle glFramebufferTexture2D(target,attachment,textarget,texture,level)
//	rbo
#define h3glGenRenderbuffers(c,h) glHandle glGenRenderbuffers(c,h)
#define h3glDeleteRenderbuffers(c,h) glHandle glDeleteRenderbuffers(c,h)
#define h3glBindRenderbuffer(p1,p2)	glHandle glBindRenderbuffer(p1,p2)
#define h3glRenderbufferStorage(p1,p2,p3,p4) glHandle glRenderbufferStorage(p1,p2,p3,p4)

#define h3glDrawBuffer(h) glHandle glDrawBuffer(h)
#define h3glReadBuffer(h) glHandle glReadBuffer(h)

//	shader
#define h3glGetAttribLocation(p,n) glHandle glGetAttribLocation(p,n)
#define h3glGetUniformLocation(p,t) glHandle glGetUniformLocation(p,t)
#define h3glUniform1i(s,z) glHandle glUniform1i(s,z) 
#define h3glUniform1f(s,z) glHandle glUniform1f(s,z) 
#define h3glUniform3fv(s,c,z) glHandle glUniform3fv(s,c,z) 

//	textures
#define h3glGenTextures(n,p) glHandle glGenTextures(n,p)
#define h3glDeleteTextures(n,p) glHandle glDeleteTextures(n,p)
#define h3glBindTexture(t,p) glHandle glBindTexture (t,p)
#define h3glActiveTexture(t) glHandle glActiveTexture(t) 
#define h3glTexParameteri(t,f,l) glHandle glTexParameteri(t,f,l)
#define h3glTexParameterfv(target,pname,params) glHandle glTexParameterfv(target,pname,params)
#define h3glTexImage2D(t,z00,c00,w,h,z01,c01,a,d) glHandle glTexImage2D(t,z00,c00,w,h,z01,c01,a,d)
#define h3glTexSubImage2D(target,level,xoffset,yoffset,width,height,format,type,pixels) glHandle glTexSubImage2D ( target,level,xoffset,yoffset,width,height,format,type,pixels ) 
#define h3glGetIntegerv(target,t) glHandle glGetIntegerv(target,t)
#define h3glPixelStorei(pname,param) glHandle glPixelStorei(pname,param)

#if defined(H3OPENGL)
#define H3_GL_DRAW_RECORD_DRAWCALL(n) \
	H3Delegate::getInstance()->recordDrawCall(n);
#define H3_GL_DRAW_RECORD_VERTICES(n) \
	H3Delegate::getInstance()->recordVertices(n);
#else
#define H3_GL_DRAW_RECORD_DRAWCALL(n)
#define H3_GL_DRAW_RECORD_VERTICES(n)
#endif

#endif