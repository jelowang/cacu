
/*

	+	H3Eular.h
	+	QDL

	+	hello3d
	+	realtime 3d engine


	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3Eular_H
#define H3Eular_H

#include "H3Type.h"
#include "H3Memory.h"
#include "H3Vector.h"

class H3Eular : virtual public H3Memory
{
public:
    
	/*
	 brief :
	*/
	H3Eular () ;

    /*
     brief :
    */
    H3Eular ( iF32 bank , iF32 head , iF32 pitch ) ;
    
    /*
     brief :
    */
    ~H3Eular () ;

    /*
     brief :
    */
    void identify () ;

	/*
	 brief : set bank direction
	*/
	void setBank ( iF32 bank ) ;

	/*
	 brief : set head direction
	*/
	void setHead ( iF32 head ) ;

	/*
	 brief : set pitch direction
	*/
	void setPitch ( iF32 pitch ) ;

    /*
     brief : return bank direction
    */
    iF32 getBank () ;

    /*
     brief : return head direction
    */
    iF32 getHead () ;

    /*
     brief : return pitch direction
    */
    iF32 getPitch () ;

private :
	/*
	 brief : bank
	*/
	iF32 bank ;

	/*
	 brief : heading
	*/
	iF32 head ;

	/*
	 brief : pitch
	*/
	iF32 pitch ;

};

#endif