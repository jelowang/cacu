
/*

	+	H3AABB.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	 
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/


#include "H3AABB.h"

/*
 brief :
*/
H3AABB::H3AABB() 
{
	min.x = min.y = min.z = FLT_MAX ;
	max.x = max.y = max.z = -FLT_MAX ;
}

/*
 brief :
*/
H3AABB::~H3AABB()
{
}

/*
 brief :
*/
void H3AABB::setEmpty ()
{
	min.x = min.y = min.z = FLT_MAX ;
	max.x = max.y = max.z = -FLT_MAX ;
}

/*
 brief :
*/

void H3AABB::add ( const H3Vector3f& p )
{

	if ( p.x < min.x ) min.x = p.x ;
	if ( p.y < min.y ) min.y = p.y ;
	if ( p.z < min.z ) min.z = p.z ;

	if ( p.x > max.x ) max.x = p.x ;
	if ( p.y > max.y ) max.y = p.y ;
	if ( p.z > max.z ) max.z = p.z ;

}

/*
 brief :
*/
void H3AABB::set ( const H3AABB* aabb ) 
{
	this->min = aabb->min ;
	this->max = aabb->max ;
}

/*
 brief : return 8 corners of aabb with left-hands orders of clockwise
*/
H3Vector3f* H3AABB::getCorners () 
{
	static H3Vector3f corners[8] ;

	//	near plane
	//	left-bottom
	corners[0].init ( min.x , min.y , min.z ) ;
	//	left-top
	corners[1].init ( min.x , max.y , min.z ) ;
	//	right-top
	corners[2].init ( max.x , max.y , min.z ) ;
	//	right-bottom
	corners[3].init ( max.x , min.y , min.z ) ;

	//	far plane
	//	left-bottom
	corners[4].init ( min.x , min.y , max.z ) ;
	//	left-top
	corners[5].init ( min.x , max.y , max.z ) ;
	//	right-top
	corners[6].init ( max.x , max.y , max.z ) ;
	//	right-bottom
	corners[7].init ( max.x , min.y , max.z ) ;

	return corners ;
}

/*
 brief : Tests whether this bounding box intersects the specified bounding object.
*/
bool H3AABB::intersect ( const H3AABB& aabb ) 
{
    return ((min.x >= aabb.min.x && min.x <= aabb.max.x) || (aabb.min.x >= min.x && aabb.min.x <= max.x)) &&
           ((min.y >= aabb.min.y && min.y <= aabb.max.y) || (aabb.min.y >= min.y && aabb.min.y <= max.y)) &&
           ((min.z >= aabb.min.z && min.z <= aabb.max.z) || (aabb.min.z >= min.z && aabb.min.z <= max.z));

}

/*
 brief : check whether the point is in.
*/
bool H3AABB::containPoint(const H3Vector3f& point ) 
{
	if ( point.x < min.x ) return false ;
	if ( point.y < min.y ) return false ;
	if ( point.z < min.z ) return false ;

	if ( point.x > max.x ) return false ; 
	if ( point.y > max.y ) return false ;
	if ( point.z > max.z ) return false ;

	return true;
}

/*
 brief : Gets the center point of the bounding box.
*/
H3Vector3f H3AABB::getCenter()
{
	H3Vector3f center ;

	center.x = 0.5f * ( min.x+max.x ) ;
	center.y = 0.5f * ( min.y+max.y ) ;
	center.z = 0.5f * ( min.z+max.z ) ;

	return center ;
}

/*
 brief : Transforms the bounding box by the given transformation matrix.
*/
H3AABB H3AABB::transform ( H3Matrix4x4f mat ) 
{

	H3AABB temp ;
	H3Vector3f* corners = 0 ;
	corners = getCorners () ;

	for ( iU32 looper = 0 ; looper < 8 ; looper ++ ) {
		H3Vector3f ret = mat.mul ( corners[looper] ) ;
		corners[looper].init ( ret.x , ret.y , ret.z ) ;
	}

	temp.setEmpty () ;

	for ( iU32 looper = 0 ; looper < 8 ; looper ++ ) {
		temp.add ( corners[looper] ) ;
    }

	return temp ;

}

/*
 brief :
*/
H3AABB H3AABB::operator = (H3AABB aabb) 
{
	this->min = aabb.min ;
	this->max = aabb.max ;
	return *this ;
}
