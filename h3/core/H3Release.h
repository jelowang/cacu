
/*

	+	H3Release.h
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

#ifndef H3Release_H
#define H3Release_H

#include "H3Std.h"

class H3Release : virtual public H3Memory
{
public:
	H3Release () ;
	virtual ~H3Release () ;

	/*
	 brief :
	*/
	virtual void use () ;

	/*
	 brief :
	*/
	virtual void deUse () ;

	/*
	 brief :
	*/
	virtual bool isUse () ;

    /*
     brief :
    */
	virtual bool release () ;

	/*
	 brief :
	*/
	virtual void autoRelease () ;

	/*
	 brief :
	*/
	virtual bool isAutoRelease () ;

private :
	bool enableAutoRelease ;
	iU32 refCount ;

} ;

#endif