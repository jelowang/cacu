
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef CacuMainScene_H
#define CacuMainScene_H

#include "H3Std.h"
#include "H3Texture.h"
#include "H3Scene.h"

class CacuMainScene : public H3Scene , virtual public H3Memory
{
public :

	/*
     brief :
    */
	CacuMainScene() ;

	/*
     brief :
    */
	~CacuMainScene () ;

	/*
     brief :
    */
	bool init () ;

	/*
     brief :
    */
	virtual void update ( iF32 deltaT ) ;

	/*
     brief :
    */
	virtual void draw ( iF32 deltaT ) ;

    /*
     brief :
    */
	virtual void resize ( iU32 width , iU32 height ) ;

    /*
     brief :
    */
	virtual void keyEvent ( H3KeyEvent k ) ;

    /*
     brief :
    */
	virtual void mouseEvent ( H3MouseEvent e ) ;

	virtual void accelerometerEvent ( H3AccelerometerEvent e ){}

	//	speical method for soda
	virtual void loadObject ( const iS8* model , const iS8* mtl ) {}
	//
private :

    /*
     brief :
    */
	bool inited ;

    /*
     brief :
    */
	iS32 width ;
	iS32 height ;

    /*
     brief :
    */
	H3Texture* texture ;

	bool mouseClicked ;
	H3Vector2f mouseClickedPosition ;

} ;

#endif
