
/*
+	HpEmitterLogic.h
+	Jelo Wang

+	Hello Particle
+	realtime VFX engine
																				
																												
+	(C) 量子动力实验室 Quantum Dynamics Lab.
+		Community : http://www.qdl.studio
+		Contact Us : jelo.wang@gmail.com

*/ 

#ifndef __HP_GEOMETRY_H__
#define __HP_GEOMETRY_H__

#include "HpAdapter.h"
#include "HpVector.h"
#include <math.h>
#include <float.h> 

//HpVector& HpGeometryTranform2OrthoPlane( float x , float y , float viewWidth , float viewHeight , float left , float right , float bottom , float top )  ;

/*
	brief : RGBA color composed of 4 bytes
*/
typedef struct {

    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

} hpColor4B;

/*
	brief : RGBA color composed of 4 floats
*/
typedef struct {

    float r ;
    float g ;
    float b ;
    float a ;

} hpColor4F ;

/*
	brief : A vertex composed of 2 floats: x, y
*/
typedef struct {

    float x ;
    float y ;
    float z ;

} hpVertex3F ;
        
/*
	brief : A texcoord composed of 2 floats: u, y
*/
typedef struct  {

     float u ;
     float v ;

} hpTex2F ;

/*
	brief : A 3D Quad. 4 * 3 floats
*/
typedef struct {

    hpVertex3F bl ;
    hpVertex3F br ;
    hpVertex3F tl ;
    hpVertex3F tr ;

} hpQuad3 ;

/*
	brief : a Point with a vertex point, a tex coord point and a color 4F
*/
typedef struct {
   
    hpVertex3F vertices ;
    hpColor4F colors ;
    hpTex2F texCoords ;

} hpV3F_C4F_T2F ;

/*
	brief : 4 ccVertex2FTex2FColor4F Quad
*/
typedef struct {

    hpV3F_C4F_T2F bl ;
    hpV3F_C4F_T2F br ;
    hpV3F_C4F_T2F tl ;
    hpV3F_C4F_T2F tr ;

} hpV2F_C4F_T2F_Quad ;

/*
	brief : Blend Function used for textures
*/
typedef struct {

    int src ;
    int dst ;

} hpBlendFunc ;

/*
	brief : texture coordinates for a quad
*/
typedef struct {

    hpTex2F	bl ;
    hpTex2F	br ;
    hpTex2F	tl ;
    hpTex2F	tr ;

} hpT2F_Quad ;

/*
	brief : 
*/
class HpPoint
{

public :
	float x  ;
	float y  ;

	/*
		brief : 
	*/
    HpPoint () {
		this->x = 0  ;
		this->y = 0  ;
	}

	/*
		brief : 
	*/
    HpPoint ( float x , float y ) {
		this->x = x  ;
		this->y = y  ;
	}

	/*
		brief : 
	*/
    HpPoint ( const HpPoint& other ) {
		this->x = other.x  ;
		this->y = other.y  ;
	}

	/*
		brief : 
	*/
	inline bool equals ( const HpPoint& target ) const {
    
		return (fabs(this->x - target.x) < FLT_EPSILON) && (fabs(this->y - target.y) < FLT_EPSILON)  ;

	}

}  ;

/*
	brief : 
*/
class HpSize
{
public:
    float width ;
    float height ;

public:

	/*
		brief : 
	*/
	HpSize() {
		this->width = 0  ;
		this->height = 0  ;
	}

	/*
		brief : 
	*/
	HpSize ( float width , float height ) {
		this->width = width  ;
		this->height = height  ;
	}

	/*
		brief : 
	*/
    HpSize ( const HpSize& other ) {
		this->width = other.width  ;
		this->height = other.height  ;
	}

	/*
		brief : 
	*/
    HpSize ( const HpPoint& point ) {
		this->width = point.x  ;
		this->height = point.y  ;
	}

	/*
		brief : 
	*/
    inline HpSize& operator = ( const HpSize& size ) {
	    setSize ( size.width , size.height )  ;
		return *this ;
	}

	/*
		brief : 
	*/
    inline HpSize& operator =  (const HpPoint& point ) {
	    setSize ( point.x , point.y )  ;
		return *this ;
	}

	/*
		brief : 
	*/
    inline HpSize operator + ( const HpSize& right ) {
		return HpSize ( this->width + right.width , this->height + right.height )  ;
	}

	/*
		brief : 
	*/
    inline HpSize operator - ( const HpSize& right ) {
		return HpSize ( this->width - right.width , this->height - right.height )  ;
	}

	/*
		brief : 
	*/
    inline HpSize operator * ( float a ) {
		return HpSize ( this->width * a , this->height * a )  ;
	}

	/*
		brief : 
	*/
    inline HpSize operator / ( float a ) {
		//assert(a)  ;
		return HpSize ( this->width / a , this->height / a )  ;
	}

	/*
		brief : 
	*/
    inline void setSize ( float width , float height ) {
		this->width = width  ;
		this->height = height  ;
	}

	/*
		brief : 
	*/
    inline bool equals ( const HpSize& target ) const {
	    return ( fabs(this->width  - target.width)  < FLT_EPSILON) && (fabs(this->height - target.height) < FLT_EPSILON )  ;
	}

}  ;

/*
	brief : 
*/
class HpRect
{
public:
    HpPoint origin ;
    HpSize size ;

public:

	/*
		brief : 
	*/
    HpRect () {
		setRect ( 0.0f , 0.0f , 0.0f , 0.0f )  ;
	}
	
	/*
		brief : 
	*/
    HpRect ( float x , float y , float width , float height ) {
		setRect(x, y, width, height) ;
	}

	/*
		brief : 
	*/
    HpRect ( const HpRect& other ) {
		setRect ( other.origin.x , other.origin.y , other.size.width , other.size.height )  ;
	}

	/*
		brief : 
	*/
    inline HpRect& operator = ( const HpRect& other ) {
	    setRect ( other.origin.x , other.origin.y , other.size.width , other.size.height )  ;
		return *this ;
	}

	/*
		brief : 
	*/
    inline void setRect ( float x , float y , float width , float height ) {

		origin.x = x  ;
		origin.y = y  ;

		size.width = width  ;
		size.height = height  ;

	}

	/*
		brief : 
	*/
	inline bool equals ( const HpRect& rect ) const {
		return ( origin.equals(rect.origin) && size.equals(rect.size) )  ;
	}

	/*
		brief : 
	*/
	inline float getMaxX() const {
		return (float)(origin.x + size.width)  ;
	}

	/*
		brief : 
	*/
	inline float getMidX() const {
		return (float)(origin.x + size.width / 2.0)  ;
	}

	/*
		brief : 
	*/
	inline float getMinX() const {
		return origin.x  ;
	}

	/*
		brief : 
	*/
	inline float getMaxY() const {
		return origin.y + size.height  ;
	}

	/*
		brief : 
	*/
	inline float getMidY() const {
		return (float)(origin.y + size.height / 2.0)  ;
	}

	/*
		brief : 
	*/
	inline float getMinY() const {
		return origin.y  ;
	}

	/*
		brief : 
	*/
	inline bool containsPoint ( const HpPoint& point ) const {

		bool bRet = false  ;

		if (point.x >= getMinX() && point.x <= getMaxX() && point.y >= getMinY() && point.y <= getMaxY()) {
			bRet = true  ;
		}

		return bRet  ;
	}

	/*
		brief : 
	*/
	inline bool intersectsRect ( const HpRect& rect ) const
	{
		return !(     getMaxX() < rect.getMinX() ||
				 rect.getMaxX() <      getMinX() ||
					  getMaxY() < rect.getMinY() ||
				 rect.getMaxY() <      getMinY())  ;
	}

} ;

#define HpPointMake(x, y) HpPoint((float)(x), (float)(y))
#define HpSizeMake(width, height) HpSize((float)(width), (float)(height))
#define HpRectMake(x, y, width, height) HpRect((float)(x), (float)(y), (float)(width), (float)(height))

#endif