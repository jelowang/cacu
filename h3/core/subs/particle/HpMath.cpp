
#include "HpMath.h"

static float g_sinTable [360] = {0} ;
static float g_cosTable [360] = {0} ;
static bool g_inited = false ;

static double shift( double x) {
    //always wrap input angle to -PI..PI
    while (x < -3.14159265)
        x += 6.28318531;
    while (x > 3.14159265)
        x -= 6.28318531;
    return x;
}

void HpMatchInit () 
{
	if ( true == g_inited ) {
		return ;
	}

	for ( int looper = 0 ; looper < 360 ; looper ++ ) {
		g_sinTable[looper] = sinf(HP_ADAPTER_HDEGREES_TO_RADIANS(looper)) ;
		g_cosTable[looper] = cosf(HP_ADAPTER_HDEGREES_TO_RADIANS(looper)) ;
	}
	g_inited = true ;
}

#define CC_SWAP(x, y, type)    \
{    type temp = (x);        \
    x = y; y = temp;        \
}
float HpClampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        CC_SWAP(min_inclusive, max_inclusive, float);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

double HpSine(double x) {

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
 
double HpCosine( double x) {

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

float sqrt3(const float x)  
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

float CarmSqrt(float x){

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
