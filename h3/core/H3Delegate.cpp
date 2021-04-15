
/*

	+	H3Delegate.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Delegate.h"
#include "H3Render.h"
#include "H3Scene.h"
#include "H3Camera.h"
#include "H3ShaderCache.h"
#include "H3DrawPrimitives.h"

#ifdef TEST
static iF32 vert[] = 
{
	-1.0 , 1.0 , -1.0 ,
	1.0 , 1.0 , -1.0 ,
	1.0 , -1.0 , -1.0 ,
} ;

static iS32 indices[] = 
{
	0 , 1 , 2
} ;
#endif

#ifdef H3_QT_5_4_2
QOpenGLFunctions_3_3_Core Opengl ;
#endif

static H3Delegate* gInstance = NULL ;

/*
 brief : 
*/
H3Delegate::H3Delegate() 
{
	iLog ( "Hello3d" ) ;
	iLog ( "(C) Quantum Dynamics Lab." ) ;
	iLog ( "%s" , getVersion() ) ;
	iLog ( "glsl %s" , h3glGetString( GL_SHADING_LANGUAGE_VERSION )  ) ;

	this->run = false ;
	this->fps = 30 ;
	this->render = 0 ;
	this->scene = 0 ;
	this->render = (void* )MMC_NEW H3Render() ;
	this->drawCall = 0 ;
    this->drawVertices = 0 ;
	this->wireFrameRendering = false ;

}

/*
 brief :
*/
H3Delegate::~H3Delegate() 
{
	H3ShaderCache::getInstance()->release () ;
	H3DrawPrimitives::release() ;
	this->run = false ;
}
  
/*
 brief :
*/  
H3Delegate* H3Delegate::getInstance()
{
    if (!gInstance) {
        gInstance = MMC_NEW H3Delegate ();
        gInstance->init ( 30 ) ;
    }

    return gInstance;
}  
 
/*
 brief : release singleton
*/
void H3Delegate::release () 
{
    if (gInstance) {
		MMC_DELETE(gInstance) ;
		gInstance = NULL ;
    }
}

/*
 brief :
*/  
void H3Delegate::init ( iU32 frameTimes ) 
{

#ifdef H3_QT_5_4_2
	Opengl.initializeOpenGLFunctions();
#endif

	this->fps = frameTimes ;
	((H3Render* )this->render)->init () ;

	H3ShaderCache::getInstance()->init () ;
	H3DrawPrimitives::init () ;

}

/*
 brief :
*/
void H3Delegate::start ( void* scene ) 
{
	this->run = true ;

	this->scene = scene ;
	((H3Scene* )this->scene)->use () ;
}

/*
 brief :
*/
void H3Delegate::stop () 
{
	this->run = false ;
	((H3Render* )this->render)->clear() ;
}

void H3Delegate::update ( iF32 deltaT ) 
{

	H3ASSERT(this->scene) ;

	if ( false == this->run ) {
		return ;
	}

	static iU64 lasttime = 0 ;
	static iU32 frameTime = (1000 / this->fps) ;
	iU64 currenttime = SCHalGetTick () ;
	iU64 unprocessTime = currenttime - lasttime ;
    lasttime = SCHalGetTick () ;
    
    #ifdef H3_QT_5_4_2
	//	processing unproccssed frames
	while ( unprocessTime > frameTime ){
		unprocessTime = unprocessTime - frameTime ;
		this->scene->update( 0.016f ) ;	
	}
	#endif
   
	//	update actions
	H3ActionPool::getInstance()->update ( deltaT ) ;

	//	call child's update
	((H3Scene* )this->scene)->update ( 0.016f ) ;
	for ( SCClList* walker = ((H3Scene* )this->scene)->childs.next ; walker ; walker = walker->next ) {
		H3Object* object = (H3Object* )walker->element ;
		object->update ( 0.016f ) ;
	}

	//	call component's update
	((H3Scene* )this->scene)->update ( 0.016f ) ;
	for ( SCClList* walker = ((H3Scene* )this->scene)->components.next ; walker ; walker = walker->next ) {
		H3Component* component = (H3Component* )walker->element ;
		component->update ( 0.016f ) ; 
	}

	((H3Render* )this->render)->render ( 0.016f , ((H3Scene* )this->scene)->camera , ((H3Scene* )this->scene) ) ;

	//ImGui_ImplGlfwGL3_NewFrame();
    //ImGui::SetNextWindowPos(ImVec2(10,50));
	//ImGui::Begin("Example: Fixed Overlay", 0, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings) ;

	//ImGui::Text("draw calls: (%d)", this->drawCall);
	//ImGui::Text("vertices: (%d)", this->drawVertices);
	//ImGui::Text("fps: (%1.3f)", 1000.0f / (iF32 )unprocessTime);
    //ImGui::End();
    //ImGui::Render();

	//	clear draw-log status
	this->drawCall = 0 ;
	this->drawVertices = 0 ;

}

/*
 brief :
*/
void H3Delegate::resize ( iU32 width , iU32 height ) 
{

	h3glViewport ( 0 , 0 , width , height ) ;    

	((H3Scene* )this->scene)->resize ( width , height ) ;
	((H3Render* )this->render)->resize ( width , height ) ;

}

/*
 brief :
*/
iS8* H3Delegate::getVersion () 
{
	return "1.4.5" ;
}

/*
 brief : release all objects and components
*/
void H3Delegate::exit () 
{
	((H3Scene* )this->scene)->removeAllAndReleseSlef () ;
	MMC_DELETE((H3Render*)this->render) ;	
}

/*
 brief :
*/
void H3Delegate::keyEvent ( H3KeyEvent k ) 
{
	((H3Scene* )this->scene)->keyEvent ( k ) ;
}

/*
 brief :
*/
void H3Delegate::mouseEvent ( H3MouseEvent e ) 
{
	((H3Scene* )this->scene)->mouseEvent ( e ) ;
}

/*
 brief :
*/
void H3Delegate::accelerometerEvent ( H3AccelerometerEvent e ) 
{
	((H3Scene* )this->scene)->accelerometerEvent ( e ) ;
}

/*
 brief : rendering all to texture
*/
void H3Delegate::renderToTexture () 
{
	((H3Render* )this->render)->renderToTexture () ;
}

/*
 brief : return texture id
*/
iU32 H3Delegate::getRenderTexture () 
{
	iU32 id = 0 ;
	id = ((H3Render* )this->render)->getRenderTexture () ;
	return id ;
}

/*
 brief : record draw calls per pass
*/
void H3Delegate::recordDrawCall ( iU32 drawcall ) 
{
	this->drawCall = this->drawCall + drawcall ;
}

/*
 brief : record vertices per pass
*/
void H3Delegate::recordVertices ( iU32 vertices ) 
{
	this->drawVertices = this->drawVertices + vertices ;
}

/*
 brief : get current matrix
*/
H3Matrix4x4f H3Delegate::getMatrix ( H3MATRIX_TYPE type ) 
{
	H3Matrix4x4f matrix ;

	matrix = ((H3Render*)this->render)->getMatrix ( type ) ; 

	return matrix ;
}

/*
 brief : set rendering background color
*/
void H3Delegate::setBackgroundColor ( H3Vector3f color ) 
{
	((H3Render* )this->render)->setBackgroundColor ( color ) ;
}

/*
 brief : enable shadow rendering
*/
void H3Delegate::setShadowEnable ( bool enable ) 
{
	((H3Render* )this->render)->setShadowEnable ( enable ) ;
}

/*
 brief : return current camera
*/
void* H3Delegate::getCurrentCamera () 
{
	return ((H3Render* )this->render)->getCurrentCamera () ;
}

/*
 brief : 
*/
void H3Delegate::setWireFrameEnable ( bool enable ) 
{
	this->wireFrameRendering = enable ;
}

/*
 brief : 
*/
bool H3Delegate::getWireFrameEnable () 
{
	return this->wireFrameRendering ;
}

//	speical method for soda
void H3Delegate::loadObject ( const iS8* model , const iS8* mtl ) 
{
	((H3Scene* )this->scene)->loadObject ( model , mtl ) ;
}
//  
