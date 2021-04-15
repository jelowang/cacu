
/*

	+	H3DrawPrimitives.h
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

#ifndef H3DrawPrimitives_H
#define H3DrawPrimitives_H

#include "H3Std.h"
#include "H3MathUtil.h"

typedef enum {
	H3DrawPrimitives_Point ,
	H3DrawPrimitives_Line , 
	H3DrawPrimitives_Triangle ,
} H3DrawPrimitivesType ;

class H3DrawPrimitives 
{
public :
	static void init () ;
	static void release();
	static void setColor(iF32 r, iF32 g, iF32 b, iF32 a);
	static void addVertex(iF32 x, iF32 y, iF32 z);
	static void drawCircle ( iF32 x, iF32 y, iF32 z , iF32 r );
	static void drawCylinderWire ( iF32 minx , iF32 miny , iF32 minz , iF32 maxx , iF32 maxy , iF32 maxz ) ;
	static void drawArc ( iF32 x0 , iF32 y0 , iF32 z0 , iF32 x1 , iF32 y1 , iF32 z1 , iF32 h , iF32 as0 , iF32 as1 ) ;
	static void drawLine(H3Vector3f origin, H3Vector3f destination);
	static void begin(H3DrawPrimitivesType type);
	static void draw();
	static void end();
	static void setDepthTestEnable(bool enable);
						 
} ;

#endif