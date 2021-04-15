
/*

	+	H3Math.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Std.h"
#include "H3Math.h"
#include "H3Matrix.h"
#include "H3Vector.h"

static float g_sinTable [360] = {0} ;
static float g_cosTable [360] = {0} ;
static bool g_inited = false ;

static double shift( double x) 
{
    //always wrap input angle to -PI..PI
    while (x < -3.14159265)
        x += 6.28318531;
    while (x > 3.14159265)
        x -= 6.28318531;
    return x;
}

#define CC_SWAP(x, y, type)    \
{    type temp = (x);        \
    x = y; y = temp;        \
}


/*
brief :
*/
float H3MathWrapPi( float theta )
{
	theta = theta + H3Pi ;
	theta = theta - floor( theta*H31Over2Pi ) * H3Pi ;
	theta = theta - H3Pi ;

	return theta ;

}

/*
brief :
*/
void H3MatchInit () 
{
	if ( true == g_inited ) {
		return ;
	}

	for ( int looper = 0 ; looper < 360 ; looper ++ ) {
		g_sinTable[looper] = sinf(H3ToRadian(looper)) ;
		g_cosTable[looper] = cosf(H3ToRadian(looper)) ;
	}
	g_inited = true ;
}

/*
brief :
*/
float H3Clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        CC_SWAP(min_inclusive, max_inclusive, float);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

/*
brief :
*/
double H3Sine(double x) {

#if 0
	HpMatchInit () ;

	float degree = HP_ADAPTER_HRADIANS_TO_DEGREES(x) ;
	degree = HpClampf(degree,0,360) ;
	int index = (int )floor(degree) ;
	return g_sinTable[index] ;
#else
    double sin = 0;
    x = shift( x );
    //compute sine
    if (x < 0) {
        sin = 1.27323954 * x + .405284735 * x * x;
 
        if (sin < 0)
            sin = .225 * (sin * -sin - sin) + sin;
        else
            sin = .225 * (sin * sin - sin) + sin;
    } else {
        sin = 1.27323954 * x - 0.405284735 * x * x;
 
        if (sin < 0)
            sin = .225 * (sin * -sin - sin) + sin;
        else
            sin = .225 * (sin * sin - sin) + sin;
    }
    return sin;
#endif
}

/*
brief :
*/ 
double H3Cosine( double x) {

#if 0
	HpMatchInit () ;

	float degree = HP_ADAPTER_HRADIANS_TO_DEGREES(x) ;
	degree = HpClampf(degree,0,360) ;
	int index = (int )floor(degree) ;
	return g_cosTable[index] ;
#else
    double cos = 0;
    //compute cosine: sin(x + PI/2) = cos(x)
    x += 1.57079632;
    if (x > 3.14159265)
        x -= 6.28318531;
 
    if (x < 0) {
        cos    = 1.27323954 * x + 0.405284735 * x * x;
 
        if (cos < 0)
            cos = .225 * (cos *-cos - cos) + cos;
        else
            cos = .225 * (cos * cos - cos) + cos;
    }
    else
    {
        cos = 1.27323954 * x - 0.405284735 * x * x;
 
        if (cos < 0)
            cos = .225 * (cos *-cos - cos) + cos;
        else
            cos = .225 * (cos * cos - cos) + cos;
    }
    return cos;
#endif
}

/*
brief :
*/
float H3Sqrt3(const float x)  
{
  union
  {
    int i;
    float x;
  } u;

  u.x = x;
  u.i = (1<<29) + (u.i >> 1) - (1<<22); 
  return u.x;
} 

/*
brief :
*/
float H3CarmSqrt(float x)
{

	union{
			int intPart;
			float floatPart;
	} convertor;
	union{
			int intPart;
			float floatPart;
	} convertor2;
	convertor.floatPart = x;
	convertor2.floatPart = x;
	convertor.intPart = 0x1FBCF800 + (convertor.intPart >> 1);
	convertor2.intPart = 0x5f3759df - (convertor2.intPart >> 1);
	return 0.5f*(convertor.floatPart + (x * convertor2.floatPart));
	
}

