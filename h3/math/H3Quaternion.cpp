
/*

	 +	H3Quaternion.h
	 +	QDL
 
	 +	hello3d
	 +	realtime 3d engine
 

	 +	(C) Quantum Dynamics Lab.
			FACEGOOD.
	 +
 
 */

#include "H3Math.h"
#include "H3Vector.h"  
#include "H3Matrix.h"
#include "H3Quaternion.h"
#include "H3Eular.h"
#include "H3Memory.h"
#include "H3Log.h"

/*
 brief :
*/
H3Quaternion::H3Quaternion() 
{
	identify () ;
}
/*
 brief :
*/
H3Quaternion::H3Quaternion( iF32 w , iF32 x , iF32 y , iF32 z ) 
{
	this->w = w ;
	this->x = x ;
	this->y = y ;
	this->z = z ;
}

/*
 brief : rotate about ambitary axis
*/
H3Quaternion::H3Quaternion( const H3Vector3f aixs , iF32 theta ) 
{

	theta = H3ToRadian ( theta ) ;
    theta = theta * 0.5f ;
    iF32 sinHalfAngle = H3sinf(theta) ;
    
    this->w = H3cosf(theta) ;
    this->x = aixs.x * sinHalfAngle ;
    this->y = aixs.y * sinHalfAngle ;
    this->z = aixs.z * sinHalfAngle ;

}

/*
 brief :
*/
H3Quaternion::~H3Quaternion() {}

/*
 brief :
*/
void H3Quaternion::identify () 
{
	this->w = 1.0f ;
	this->x = 0.0f ;
	this->y = 0.0f ;
	this->z = 0.0f ;
}

/*
 brief :
*/
H3Quaternion H3Quaternion::mul ( H3Quaternion a ) 
{
	H3Quaternion results ;
	
	//	standard forms
	//results.w = this->w*Q.w - this->x*Q.x - this->y*Q.y - this->z*Q.z ;
	//results.x = this->x*Q.w + this->w*Q.x + this->z*Q.y - this->y*Q.z ;
	//results.y = this->y*Q.w + this->w*Q.y + this->x*Q.z - this->z*Q.x ;
	//results.z = this->z*Q.w + this->w*Q.z + this->y*Q.x - this->x*Q.y ;

	//	alternetive
	//results.w = this->w*a.w - this->x*a.x - this->y*a.y - this->z*a.z ;
	//results.x = this->w*a.x + this->x*a.w + this->z*a.y - this->y*a.z ;
	//results.y = this->w*a.y + this->y*a.w + this->x*a.z - this->z*a.x ;
	//results.z = this->w*a.z + this->z*a.w + this->y*a.x - this->x*a.y ;

	results.w = this->w * a.w - this->x * a.x - this->y * a.y - this->z * a.z ;
	results.x = this->x * a.w + this->w * a.x + this->y * a.z - this->z * a.y ;
	results.y = this->y * a.w + this->w * a.y + this->z * a.x - this->x * a.z ;
	results.z = this->z * a.w + this->w * a.z + this->x * a.y - this->y * a.x ;
	 
	return results ;
}

/*
 brief :
*/
H3Quaternion H3Quaternion::mul ( H3Vector3f v ) 
{
	H3Quaternion results ;

	results.w = -this->x*v.x - this->y*v.y - this->z*v.z ;
	results.x = this->w*v.x + this->y*v.z - this->z*v.y ;
	results.y = this->w*v.y + this->z*v.x - this->x*v.z ;
	results.z = this->w*v.z + this->x*v.y - this->y*v.x ;

	return results ;
}

/*
 brief :
 */
H3Quaternion H3Quaternion::conjugate()
{
    H3Quaternion results ;
    
    results.w = this->w ;
    results.x = -this->x ;
    results.y = -this->y ;
    results.z = -this->z ;
    
    return results ;
}

/*
 brief : magnitude 
*/
iF32 H3Quaternion::mag ()
{
	iF32 results = (iF32 ) H3MathSqrt ( this->w*this->w + this->x*this->x + this->y*this->y + this->z*this->z ) ;
	return results ;
}

/*
 brief :
*/
void H3Quaternion::normalize ()
{

	iF32 mag = this->mag () ;

	if ( mag > 0.0f )
	{
		iF32 oneOverMag = 1.0f / mag ;
		this->w = this->w * oneOverMag ;
		this->x = this->x * oneOverMag ;
		this->y = this->y * oneOverMag ;
		this->z = this->z * oneOverMag ;
	} 
	else
	{
		assert(0) ;
	}
	
	return ;
	

}

 /*
  brief : 
 */
 H3Vector3f H3Quaternion::getRotateAxis() 
 {
	 H3Vector3f r ;

	 iF32 sinThetaOver2Sq = 1.0f - w*w ;

	 if ( sinThetaOver2Sq <= 0.0f ) 
	 {
		 r.init ( 1.0f , 0.0f , 0.0f ) ;
	 }

	 iF32 oneOverSinThetaOver2 = 1.0f / H3Sqrt3(sinThetaOver2Sq) ;

	 r.init ( this->x*oneOverSinThetaOver2 , this->y*oneOverSinThetaOver2 , this->z*oneOverSinThetaOver2 ) ;

	 return r ;

 }

/*
 brief : get matrix forms
		
		|1-2y^2+2z^2	2xy-2wz			2xz+2wy		|
		|2xy+2wz		1-2x^2+2z^2		2yz-2wz		|
		|2xz-2wy		2yz+2wx			1-2x^2+2y^2	|	

*/
H3Matrix4x4f H3Quaternion::getMatrixForm () 
{
	H3Matrix4x4f r ;

	iF32 xsqr = this->x*this->x ;
	iF32 ysqr = this->y*this->y ;
	iF32 zsqr = this->z*this->z ; 
	iF32 xmuly = this->x*this->y ;
	iF32 xmulz = this->x*this->z ;
	iF32 wmulx = this->w*this->x ;
	iF32 wmuly = this->w*this->y ;
	iF32 wmulz = this->w*this->z ;
	iF32 ymulz = this->y*this->z ;

#if 1 
	//	up-right to object space
	r.m[0][0] = 1.0f-2.0f*(ysqr+zsqr) ;		r.m[0][1] = 2.0f*(xmuly-wmulz) ;		r.m[0][2] = 2.0f*(xmulz+wmuly) ;		r.m[0][3] = 0 ;
	r.m[1][0] = 2.0f*(xmuly+wmulz) ;		r.m[1][1] = 1.0f-2.0f*(xsqr+zsqr) ;		r.m[1][2] = 2.0f*(ymulz-wmulx) ;		r.m[1][3] = 0 ;
	r.m[2][0] = 2.0f*(xmulz-wmuly) ;		r.m[2][1] = 2.0f*(ymulz+wmulx) ;		r.m[2][2] = 1.0f-2.0f*(xsqr+ysqr) ;		r.m[2][3] = 0 ;
	r.m[3][0] = 0 ;							r.m[3][1] = 0 ;							r.m[3][2] = 0 ;							r.m[3][3] = 1.0f ;
	return r ;
#endif

	r.m[0][0] = 1.0f-2.0f*(ysqr-zsqr) ;		r.m[0][1] = 2.0f*(xmuly+wmulz) ;		r.m[0][2] = 2.0f*(xmulz-wmuly) ;		r.m[0][3] = 0 ;
	r.m[1][0] = 2.0f*(xmuly-wmulz) ;		r.m[1][1] = 1.0f-2.0f*(xsqr-zsqr) ;		r.m[1][2] = 2.0f*(ymulz+wmulx) ;		r.m[1][3] = 0 ;
	r.m[2][0] = 2.0f*(xmulz+wmuly) ;		r.m[2][1] = 2.0f*(ymulz-wmulx) ;		r.m[2][2] = 1.0f-2.0f*(xsqr-ysqr) ;		r.m[2][3] = 0 ;
	r.m[3][0] = 0 ;							r.m[3][1] = 0 ;							r.m[3][2] = 0 ;							r.m[3][3] = 1.0f ;

	return r ;

}

/*
 brief : output data 
*/
void H3Quaternion::display () 
{
	iLog ( "|%f|\n" , this->w ) ;
	iLog ( "|%f|\n" , this->x ) ;
	iLog ( "|%f|\n" , this->y ) ;
	iLog ( "|%f|\n" , this->z ) ;
}

/*
 brief :
*/
H3Quaternion H3Quaternion::operator * (H3Quaternion Q) 
{

	H3Quaternion results ;

	results = this->mul ( Q ) ; 

	return results ;

}

/*
 brief :
*/
H3Quaternion H3Quaternion::operator = (H3Quaternion Q) 
{
	
	this->w = Q.w ;
	this->x = Q.x ;
	this->y = Q.y ;
	this->z = Q.z ;

	return *this ;

}
