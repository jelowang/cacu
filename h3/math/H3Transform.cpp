
/*

	+	H3Transform.cpp
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Transform.h"

/*
 brief :
*/
H3Transform::H3Transform() 
{
	this->basics.init ( -0.0f , 0.0f , 0.0f ) ;
	this->scale.init ( 1.0f , 1.0f , 1.0f ) ;
	this->position.init ( 0.0f , 0.0f , 0.0f ) ;
	this->quaternion.identify () ;
}

/*
 brief :
*/
H3Transform::~H3Transform() {
};

/*
 brief :
*/
void H3Transform::reset () 
{
	this->basics.init ( 0.0f , 0.0f , 0.0f ) ;
	this->scale.init ( 1.0f , 1.0f , 1.0f ) ;
	this->position.init ( 0.0f , 0.0f , 0.0f ) ;
	this->quaternion.identify () ;
}

/*
 brief :
*/
H3Vector3f& H3Transform::getPosition () 
{
	return this->position ;
}

/*
 brief : set basics
*/
void H3Transform::setBasics ( H3Vector3f basics ) 
{
	this->basics = basics ;
}

/*
 brief : get basics
*/
H3Vector3f& H3Transform::getBasics () 
{
	return this->basics ; 
}

/*
 brief :
*/
void H3Transform::setPosition ( H3Vector3f position ) 
{
	this->position = position ;
}

/*
 brief :
*/
void H3Transform::setScale ( H3Vector3f scale ) 
{
	this->scale = scale ;
}

/*
 brief :
*/
void H3Transform::rotate ( H3Quaternion Q ) 
{
	//	we do conjugate at getViewProject since getRotate called
	this->quaternion = Q * this->quaternion ; 
	this->quaternion.normalize () ;
}  
  
/*
 brief :
*/
H3Quaternion H3Transform::getRotate () 
{
	return this->quaternion ;
}

/*
 brief : rotate , scale , transform
*/
H3Matrix4x4f H3Transform::finishTransformation ( bool skipRotate ) 
{

	H3Matrix4x4f baiscsTranslateMat4 ;
	H3Matrix4x4f translateMat4 ;
	H3Matrix4x4f rotateMat4 ;
	H3Matrix4x4f scaleMat4 ;

	baiscsTranslateMat4.initTranslate ( this->basics ) ;
	translateMat4.initTranslate ( getPosition() ) ;
	scaleMat4.initScale ( this->scale ) ;

	if ( false == skipRotate ) 
		rotateMat4 = this->quaternion.getMatrixForm () ;
	else {
		rotateMat4.identify () ;
	}
	
	translateMat4 = translateMat4 * scaleMat4 * rotateMat4 ; 
	translateMat4 = translateMat4 * baiscsTranslateMat4 ;

	return translateMat4 ;

}
