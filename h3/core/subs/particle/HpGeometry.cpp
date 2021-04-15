#if 0
/*
+	HpeGeometry.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#include "HpGeometry.h"

HpVector& HpGeometryTranform2OrthoPlane( float x , float y , float viewWidth , float viewHeight , float left , float right , float bottom , float top ) 
{
	static HpVector vector ;

	float xa = (left-right)*((viewWidth-x)/viewWidth)+right ;
	float ya = (bottom-top)*((viewHeight-y)/viewHeight)+top ;

	vector.m_x = xa ;
	vector.m_y = ya ;

	return vector ;

}
#endif