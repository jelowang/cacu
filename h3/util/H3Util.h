
/*

	+	H3Util.h
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

#ifndef H3Util_H
#define H3Util_H

#include "H3Std.h"

extern string H3UtilGbk2Utf8 ( const string &str ) ;
extern int H3UtilGetUtf8Length ( char* str ) ;
extern int H3UtilConvertCharFromUtf8 (unsigned int* out_char, const char* in_text, const char* in_text_end) ;

#endif