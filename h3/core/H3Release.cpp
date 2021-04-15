
/*

	+	H3Release.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Release.h"

H3Release::H3Release () 
{
	this->refCount = 0 ;
	this->enableAutoRelease = false ;
}

H3Release::~H3Release () 
{
}

/*
 brief :
*/
void H3Release::use () 
{
	this->refCount ++ ;
}

/*
 brief :
*/
void H3Release::deUse () 
{
	H3ASSERT( 0 < this->refCount ) ;
	this->refCount -- ;
}

/*
 brief :
*/
bool H3Release::isUse () 
{
	return (this->refCount > 0) ? true : false ;
}
/*
 brief :
*/
bool H3Release::release () 
{
	bool ret = false ;
	deUse () ;
	if ( 0 == this->refCount ) {
		MMC_DELETE(this) ;
		ret = true ;
	}
	return ret ;
}

/*
 brief :
*/
void H3Release::autoRelease () 
{
	this->enableAutoRelease = true ;
}

/*
 brief :
*/
bool H3Release::isAutoRelease () 
{
	return this->enableAutoRelease ;
}