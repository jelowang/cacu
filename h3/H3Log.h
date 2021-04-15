
/*

	+	H3Log.h
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

#ifndef H3Log_H
#define H3Log_H
  
#include <memory.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#ifdef H3_ANDROID
#include <android/log.h>   
#endif

#define LOG

#ifdef LOG
	#ifdef H3_ANDROID
		#define iLog(...) __android_log_print ( ANDROID_LOG_ERROR , "QDL-H3" , __VA_ARGS__ )  
	#else
		extern void H3Log ( int module , const char* log , ... ) ;
		#define iLog(...) H3Log  ( 0 , __VA_ARGS__ )
	#endif
#else
	#define iLog(...)
#endif

#endif
