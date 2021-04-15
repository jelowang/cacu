
/*

	+	H3Std.h
	+	QDL

	+	hello3d
	+	realtime 3d engine
																				
 
	+	(C)	Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3Std_H
#define H3Std_H

#define H3_OPENGL

#ifdef WIN32
#define H3_GLWF
#else
#define H3_ANDROID
//#define H3_QT_5_4_2
//#define H3_IOS
//#define H3_ANDROID
#endif
 
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <fstream>  
#include <string>
#include <vector>
#include <map>

using namespace std ;

#ifdef H3_IOS
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#endif

#ifdef H3_QT_5_4_2
	#include <QOpenGLShaderProgram>  
	#include <QOpenGLFunctions_3_3_Core>  
	#include <QGLFunctions>
	#include <QOpenGLBuffer>
	#include <QOpenGLVertexArrayObject>
	#include <QOpenGLShaderProgram>
	#ifdef H3_QT_5_4_2
	//	see the defination at H3Delegate
	extern QOpenGLFunctions_3_3_Core Opengl ;
	#endif
#endif

#ifdef H3_GLWF

	#include <GL/glew.h>
	#include <GLFW/glfw3.h>
	extern GLEWContext* glewGetContext() ;
	#ifdef _WIN32
	#undef APIENTRY
	#define GLFW_EXPOSE_NATIVE_WIN32
	#define GLFW_EXPOSE_NATIVE_WGL
	#include <GLFW/glfw3native.h>
	#endif

	#include <imgui.h>
	#include "imgui_impl_glfw_gl3.h"

#endif

#ifdef H3_ANDROID
	#include <EGL/egl.h>
	#include <GLES2/gl2.h>  
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include "H3Memory.h"
#include "H3Event.h"
#include "H3Log.h"
#include "schal.h"
#include "sccl.h"
#include "H3Util.h"
#include "H3Type.h"

#endif 