
/*

	+	H3Components.h
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+	

*/

#include "H3Std.h"
#include "H3Component.h"

/*
 brief :
*/
H3Component::H3Component () 
{
	this->type = H3COMPONENT_TYPE_COMPONENT ;
	this->refCount = 0 ;
}

/*
 brief :
*/
H3Component::~H3Component () {}


/*
 brief :
*/
void H3Component::update ( iF32 deltaT ) {}

/*
 brief :
*/
void H3Component::render ( void* render , H3Component* light , void* shader , H3Component* camera , H3Matrix4x4f* worldMatrix , H3Matrix4x4f* projMatrix , H3Matrix4x4f* lightProjMatrix ) {}

/*
 brief :
*/
H3Vector3f& H3Component::getPosition () 
{
	static H3Vector3f position ;
	position = this->h3Transform.getPosition () ;
	return position ;
}

/*
 brief :
*/
void H3Component::setPosition ( H3Vector3f position ) 
{
	this->h3Transform.setPosition ( position ) ;
}

/*
 brief :
*/
void H3Component::rotate ( H3Vector3f axis , iF32 amout ) 
{
	H3Quaternion Q ( axis , amout ) ;

	this->h3Transform.rotate ( Q ) ;
}

/*
 brief :
*/
void H3Component::rotateAround ( H3Vector3f axis , H3Vector3f point , iF32 amout ) 
{
}

/*
 brief :
*/
H3Transform& H3Component::getTransform () 
{
	return this->h3Transform ;
}

/*
 brief :
*/
iS32 H3Component::getType () 
{
	return this->type ;
}

/* 
 brief : call use inside engine 
*/
void H3Component::addRef () 
{
	this->refCount ++ ;
}

/*
 brief : engine will call this method to release material memorys
*/
void H3Component::release () 
{
	this->refCount -- ;

	if ( 0 == this->refCount ) {
		MMC_DELETE(this) ;
	}
}