
/*

	+	H3Scene.h
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

#ifndef H3Scene_H
#define H3Scene_H

#include "H3Std.h"
#include "H3Object.h"

class H3Scene : public H3Object , virtual public H3Memory
{
public :
	/*
     brief :
    */
	virtual void update ( iF32 deltaT ) = 0 ;

	/*
     brief :
    */
	virtual void draw ( iF32 deltaT ) = 0 ;

    /*
     brief :
    */
	virtual void resize ( iU32 width , iU32 height ) = 0 ;

    /*
     brief :
    */
	virtual void keyEvent ( H3KeyEvent k ) = 0 ;

    /*
     brief :
    */
	virtual void mouseEvent ( H3MouseEvent e ) = 0 ;

	/*
     brief :
    */
	virtual void accelerometerEvent ( H3AccelerometerEvent e ) = 0 ;

	//	speical method for soda
	virtual void loadObject ( const iS8* model , const iS8* mtl ) = 0 ;
	//

    /*
     brief :
    */
	H3Camera* camera ;

protected :
	/*
	brief :
	*/
	iS32 width ;
	iS32 height ;

} ;

#endif