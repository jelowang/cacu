
/*

	+	H3Vector.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine
  

	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Math.h"
#include "H3Vector.h"
#include "H3Memory.h"
#include "H3Log.h"

/*
 brief : 2d
*/
H3Vector2f::H3Vector2f() 
{
	this->x = 0 ;
	this->y = 0 ;
}

/*
 brief :
*/
H3Vector2f::H3Vector2f( iF32 x , iF32 y ) 
{
	this->x = x ;
	this->y = y ;
}

/*
 brief :
*/
H3Vector2f::~H3Vector2f() 
{
	this->x = 0 ;
	this->y = 0 ;
}

/*
 brief :
*/
void H3Vector2f::identify() 
{
	this->x = 0 ;
	this->y = 0 ;
}

/*
 brief :
*/
void H3Vector2f::init( iF32 x , iF32 y ) 
{
	this->x = x ;
	this->y = y ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::add( H3Vector2f v ) 
{
	H3Vector2f r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::sub( H3Vector2f v ) 
{
	H3Vector2f r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::mul( iF32 v ) 
{
	H3Vector2f r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::div( iF32 v ) 
{
	H3ASSERT(v != 0) ;

	H3Vector2f r ; 
	
	iF32 f = 1.0f / v ;

	r.x = this->x * f ;
	r.y = this->y * f ;

	return r ;
}

/*
 brief :
*/
iF32 H3Vector2f::dot( H3Vector2f v ) 
{
	iF32 r = 0.0f ;

	r = this->x*v.x + this->y*v.y ;

	return r ;

}

/*
 brief :
*/
bool H3Vector2f::equal ( H3Vector2f v ) 
{

	iF32 r0 = fabs(this->x - v.x) ;
	iF32 r1 = fabs(this->y - v.y) ;

	if ( H3MathZero(r0) && H3MathZero(r1) )
	{
		return true ;
	}

	return false ;

}

/*
 brief :
*/
iF32 H3Vector2f::mag () 
{
	iF32 r = 0.0f ;

	r = H3MathSqrt( this->x*this->x + this->y*this->y ) ;

	return r ;
}

/*
 brief :
*/
void H3Vector2f::normalize () 
{

	iF32 mag = this->mag() ;

	if ( H3MathZero(mag) ) 
		return ;

	div(mag) ;

}

/*
 brief :
*/
iF32 H3Vector2f::distance ( H3Vector2f v , H3Vector2f v2 ) 
{

	iF32 distance ;

	H3Vector2f r = v ;

	r.sub(v2) ;

	distance = r.mag() ;

	return distance ;

}

/*
 brief : output data 
*/
void H3Vector2f::display () 
{
	iLog ( "|%f,%f|\n" , this->x , this->y ) ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::operator + (H3Vector2f v) 
{
	H3Vector2f r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::operator - (H3Vector2f v) 
{
	H3Vector2f r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::operator * (iF32 v) 
{
	H3Vector2f r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::operator / (iF32 v) 
{
	H3Vector2f r ; 

	iF32 f = 1.0f / v ;

	r.x = this->x * f ;
	r.y = this->y * f ;

	return r ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::operator = (iF32 value) 
{

	this->init ( value , value ) ;

	return *this ;
}

/*
 brief :
*/
H3Vector2f H3Vector2f::operator = (H3Vector2f v) 
{

	this->init ( v.x , v.y ) ;

	return *this ;
}

/*
 brief : 2d
*/
H3Vector2i::H3Vector2i() 
{
	this->x = 0 ;
	this->y = 0 ;
}

/*
 brief :
*/
H3Vector2i::H3Vector2i( iS32 x , iS32 y ) 
{
	this->x = x ;
	this->y = y ;
}

/*
 brief :
*/
H3Vector2i::~H3Vector2i() 
{
	this->x = 0 ;
	this->y = 0 ;
}

/*
 brief :
*/
void H3Vector2i::identify() 
{
	this->x = 0 ;
	this->y = 0 ;
}

/*
 brief :
*/
void H3Vector2i::init( iS32 x , iS32 y ) 
{
	this->x = x ;
	this->y = y ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::add( H3Vector2i v ) 
{
	H3Vector2i r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::sub( H3Vector2i v ) 
{
	H3Vector2i r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::mul( iS32 v ) 
{
	H3Vector2i r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::div( iS32 v ) 
{
	H3ASSERT(v != 0) ;

	H3Vector2i r ; 

	r.x = this->x / v ;
	r.y = this->y / v ;

	return r ;
}

/*
 brief :
*/
iS32 H3Vector2i::dot( H3Vector2i v ) 
{
	iS32 r = 0 ;

	r = this->x*v.x + this->y*v.y ;

	return r ;

}

/*
 brief :
*/
bool H3Vector2i::equal ( H3Vector2i v ) 
{

	iS32 r0 = H3abs(this->x - v.x) ;
	iS32 r1 = H3abs(this->y - v.y) ;

	if ( 0 == r0 && 0 == r1 )
	{
		return true ;
	}

	return false ;

}

/*
 brief :
*/
iS32 H3Vector2i::mag () 
{
	iS32 r = 0 ;

	r = H3MathSqrt( (iD64 )(this->x*this->x + this->y*this->y) ) ;

	return r ;
}

/*
 brief :
*/
void H3Vector2i::normalize () 
{

	iS32 mag = this->mag() ;

	if ( 0 == mag ) 
		return ;

	div(mag) ;

}

/*
 brief :
*/
iS32 H3Vector2i::distance ( H3Vector2i v , H3Vector2i v2 ) 
{

	iS32 distance ;

	H3Vector2i r = v ;

	r.sub(v2) ;

	distance = r.mag() ;

	return distance ;

}

/*
 brief : output data 
*/
void H3Vector2i::display () 
{
	iLog ( "|%d,%d|\n" , this->x , this->y ) ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::operator + (H3Vector2i v) 
{
	H3Vector2i r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::operator - (H3Vector2i v) 
{
	H3Vector2i r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::operator * (iS32 v) 
{
	H3Vector2i r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::operator / (iS32 v) 
{
	H3Vector2i r ; 
	
	r.x = this->x / v ;
	r.y = this->y / v ;

	return r ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::operator = (iS32 value) 
{

	this->init ( value , value ) ;

	return *this ;
}

/*
 brief :
*/
H3Vector2i H3Vector2i::operator = (H3Vector2i v) 
{

	this->init ( v.x , v.y ) ;

	return *this ;
}

/*
 brief : 3d
*/
H3Vector3f::H3Vector3f() 
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
}

/*
 brief :
*/
H3Vector3f::H3Vector3f( iF32 x , iF32 y , iF32 z ) 
{
	this->x = x ;
	this->y = y ;
	this->z = z ;
}

/*
 brief :
*/
H3Vector3f::~H3Vector3f() 
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
}

/*
 brief :
*/
void H3Vector3f::identify() 
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
}

/*
 brief :
*/
void H3Vector3f::init( iF32 x , iF32 y , iF32 z ) 
{

	this->x = x ;
	this->y = y ;
	this->z = z ;

}

/*
 brief :
*/
H3Vector3f H3Vector3f::add( H3Vector3f v ) 
{
	H3Vector3f r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;
	r.z = this->z + v.z ;

	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::sub( H3Vector3f v ) 
{
	H3Vector3f r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;
	r.z = this->z - v.z ;

	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::mul( iF32 v ) 
{

	H3Vector3f r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;
	r.z = this->z * v ;

	return r ;

}

/*
 brief :
*/
H3Vector3f H3Vector3f::div( iF32 v ) 
{
	H3ASSERT(v != 0) ;

	H3Vector3f r ; 

	iF32 f = 1.0f / v ;

	r.x = this->x * f ;
	r.y = this->y * f ;
	r.z = this->z * f ;

	return r ;

}

/*
 brief :
*/
iF32 H3Vector3f::dot( H3Vector3f v ) 
{
	iF32 r = 0.0f ;

	r = this->x*v.x + this->y*v.y + this->z*v.z ;

	return r ;

}

/*
 brief :
*/
H3Vector3f H3Vector3f::cross ( H3Vector3f v ) 
{
	H3Vector3f r ; 

	r.init ( 
		this->y*v.z - this->z*v.y , 
		this->z*v.x - this->x*v.z , 
		this->x*v.y - this->y*v.x  
	) ;

	return r ;
}

/*
 brief :
*/
bool H3Vector3f::equal ( H3Vector3f v ) 
{

	iF32 r0 = fabs(this->x - v.x) ;
	iF32 r1 = fabs(this->y - v.y) ;
	iF32 r2 = fabs(this->z - v.z) ;

	if ( H3MathZero(r0) && H3MathZero(r1) && H3MathZero(r2) )
	{
		return true ;
	}

	return false ;

}

/*
 brief :
*/
iF32 H3Vector3f::mag () 
{
	iF32 r = 0.0f ;

	r = H3MathSqrt( this->x*this->x + this->y*this->y + this->z*this->z ) ;

	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::normalize () 
{

	iF32 mag = this->mag() ;

	if ( H3MathZero(mag) ) 
		return *this ;

	*this = div(mag) ;

	return *this ;

}

/*
 brief :
*/
iF32 H3Vector3f::distance ( H3Vector3f v , H3Vector3f v2 ) 
{

	iF32 distance ;

	H3Vector3f r = v ;

	r.sub(v2) ;

	distance = r.mag() ;

	return distance ;

}

/*
 brief : output data 
*/
void H3Vector3f::display () 
{
	iLog ( "|%f,%f,%f|" , this->x , this->y , this->z );
}

/*
 brief :
*/
H3Vector3f H3Vector3f::operator + (H3Vector3f v) 
{
	H3Vector3f r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;
	r.z = this->z + v.z ;

	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::operator - (H3Vector3f v) 
{
	H3Vector3f r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;
	r.z = this->z - v.z ;

	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::operator * (iF32 v) 
{
	H3Vector3f r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;
	r.z = this->z * v ;
	 
	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::operator / (iF32 v) 
{
	H3Vector3f r ; 

	iF32 f = 1.0f / v ;

	r.x = this->x * f ;
	r.y = this->y * f ;
	r.z = this->z * f ;

	return r ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::operator = (iF32 value) 
{

	this->init ( value , value , value ) ;

	return *this ;
}

/*
 brief :
*/
H3Vector3f H3Vector3f::operator = (H3Vector3f v) 
{

	this->init ( v.x , v.y , v.z ) ;

	return *this ;
}

/*
 brief : 4d
*/
H3Vector4f::H3Vector4f() 
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
	this->w = 0 ;
}

/*
 brief :
*/
H3Vector4f::H3Vector4f( iF32 x , iF32 y , iF32 z , iF32 w ) 
{
	this->x = x ;
	this->y = y ;
	this->z = z ;
	this->w = w ;
}

/*
 brief :
*/
H3Vector4f::~H3Vector4f() 
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
	this->w = 0 ;
}

/*
 brief :
*/
void H3Vector4f::identify() 
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
	this->w = 0 ;
}

/*
 brief :
*/
void H3Vector4f::init( iF32 x , iF32 y , iF32 z , iF32 w ) 
{

	this->x = x ;
	this->y = y ;
	this->z = z ;
	this->w = w ;

}

/*
 brief :
*/
H3Vector4f H3Vector4f::add( H3Vector4f v ) 
{
	H3Vector4f r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;
	r.z = this->z + v.z ;
	r.w = this->w + v.w ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::sub( H3Vector4f v ) 
{
	H3Vector4f r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;
	r.z = this->z - v.z ;
	r.w = this->w - v.w ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::mul( iF32 v ) 
{

	H3Vector4f r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;
	r.z = this->z * v ;
	r.w = this->w * v ;

	return r ;

}

/*
 brief :
*/
H3Vector4f H3Vector4f::div( iF32 v ) 
{
	H3ASSERT(v != 0) ;

	H3Vector4f r ; 

	iF32 f = 1.0f / v ;

	r.x = this->x * f ;
	r.y = this->y * f ;
	r.z = this->z * f ;
	r.w = this->w * f ;

	return r ;

}

/*
 brief :
*/
iF32 H3Vector4f::dot( H3Vector4f v ) 
{
	iF32 r = 0.0f ;

	r = this->x*v.x + this->y*v.y + this->z*v.z + this->w*this->w ;

	return r ;

}

/*
 brief :
*/
bool H3Vector4f::equal ( H3Vector4f v ) 
{

	iF32 r0 = fabs(this->x - v.x) ;
	iF32 r1 = fabs(this->y - v.y) ;
	iF32 r2 = fabs(this->z - v.z) ;
	iF32 r3 = fabs(this->w - v.w) ;

	if ( H3MathZero(r0) && H3MathZero(r1) && H3MathZero(r2) && H3MathZero(r3) )
	{
		return true ;
	}

	return false ;

}

/*
 brief :
*/
iF32 H3Vector4f::mag () 
{
	iF32 r = 0.0f ;

	r = H3MathSqrt( this->x*this->x + this->y*this->y + this->z*this->z + this->w*this->w ) ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::normalize () 
{

	iF32 mag = this->mag() ;

	if ( H3MathZero(mag) ) 
		return *this ;

	*this = div(mag) ;

	return *this ;

}

/*
 brief :
*/
iF32 H3Vector4f::distance ( H3Vector4f v , H3Vector4f v2 ) 
{

	iF32 distance ;

	H3Vector4f r = v ;

	r.sub(v2) ;

	distance = r.mag() ;

	return distance ;

}

/*
 brief : output data 
*/
void H3Vector4f::display () 
{
	iLog ( "|%f,%f,%f,%f|" , this->x , this->y , this->z , this->w );
}

/*
 brief :
*/
H3Vector4f H3Vector4f::operator + (H3Vector4f v) 
{
	H3Vector4f r ; 

	r.x = this->x + v.x ;
	r.y = this->y + v.y ;
	r.z = this->z + v.z ;
	r.w = this->w + v.w ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::operator - (H3Vector4f v) 
{
	H3Vector4f r ; 

	r.x = this->x - v.x ;
	r.y = this->y - v.y ;
	r.z = this->z - v.z ;
	r.w = this->w - v.w ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::operator * (iF32 v) 
{
	H3Vector4f r ; 

	r.x = this->x * v ;
	r.y = this->y * v ;
	r.z = this->z * v ;
	r.w = this->w * v ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::operator / (iF32 v) 
{
	H3Vector4f r ; 

	iF32 f = 1.0f / v ;

	r.x = this->x * f ;
	r.y = this->y * f ;
	r.z = this->z * f ;
	r.w = this->w * f ;

	return r ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::operator = (iF32 value) 
{

	this->init ( value , value , value , value ) ;

	return *this ;
}

/*
 brief :
*/
H3Vector4f H3Vector4f::operator = (H3Vector4f v) 
{

	this->init ( v.x , v.y , v.z , v.w ) ;

	return *this ;
}


#if 0
int main(int argc, char** argv[])
{

	H3Vector3f v1 ;
	H3Vector3f v2 ;

	v1.init( 2.0f,10.0f ,0.0f ) ;
	v1.normalize () ;

	v2.init( 15.0f,5.0f ,0.0f ) ;
	v2.normalize() ;

	iLog("%1.3f,%1.3f,%1.3f\n",v1.x,v1.y,v1.z);
	iLog("%1.3f,%1.3f,%1.3f\n",v2.x,v2.y,v2.z);

	iLog("%1.50f\n",v1.dot(v2));

	H3Vector3f v3 = v1.cross(v2) ;
	iLog("%1.3f,%1.3f,%1.3f\n",v3.x,v3.y,v3.z);
	v3.normalize();
	iLog("%1.3f,%1.3f,%1.3f\n",v3.x,v3.y,v3.z);

	for ( int i = 0;1; i ++ ) {
		v2.init( -100,-100 ,0.0f ) ;
		v2.normalize() ;
		iLog("%1.50f\n",v1.dot(v2));
		//Sleep(10) ;
	}

	return 0;
}

#endif