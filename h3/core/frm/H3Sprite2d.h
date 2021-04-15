
/*

	+	H3Sprite2d.h
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

#ifndef H3Sprite2d_H
#define H3Sprite2d_H

#include "H3Std.h"
#include "H3Object.h"

class H3Sprite2d : public H3Object , virtual public H3Memory 
{
public :
	H3Sprite2d () ;
	~H3Sprite2d () ;
} ;

#endif