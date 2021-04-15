
/*

	+	H3Ray.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
		   FACEGOOD.
	+
 
*/

#include "H3Ray.h"

/*
 brief :
*/
H3Ray::H3Ray () 
{
	this->o = 0 ;
	this->d = 0 ;
}

/*
 brief : o = orign , d direction 
*/
H3Ray::H3Ray ( H3Vector3f o , H3Vector3f d ) 
{
	this->o = o ;
	this->d = d ;
}

/*
 brief : rotate direction 
*/
H3Ray::~H3Ray () {}

/*
 brief :
*/
H3Vector3f H3Ray::castRay ( H3Camera* camera , H3Vector2f point , H3Vector2f viewPort ) 
{

	H3ASSERT ( NULL != camera ) ;

	H3Matrix4x4f viewMat = camera->getView().inverse() ;
	H3Matrix4x4f projMat = camera->getProjection().inverse() ;

	H3Vector2f NDC ;
	NDC.x = ( 2.0f * point.x ) / viewPort.x - 1.0f ;
	NDC.y = -1.0f * (( 2.0f * point.y ) / viewPort.y - 1.0f) ; 

	H3Vector4f clipCoords ( NDC.x , NDC.y , -1.0f , 1.0f ) ;

	//	get eye coords
	H3Vector4f eyeCoords = projMat.mul ( clipCoords ) ;
	eyeCoords.z = 1.0f ;
	eyeCoords.w = 0.0f ;

	//	get word coords
	H3Vector4f worldCoords = viewMat.mul ( eyeCoords ) ;
	H3Vector3f ray ( worldCoords.x , worldCoords.y , worldCoords.z ) ;

	if ( true == camera->isPerspectiveProjection () )
		ray.normalize () ;

	return ray ;

}