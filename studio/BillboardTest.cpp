
#include "H3MeshRender.h"
#include "H3Object.h"
#include "BillboardTest.h"
#include "H3Raster.h"
#include "H3Image.h"
#include "H3Delegate.h"
#include "H3DrawPrimitives.h"
#include "H3PlaneRender.h"
#include "H3Billboard.h"

static H3Object* object4 ;
static H3Object* object5 ;
static H3Object* object6 ;
static H3Object* object7 ;
static H3Object* object8 ;
static H3Object* wall1 ;
static H3Object* wall2 ;
static H3Object* wall3 ;
static H3Object* wall4 ;
static H3Billboard* h3Billboard ;

static H3AmbientLight* h3AmbientLight = 0 ;
static H3DirectionalLight* h3DirectionalLight = 0 ;
static H3DirectionalLight* h3DirectionalLight2 = 0 ;
static H3PointLight* h3PointLight = 0 ;
static H3SpotLight* h3SpotLight = 0 ;

BillboardTest::BillboardTest() 
{

	h3AmbientLight = MMC_NEW H3AmbientLight(H3Vector3f(0.5f , 0.5f , 0.5f));
	h3DirectionalLight = MMC_NEW H3DirectionalLight( H3Vector3f(1.0f , 1.0f , 1.0f) , H3Vector3f(0.0f , 0.0f , 1.0f) , 0.5f ) ;

	this->addComponent ( h3DirectionalLight ) ; 
	this->addComponent ( h3AmbientLight ) ; 
	   
	this->camera = MMC_NEW H3Camera() ;
	this->camera->setPosition ( H3Vector3f( 0.0f , 0.0f , 0.0f ) ) ;
	this->addComponent ( this->camera ) ;
	   
	h3Billboard = MMC_NEW H3Billboard( 10.0f ) ;    
	h3Billboard->setScale ( H3Vector3f(1.0,1.0,1.0) ) ; 
	h3Billboard->rotate ( H3Vector3f(1.0,0.0,0.0) , 0.0f ) ;
	h3Billboard->getMaterial()->addColor ( H3_MATERIAL_COLOR_AMBIENT , &H3Vector3f(1.0,0,0) ) ;
	h3Billboard->getMaterial()->addFloat ( H3_MATERIAL_SPECULARINTENSITY , 30.0f ) ;
	h3Billboard->getMaterial()->addFloat ( H3_MATERIAL_SHININESS , 3.0f ) ; 
	h3Billboard->getMaterial()->addTexture ( "diffuseTexture" , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/bricks2_normal.png") , GL_TEXTURE0 ) ;
	h3Billboard->getMaterial()->addTexture ( "normalTexture" , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/bricks2_normal.png") , GL_TEXTURE1 ) ;
	addChild ( h3Billboard ) ;

	H3Delegate::getInstance()->setBackgroundColor ( H3Vector3f( 0.5f , 0.5f , 0.5f ) ) ;

}

BillboardTest::~BillboardTest () 
{
}

bool BillboardTest::init () 
{
	return true ;
} 

/*
 brief :
*/
void BillboardTest::update ( iF32 deltaT ) 
{

	static iF32 deltaTA = 0.0f ;
	//object4->rotate ( H3Vector3f(0,1.0,0) , 3.2f ) ;
	//object4->rotateAroundLocal ( H3Vector3f(1.0,0.0,0) , H3Vector3f(0.0,0.0,0.0) , 3.2f ) ;
	//object5->rotate ( H3Vector3f(0.0,0.0,1.0) , 0.1f ) ;
#if 0
	object1->rotate ( H3Vector3f(1.0,0.0,0) , 0.1f ) ;
	object8->rotate ( H3Vector3f(0,1.0,0) , -0.4f ) ;
	object3->rotate ( H3Vector3f(0,0.0,1.0) , 0.3f ) ;
	object5->rotate ( H3Vector3f(1.0,0.0,0) , 0.7f ) ;
	object6->rotate ( H3Vector3f(0,1.0,0) , -0.2f ) ;
#endif
	iF32 randv = H3ToRadian(deltaTA);

	//h3DirectionalLight->setColor ( H3Vector3f((iF32)H3Cosine(randv) , (iF32)H3Sine(randv) , (iF32)H3Cosine(randv)) ) ;
	//h3DirectionalLight2->setPosition ( H3Vector3f((iF32)H3Cosine(randv) , (iF32)H3Cosine(randv) , (iF32)H3Sine(randv)) ) ;
	//h3PointLight->setColor ( H3Vector3f((iF32)H3Sine(randv*100.0) , (iF32)H3Cosine(randv) , (iF32)H3Sine(randv)) ) ;
	//h3SpotLight->setColor ( H3Vector3f((iF32)H3Sine(randv*100.0) , (iF32)H3Cosine(randv) , (iF32)H3Sine(randv)) ) ;
	   
	deltaTA = deltaTA + deltaT ;

	if ( deltaTA >= 360.0f )
		deltaTA = 0.0f ; 

}

static void drawMap ()
{
	
	//	draw row
	iF32 r = 0.0f ;
	iS32 row = 0 ;
	for ( row = 0 ; row < 50 ; row ++ ) {
		r = (iF32)row / 50.0f * 100.0f ;		
		if ( 0 == r-50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f , 0.0f , 1.0f , 1.0f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;
		} else {
			H3DrawPrimitives::setColor ( 0.4f , 0.4f , 0.4f , 0.5f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;
		}
	}
	r = (iF32)row / 50.0f * 100.0f ;		
	H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;

	//draw col
	iS32 col = 0 ;
	for ( col = 0 ; col < 50 ; col ++ ) {
		r = (iF32)col / 50.0f * 100.0f ;
		if ( 0 == r-50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f , 0.0f , 1.0f , 1.0f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;
		} else {
			H3DrawPrimitives::setColor ( 0.4f , 0.4f , 0.4f , 0.5f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;
		}
	}
	r = (iF32)col / 50.0f * 100.0f ;
	H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;

	

}

/*
 brief :
*/
void BillboardTest::draw ( iF32 deltaT ) 
{ 
	drawMap () ;
	//H3DrawPrimitives::setColor ( 1.0f , 0.0f , 0.0f , 1.0f ) ;
	//H3DrawPrimitives::drawLine ( ray.getO() , ray.getD().mul(10000.0f) ) ;

	//H3DrawPrimitives::drawLine ( H3Vector3f(0.0,0.0,0.3) , H3Vector3f(1.0,1.0,0.3) ) ;
}
 
/*
 brief :
*/
void BillboardTest::resize ( iU32 width , iU32 height ) 
{
	this->width = width ;
	this->height = height ;
	this->camera->initProjection ( 45.0f , (iF32 )width/(iF32 )height , 0.1f , 10000.0f ) ;
	//this->camera->initOrtho ( -100.0 , 100.0 , 100.0 , -100.0 , 0.0f , 10000.0f ) ;
	//this->camera->setPosition ( H3Vector3f(0,0,100.0) ) ;
}

/*
 brief :
*/
void BillboardTest::keyEvent ( H3KeyEvent k ) 
{
	bool pressed = H3KeyEvent_Event_Press(k.e) ;

	if ( false == pressed ) {
		return ;
	}

	iU32 key = H3KeyEvent_Key ( k.e ) ;

	if ( key == H3KeyEvent_Key_W ) {
		H3Vector3f direction = this->camera->getForward () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	} else if ( key == H3KeyEvent_Key_S ) {
		H3Vector3f direction = this->camera->getBackward () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	} else if ( key == H3KeyEvent_Key_A ) {
		H3Vector3f direction = this->camera->getLeft () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	} else if ( key == H3KeyEvent_Key_D ) {
		H3Vector3f direction = this->camera->getRight () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	}
}

/*
 brief :
*/
#include "H3Ray.h"
static H3Ray ray ;
static H3Vector2f clicked ;
static H3Vector2f screen ;
void BillboardTest::mouseEvent ( H3MouseEvent e ) 
{

	static iF32 theLastThetaX = 0 ;
	static iF32 theLastThetaY = 0 ;

	if ( true == H3MouseEvent_Event_Click(e.e) ) {

		if ( H3MouseEvent_Left_Button == H3KeyEvent_Button(e.e) ) {
			

			clicked.x = (iF32 )e.point.x[0] ;
			clicked.y = (iF32 )e.point.y[0] ;

			screen.x = (iF32 )this->width ;
			screen.y = (iF32 )this->height ;

			ray.setO ( this->camera->getPosition () ) ;
			ray.setD(H3Ray::castRay ( this->camera , clicked , screen )) ;

			//h3Billboard->intersect ( ray ) ;

			return ;

		} else {

			this->mouseClicked = true ;
			this->mouseClickedPosition.x = e.point.x[0] ;
			this->mouseClickedPosition.y = e.point.y[0] ;

			iF32 x = (iF32 )( e.point.x[0] - (this->width >>1 ) ) / ( this->width >> 1 ) ;
			iF32 y = (iF32 )( e.point.y[0] - (this->height >>1 ) ) / ( this->height >> 1 ) ;

			iF32 thetaX = ( x * 45.0f ) - theLastThetaX ;
			iF32 thetaY = ( y * 45.0f ) - theLastThetaY ;

			theLastThetaX = ( x * 45.0f ) ;
			theLastThetaY = ( y * 45.0f ) ;
		
		}


	} else if ( true == H3MouseEvent_Event_Release(e.e) ) {
		this->mouseClicked = false ;
	} else if ( true == H3MouseEvent_Event_Wheel(e.e) ) {

		iF32 y = (iF32 )e.point.y[0] ;
		if ( 1.0f == y ) {
			H3Vector3f direction = this->camera->getForward () ;
			direction = direction.mul ( 5.0f ) ;
			H3Vector3f position = this->camera->getPosition () ;
			position = position.add ( direction ) ;
			this->camera->setPosition ( position ) ;
		} else if ( -1.0f == y ) {
			H3Vector3f direction = this->camera->getBackward () ;
			direction = direction.mul ( 5.0f ) ;
			H3Vector3f position = this->camera->getPosition () ;
			position = position.add ( direction ) ;
			this->camera->setPosition ( position ) ;
		}

	} else if ( true == H3MouseEvent_Event_Moving(e.e) ) {

		if ( true == mouseClicked ) {


			iF32 x = (iF32 )( e.point.x[0] - (this->width >>1 ) ) / ( this->width >> 1 ) ;
			iF32 y = (iF32 )( e.point.y[0] - (this->height >>1 ) ) / ( this->height >> 1 ) ;
	
			iF32 thetaX = ( x * 45.0f ) - theLastThetaX ;
			iF32 thetaY = ( y * 45.0f ) - theLastThetaY ;
	
			this->camera->rotate ( H3Vector3f ( 0.0f , 1.0f , 0.0f ) , thetaX ) ;
			this->camera->rotate ( this->camera->getRight () , thetaY ) ;
	
			theLastThetaX = ( x * 45.0f ) ;
			theLastThetaY = ( y * 45.0f ) ;

		}  

	} 
}

/*
 brief :
*/
void BillboardTest::accelerometerEvent ( H3AccelerometerEvent e ) 
{
	if ( fabs(e.x) > 1.0 ) {

		if ( H3MathZero (1.0f - e.x) ) {
		} else if ( 1.0f < e.x ) {
			this->camera->rotate ( H3Vector3f ( 0.0f , 1.0f , 0.0f ) , 1.0 ) ;
		} else {
			this->camera->rotate ( H3Vector3f ( 0.0f , 1.0f , 0.0f ) , -1.0 ) ;
		}
		    
	}  
}

/*
 brief :
*/
void BillboardTest::loadObject ( const iS8* model , const iS8* mtl ) 
{
	H3MeshRender* h3MeshRender3 = MMC_NEW H3MeshRender( MMC_NEW H3Mesh( model , mtl , false ) ) ;
	H3Object* object = MMC_NEW H3Object() ;        
	object->addComponent( h3MeshRender3 ) ;  
	object->setPosition ( H3Vector3f ( 0.0f , 0.0f , 0.0f ) ) ;  
	addChild ( object ) ;
}