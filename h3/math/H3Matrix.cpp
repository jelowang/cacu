
/*

	+	H3Matrix.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+

*/

#include "H3Std.h"
#include "H3Math.h"
#include "H3Vector.h"
#include "H3Matrix.h"
#include "H3Memory.h"

/*
 brief :
*/
H3Matrix4x4f::H3Matrix4x4f() 
{
	memset(this->m , 0 , sizeof(this->m)) ;
	identify () ;
}

/*
 brief :
*/	
H3Matrix4x4f::~H3Matrix4x4f() 
{
}

/*
 brief :
*/	
void H3Matrix4x4f::identify() 
{

	m[0][0] = 1.0f ;	m[0][1] = 0 ;		m[0][2] = 0 ;		m[0][3] = 0 ;
	m[1][0] = 0 ;		m[1][1] = 1.0f ;	m[1][2] = 0 ;		m[1][3] = 0 ;
	m[2][0] = 0 ;		m[2][1] = 0 ;		m[2][2] = 1.0f ;	m[2][3] = 0 ;
	m[3][0] = 0 ;		m[3][1] = 0 ;		m[3][2] = 0 ;		m[3][3] = 1.0f ;

}

/*
 brief : mulfity a scalar
*/
H3Matrix4x4f H3Matrix4x4f::mul( iF32 scalar ) 
{
	H3Matrix4x4f mat ;

	mat.m[0][0] = m[0][0] * scalar ;				
	mat.m[1][0] = m[1][0] * scalar ;				
	mat.m[2][0] = m[2][0] * scalar ;				
	mat.m[3][0] = m[3][0] * scalar ;				
				  		   
	mat.m[0][1] = m[0][1] * scalar ;	
	mat.m[1][1] = m[1][1] * scalar ;	
	mat.m[2][1] = m[2][1] * scalar ;	
	mat.m[3][1] = m[3][1] * scalar ;	
				  		   
	mat.m[0][2] = m[0][2] * scalar ;	
	mat.m[1][2] = m[1][2] * scalar ;	
	mat.m[2][2] = m[2][2] * scalar ;	
	mat.m[3][2] = m[3][2] * scalar ;	
				  		   
	mat.m[0][3] = m[0][3] * scalar ;
	mat.m[1][3] = m[1][3] * scalar ;
	mat.m[2][3] = m[2][3] * scalar ;
	mat.m[3][3] = m[3][3] * scalar ;

	return mat ;
						  
}

/*
 brief :
*/	
H3Matrix4x4f H3Matrix4x4f::mul( const H3Matrix4x4f a ) 
{

	H3Matrix4x4f r ;

	r.m[0][0] = a.m[0][0]*m[0][0] + a.m[1][0]*m[0][1] + a.m[2][0]*m[0][2] + a.m[3][0]*m[0][3] ; r.m[0][1] = a.m[0][1]*m[0][0] + a.m[1][1]*m[0][1] + a.m[2][1]*m[0][2] + a.m[3][1]*m[0][3] ; r.m[0][2] = a.m[0][2]*m[0][0] + a.m[1][2]*m[0][1] + a.m[2][2]*m[0][2] + a.m[3][2]*m[0][3] ; r.m[0][3] = a.m[0][3]*m[0][0] + a.m[1][3]*m[0][1] + a.m[2][3]*m[0][2] + a.m[3][3]*m[0][3] ;
	r.m[1][0] = a.m[0][0]*m[1][0] + a.m[1][0]*m[1][1] + a.m[2][0]*m[1][2] + a.m[3][0]*m[1][3] ; r.m[1][1] = a.m[0][1]*m[1][0] + a.m[1][1]*m[1][1] + a.m[2][1]*m[1][2] + a.m[3][1]*m[1][3] ; r.m[1][2] = a.m[0][2]*m[1][0] + a.m[1][2]*m[1][1] + a.m[2][2]*m[1][2] + a.m[3][2]*m[1][3] ; r.m[1][3] = a.m[0][3]*m[1][0] + a.m[1][3]*m[1][1] + a.m[2][3]*m[1][2] + a.m[3][3]*m[1][3] ;
	r.m[2][0] = a.m[0][0]*m[2][0] + a.m[1][0]*m[2][1] + a.m[2][0]*m[2][2] + a.m[3][0]*m[2][3] ; r.m[2][1] = a.m[0][1]*m[2][0] + a.m[1][1]*m[2][1] + a.m[2][1]*m[2][2] + a.m[3][1]*m[2][3] ; r.m[2][2] = a.m[0][2]*m[2][0] + a.m[1][2]*m[2][1] + a.m[2][2]*m[2][2] + a.m[3][2]*m[2][3] ; r.m[2][3] = a.m[0][3]*m[2][0] + a.m[1][3]*m[2][1] + a.m[2][3]*m[2][2] + a.m[3][3]*m[2][3] ;
	r.m[3][0] = a.m[0][0]*m[3][0] + a.m[1][0]*m[3][1] + a.m[2][0]*m[3][2] + a.m[3][0]*m[3][3] ; r.m[3][1] = a.m[0][1]*m[3][0] + a.m[1][1]*m[3][1] + a.m[2][1]*m[3][2] + a.m[3][1]*m[3][3] ; r.m[3][2] = a.m[0][2]*m[3][0] + a.m[1][2]*m[3][1] + a.m[2][2]*m[3][2] + a.m[3][2]*m[3][3] ; r.m[3][3] = a.m[0][3]*m[3][0] + a.m[1][3]*m[3][1] + a.m[2][3]*m[3][2] + a.m[3][3]*m[3][3] ;

	//printf("%1.3f|%1.3f|%1.3f|%1.3f|\n" , r.m[0][0] , r.m[0][1] , r.m[0][2] , r.m[0][3] ) ;
	//printf("%1.3f|%1.3f|%1.3f|%1.3f|\n" , r.m[1][0] , r.m[1][1] , r.m[1][2] , r.m[1][3] ) ;
	//printf("%1.3f|%1.3f|%1.3f|%1.3f|\n" , r.m[2][0] , r.m[2][1] , r.m[2][2] , r.m[2][3] ) ;
	//printf("%1.3f|%1.3f|%1.3f|%1.3f|\n" , r.m[3][0] , r.m[3][1] , r.m[3][2] , r.m[3][3] ) ;
	//printf("\n") ;

	return r ;

}

/*
 brief :
*/
H3Vector3f H3Matrix4x4f::mul( const H3Vector3f v ) 
{

	H3Vector3f r ;

	r.x = v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2] + m[0][3] ;	
	r.y = v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2] + m[1][3] ;			
	r.z = v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2] + m[2][3] ;	

	return r ;

}

/*
 brief : mulfity a verctor
*/
H3Vector4f H3Matrix4x4f::mul( const H3Vector4f v ) 
{

	H3Vector4f r ;

	r.x = v.x*m[0][0] + v.y*m[0][1] + v.z*m[0][2] + v.w*m[0][3] ;	
	r.y = v.x*m[1][0] + v.y*m[1][1] + v.z*m[1][2] + v.w*m[1][3] ;			
	r.z = v.x*m[2][0] + v.y*m[2][1] + v.z*m[2][2] + v.w*m[2][3] ;	
	r.w = v.x*m[3][0] + v.y*m[3][1] + v.z*m[3][2] + v.w*m[3][3] ;

	return r ;

}

/*
 brief : 
*/
void H3Matrix4x4f::initScale ( const H3Vector3f v ) 
{

	m[0][0] = v.x ;		m[0][1] = 0 ;		m[0][2] = 0 ;		m[0][3] = 0 ;
	m[1][0] = 0 ;		m[1][1] = v.y ;		m[1][2] = 0 ;		m[1][3] = 0 ;
	m[2][0] = 0 ;		m[2][1] = 0 ;		m[2][2] = v.z ;		m[2][3] = 0 ;
	m[3][0] = 0 ;		m[3][1] = 0 ;		m[3][2] = 0 ;		m[3][3] = 1.0f ;

}

/*
 brief : 
*/
void H3Matrix4x4f::initTranslate ( const H3Vector3f v ) 
{

	m[0][0] = 1.0f ;	m[0][1] = 0 ;		m[0][2] = 0 ;		m[0][3] = v.x ;
	m[1][0] = 0 ;		m[1][1] = 1.0f ;	m[1][2] = 0 ;		m[1][3] = v.y ;
	m[2][0] = 0 ;		m[2][1] = 0 ;		m[2][2] = 1.0f ;	m[2][3] = v.z ;
	m[3][0] = 0 ;		m[3][1] = 0 ;		m[3][2] = 0 ;		m[3][3] = 1.0f ;

}

/*
 brief :
*/
iF32 H3Matrix4x4f::determinant() 
{

	iF32 value = 0.0f ;

    iF32 a0 = m[0][0] * m[1][1] - m[1][0] * m[0][1] ;
    iF32 a1 = m[0][0] * m[2][1] - m[2][0] * m[0][1] ;
    iF32 a2 = m[0][0] * m[3][1] - m[3][0] * m[0][1] ;
    iF32 a3 = m[1][0] * m[2][1] - m[2][0] * m[1][1] ;
    iF32 a4 = m[1][0] * m[3][1] - m[3][0] * m[1][1] ;
    iF32 a5 = m[2][0] * m[3][1] - m[3][0] * m[2][1] ;
    iF32 b0 = m[0][2] * m[1][3] - m[1][2] * m[0][3] ;
    iF32 b1 = m[0][2] * m[2][3] - m[2][2] * m[0][3] ;
    iF32 b2 = m[0][2] * m[3][3] - m[3][2] * m[0][3] ;
    iF32 b3 = m[1][2] * m[2][3] - m[2][2] * m[1][3] ;
    iF32 b4 = m[1][2] * m[3][3] - m[3][2] * m[1][3] ;
    iF32 b5 = m[2][2] * m[3][3] - m[3][2] * m[2][3] ;

    value = (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0) ; 

	return value ;

}

/*
 brief :
*/
H3Matrix4x4f H3Matrix4x4f::inverse () 
{
	H3Matrix4x4f mat(*this) ;
	
	iF32 det = mat.determinant () ;

    iF32 a0 = m[0][0] * m[1][1] - m[1][0] * m[0][1] ;
    iF32 a1 = m[0][0] * m[2][1] - m[2][0] * m[0][1] ;
    iF32 a2 = m[0][0] * m[3][1] - m[3][0] * m[0][1] ;
    iF32 a3 = m[1][0] * m[2][1] - m[2][0] * m[1][1] ;
    iF32 a4 = m[1][0] * m[3][1] - m[3][0] * m[1][1] ;
    iF32 a5 = m[2][0] * m[3][1] - m[3][0] * m[2][1] ;
    iF32 b0 = m[0][2] * m[1][3] - m[1][2] * m[0][3] ;
    iF32 b1 = m[0][2] * m[2][3] - m[2][2] * m[0][3] ;
    iF32 b2 = m[0][2] * m[3][3] - m[3][2] * m[0][3] ;
    iF32 b3 = m[1][2] * m[2][3] - m[2][2] * m[1][3] ;
    iF32 b4 = m[1][2] * m[3][3] - m[3][2] * m[1][3] ;
    iF32 b5 = m[2][2] * m[3][3] - m[3][2] * m[2][3] ;

    det = (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0) ; 

	if ( fabs ( det ) < H3Tolerance ) {
		return mat ;
	}

    mat.m[0][0]  = m[1][1] * b5 - m[2][1] * b4 + m[3][1] * b3 ;
    mat.m[1][0]  = -m[1][0] * b5 + m[2][0] * b4 - m[3][0] * b3 ;
    mat.m[2][0]  = m[1][3] * a5 - m[2][3] * a4 + m[3][3] * a3 ;
    mat.m[3][0]  = -m[1][2] * a5 + m[2][2] * a4 - m[3][2] * a3 ;

    mat.m[0][1]  = -m[0][1] * b5 + m[2][1] * b2 - m[3][1] * b1 ;
    mat.m[1][1]  = m[0][0] * b5 - m[2][0] * b2 + m[3][0] * b1 ;
    mat.m[2][1]  = -m[0][3] * a5 + m[2][3] * a2 - m[3][3] * a1 ;
    mat.m[3][1]  = m[0][2] * a5 - m[2][2] * a2 + m[3][2] * a1 ;

    mat.m[0][2]  = m[0][1] * b4 - m[1][1] * b2 + m[3][1] * b0 ;
    mat.m[1][2]  = -m[0][0] * b4 + m[1][0] * b2 - m[3][0] * b0 ;
    mat.m[2][2] = m[0][3] * a4 - m[1][3] * a2 + m[3][3] * a0 ;
    mat.m[3][2] = -m[0][2] * a4 + m[1][2] * a2 - m[3][2] * a0 ;
	
    mat.m[0][3] = -m[0][1] * b3 + m[1][1] * b1 - m[2][1] * b0 ;
    mat.m[1][3] = m[0][0] * b3 - m[1][0] * b1 + m[2][0] * b0 ;
    mat.m[2][3] = -m[0][3] * a3 + m[1][3] * a1 - m[2][3] * a0 ;
    mat.m[3][3] = m[0][2] * a3 - m[1][2] * a1 + m[2][2] * a0 ;

	mat = mat.mul ( 1.0f / det ) ;

	return mat ;
}

/*
 brief : 
*/
void H3Matrix4x4f::transpose() 
{

	for ( iS32 row = 0 ; row < 4 ; row ++ )
	{
		for ( iS32 col = row+1 ; col < 4 ; col ++ )
		{
			iF32 value = this->m[row][col] ;
			this->m[row][col] = this->m[col][row] ;
			this->m[col][row] = value ;
		}

	}

}

/*
 brief :
*/
void H3Matrix4x4f::set( int row , int col , iF32 value ) 
{

	H3ASSERT( row >= 0 && row <= 3 ) ;
	H3ASSERT( col >= 0 && col <= 3 ) ;

	this->m[row][col] = value ;

}

/*
 brief :
*/
void H3Matrix4x4f::set( iF32 matrix[4][4] ) 
{
	memcpy ( this->m , matrix , 4*4*sizeof(iF32) ) ;
}

/*
 brief : output data on console
*/
void H3Matrix4x4f::display () 
{

	iLog("%1.3f|%1.3f|%1.3f|%1.3f|" , m[0][0] , m[0][1] , m[0][2] , m[0][3] ) ;
	iLog ( "%1.3f|%1.3f|%1.3f|%1.3f|", m[1][0], m[1][1], m[1][2], m[1][3] );
	iLog ( "%1.3f|%1.3f|%1.3f|%1.3f|", m[2][0], m[2][1], m[2][2], m[2][3] );
	iLog ( "%1.3f|%1.3f|%1.3f|%1.3f|", m[3][0], m[3][1], m[3][2], m[3][3] );

}

/*
 brief :
*/
H3Matrix4x4f& H3Matrix4x4f::operator * (H3Matrix4x4f M) 
{
	static H3Matrix4x4f results ;
	results = *this ;
	results = results.mul ( M ) ;
	return results ;
}

/*
 brief :
*/
H3Matrix4x4f& H3Matrix4x4f::operator = (H3Matrix4x4f M) 
{
	memcpy ( this->m , M.m , sizeof(this->m) ) ;
	return *this ;
}

#if 0

int main(int argc, char** argv[])
{

	H3Matrix4x4f a ;
	H3Matrix4x4f b ;

	a.set(0,0,1) ;
	a.set(1,0,20/2) ;
	a.set(2,0,30/2) ;
	a.set(3,0,40/2) ;

	a.set(0,1,1/2) ;
	a.set(1,1,1/2) ;
	a.set(2,1,50/2) ;
	a.set(3,1,1/2) ;

	a.set(0,2,1/2) ;
	a.set(1,2,95/2) ;
	a.set(2,2,54/2) ;
	a.set(3,2,32/2) ;

	a.set(0,3,1/2) ;
	a.set(1,3,84/2) ;
	a.set(2,3,2/2) ;
	a.set(3,3,101/2) ;

	//

	b.set(0,0,1) ;
	b.set(1,0,20) ;
	b.set(2,0,30) ;
	b.set(3,0,40) ;

	b.set(0,1,1) ;
	b.set(1,1,1) ;
	b.set(2,1,50) ;
	b.set(3,1,1) ;

	b.set(0,2,1) ;
	b.set(1,2,95) ;
	b.set(2,2,54) ;
	b.set(3,2,32) ;

	b.set(0,3,1) ;
	b.set(1,3,84) ;
	b.set(2,3,2) ;
	b.set(3,3,101) ;

	a.mul(b) ;

	return 0;
}

#endif