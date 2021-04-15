
/*

	+	H3Camera.h
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

#ifndef H3Camera_H
#define H3Camera_H

#include "H3Std.h"
#include "H3Component.h"
#include "H3Matrix.h"
#include "H3Eular.h"

class H3Camera : public H3Component , virtual public H3Memory
{
public :
	/*
	 brief :
	*/
	H3Camera () ;

	/*
	 brief :
	*/
	~H3Camera () ;

	/*
	 brief :
	*/
	H3Matrix4x4f getProjection () ;

	/*
	 brief : projMatrix mul cameraMatrix
	*/
	H3Matrix4x4f getViewProjection () ;

	/*
	 brief :
	*/
	H3Matrix4x4f getView () ;

    /*
      brief : ortho
    */
    void initOrtho ( iF32 left , iF32 right , iF32 top , iF32 bottom , iF32 zNear , iF32 zFar ) ;
    
	/*
	 brief : right-hands
	*/
	void initProjection ( iF32 fov , iF32 aspectRatio , iF32 zNear , iF32 zFar ) ;
    
    /*
     brief : left-hands
    */
    void initProjection ( iF32 left , iF32 right , iF32 top , iF32 bottom , iF32 near , iF32 far ) ;

    /*
     brief : check is perspective projection or not
    */
	bool isPerspectiveProjection () ;

	/*
	 brief : set forward direction
	*/
	H3Vector3f getForward () ;

	/*
	 brief : set backward direction
	*/
	H3Vector3f getBackward () ;

	/*
	 brief : get upward direction
	*/
	H3Vector3f getUpward () ;

	/*
	 brief : get downward direction
	*/
	H3Vector3f getDownward () ;

	/*
	 brief : set left direction
	*/
	H3Vector3f getLeft () ;

	/*
	 brief : set right direction
	*/
	H3Vector3f getRight () ;

	/*
	 brief :
	*/
	void rotate ( H3Vector3f axis , iF32 amout ) ;

	/*
	 brief :
	*/
	void lookAt ( H3Vector3f axis ) ;

	/*
	 brief :
	*/
	void reset () ;

private :

	H3Matrix4x4f projection ;
	H3Eular eular ;

} ;

#endif