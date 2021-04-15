#ifndef HP_MATH_H
#define HP_MATH_H

#include <stdlib.h>
#include <float.h>
#include <math.h>

#ifndef HpMIN
#define HpMIN(x,y) (((x) > (y)) ? (y) : (x))
#endif 

#ifndef HpMAX
#define HpMAX(x,y) (((x) < (y)) ? (y) : (x))
#endif  

#define HP_IS_ZERO(x) (-0.000001 <= x && x <= 0.000001)

/** @def MPRANDOM_MINUS1_1
 returns a random float between -1 and 1
 */
#define HPRANDOM_MINUS1_1() ((2.0f*((float)rand()/RAND_MAX))-1.0f)

/** @def MPRANDOM_0_1
 returns a random float between 0 and 1
 */
#define HPRANDOM_0_1() ((float)rand()/RAND_MAX)

/** @def HP_ADAPTER_HDEGREES_TO_RADIANS
 converts degrees to radians
 */
#define HP_ADAPTER_HDEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180

/** @def HP_ADAPTER_HRADIANS_TO_DEGREES
 converts radians to degrees
 */
#define HP_ADAPTER_HRADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

float HpClampf(float value, float min_inclusive, float max_inclusive) ;
double HpSine(double x) ;
double HpCosine( double x) ;
float CarmSqrt(float x) ;

#endif