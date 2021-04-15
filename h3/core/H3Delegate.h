
/*

	+	H3Delegate.h
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

#ifndef H3Delegate_H
#define H3Delegate_H

#include "H3Type.h"
#include "H3Memory.h"
#include "H3MathUtil.h"

class H3Delegate : virtual public H3Memory
{

public :

	/*
	 brief :
	*/
	H3Delegate () ;

	/*
	 brief :
	*/
	~H3Delegate () ;
	
	/*
	 brief :
	*/
	static H3Delegate* getInstance () ;

    /*
	 brief : 
	*/
	void init ( iU32 frameTimes ) ;

    /*
     brief :
    */
	void start ( void* scene ) ;

    /*
     brief :
    */
	void stop () ;

    /*
     brief :
    */
	void update ( iF32 deltaT ) ;

    /*
     brief :
    */
	void resize ( iU32 width , iU32 height ) ;

    /*
     brief :
    */
	static iS8* getVersion () ;

    /*
     brief : release all objects and components
    */
	void exit () ;

    /*
     brief :
    */
	void keyEvent ( H3KeyEvent k ) ;

    /*
     brief :
    */
	void mouseEvent ( H3MouseEvent e ) ;

    /*
     brief :
    */
	void accelerometerEvent ( H3AccelerometerEvent e ) ;

	/*
	 brief : rendering all to texture
	*/
	void renderToTexture () ;

	/*
	 brief : return texture id
	*/
	iU32 getRenderTexture () ;

	/*
	 brief : record draw calls per pass
	*/
	void recordDrawCall ( iU32 drawcall ) ;

	/*
	 brief : record vertices per pass
	*/
	void recordVertices ( iU32 vertices ) ;

	/*
	 brief : release singleton
	*/
	static void release () ;

	/*
	 brief : get current matrix
	*/
	H3Matrix4x4f getMatrix ( H3MATRIX_TYPE type ) ;

	/*
	 brief : set rendering background color
	*/
	void setBackgroundColor ( H3Vector3f color ) ;

	/*
	 brief : enable shadow rendering
	*/
	void setShadowEnable ( bool enable ) ;

	/*
	 brief : return current camera
	*/
	void* getCurrentCamera () ;

	/*
	 brief : 
	*/
	void setWireFrameEnable ( bool enable ) ;

	/*
	 brief : 
	*/
	bool getWireFrameEnable () ;

	//	speical method for soda
	void loadObject ( const iS8* model , const iS8* mtl ) ;
	//

private :
    /*
     brief :
    */
	bool run ;

    /*
     brief :
    */
	iU32 fps ;
	  
    /*  
     brief :
    */
	void* render ;

    /*
     brief :
    */
	void* scene ;

    /*
     brief :
    */
	iU32 drawCall ;

    /*
     brief :
    */
	iU32 drawVertices ;

    /*
     brief : rendering with wire frame
    */
	bool wireFrameRendering ;

#ifdef H3_QT_5_4_2
private :
	QOpenGLVertexArrayObject vao ;
	QOpenGLBuffer vertexBuffer ;
    QOpenGLBuffer normalBuffer ;
    QOpenGLBuffer textureUVBuffer ;
    QOpenGLBuffer indexBuffer ;
#endif

} ;

#endif