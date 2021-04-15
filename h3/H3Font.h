
/*

	+	H3Font.h
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

#ifndef H3Font_H
#define H3Font_H

#include <stdio.h>
#include "H3Std.h"
#include "H3Memory.h"
#include "H3MathUtil.h"
#include "H3Texture.h"

using namespace std ;

namespace H3Font {

	class TexCoord
	{
	public :
		TexCoord () {}
		~TexCoord () {}
		void operator = ( const TexCoord& texCoord ) {
			this->U0 = texCoord.U0 ;
			this->V0 = texCoord.V0 ;
			this->U1 = texCoord.U1 ;
			this->V1 = texCoord.V1 ;
		}
		float U0 ;
		float U1 ;
		float V0 ;
		float V1 ;
	} ;

	void* init ( iS8* ttf ) ;

	void release () ;

	void draw ( H3Vector3f position , iS8* string ) ;

	TexCoord getTexCoord ( unsigned short c ) ;

	void* Texture () ;

	void* Data () ;

	int TextureWidth () ;

	int TextureHeight () ;

	int getWidth ( const iS8* words )  ;


} ;

#endif