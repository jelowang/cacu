
/*

	+	H3Eular.h
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Memory.h"
#include "H3Eular.h"

/*
 brief :
*/
H3Eular::H3Eular () 
{
	identify () ;
}

/*
 brief :
*/
H3Eular::H3Eular ( iF32 bank , iF32 head , iF32 pitch )
{
	this->bank = bank ;
	this->head = head ;
	this->pitch = pitch ;
}

/*
 brief :
*/
H3Eular::~H3Eular ()
{
}

/*
 brief :
*/
void H3Eular::identify ()
{
	this->bank = 0.0f ;
	this->head = 0.0f ;
	this->pitch = 0.0f ;
}

/*
 brief : set Bank direction
*/
void H3Eular::setBank ( iF32 Bank ) 
{
	this->bank = Bank ;
}

/*
 brief : set Head direction
*/
void H3Eular::setHead ( iF32 Head ) 
{
	this->head = Head ;
}

/*
 brief : set pitch direction
*/
void H3Eular::setPitch ( iF32 pitch ) 
{
	this->pitch = pitch ;
}

/*
 brief : return Bank direction
*/
iF32 H3Eular::getBank () 
{
	return this->bank ;
}

/*
 brief : return Head direction
*/
iF32 H3Eular::getHead () 
{
	return this->head ;
}

/*
 brief : return pitch direction
*/
iF32 H3Eular::getPitch () 
{
	return this->pitch ;
}