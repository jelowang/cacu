
#include "H3MeshRender.h"
#include "H3Object.h"
#include "CacuMainScene.h"
#include "H3Delegate.h"
#include "H3Image.h"
#include "H3DrawPrimitives.h"

static H3Object* object1 ;
static H3Object* object2 ;
static H3Object* object3 ;
static H3Object* object4 ;
static H3Object* object5 ;
static H3Object* object6 ;
static H3Object* object7 ;
static H3Object* object8 ;
static H3Object* wall1 ;
static H3Object* wall2 ;
static H3Object* wall3 ;
static H3Object* wall4 ;

static H3AmbientLight* h3AmbientLight = 0 ;
static H3DirectionalLight* h3DirectionalLight = 0 ;
static H3DirectionalLight* h3DirectionalLight2 = 0 ;
static H3PointLight* h3PointLight = 0 ;
static H3SpotLight* h3SpotLight = 0 ;

CacuMainScene::CacuMainScene() 
{

	H3Material material ;
	material.addTexture ( "diffuseTexture" , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/bricks2.png") , GL_TEXTURE0 ) ;
	material.addTexture ( "normalTexture" , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/bricks2_normal.png") , GL_TEXTURE1 ) ;
	material.addFloat ( "specularIntensity" , 1.0f ) ;
	material.addFloat ( "shininess" , 30.0f ) ; 
	material.addColor ( "ambientColor" , &H3Vector3f(1.0,1.0,1.0) ) ;

	H3Material material2 ;
	material2.addTexture ( "diffuseTexture" , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/bricks2.png") , GL_TEXTURE0 ) ;
	material2.addTexture ( "normalTexture" , H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/bricks2_normal.png") , GL_TEXTURE1 ) ;
	material2.addFloat ( "specularIntensity" , 10.0f ) ;
	material2.addFloat ( "shininess" , 30.0f ) ; 

	H3Material material3 ;
	material3.addFloat ( "specularIntensity" , 10.0f ) ;
	material3.addFloat ( "shininess" , 30.0f ) ; 

	H3MeshRender* planeMeshRenderMaterial = MMC_NEW H3MeshRender( MMC_NEW H3Mesh( "models/plane3.obj" , &material )  ) ;

	wall1 = MMC_NEW H3Object() ;
	wall1->addComponent( planeMeshRenderMaterial ) ;
	wall1->rotate ( H3Vector3f ( 1.0f , 0.0f , 0.0f ) , -90.0f ) ;
	wall1->setPosition ( H3Vector3f ( 0.0f , 3.0f , 50.0f ) ) ;
	wall1->setScale ( H3Vector3f ( 10.0f , 10.0f , 1.0f ) ) ;
	addChild ( wall1 ) ;

	h3AmbientLight = MMC_NEW H3AmbientLight( H3Vector3f(0.5f , 0.5f , 0.5f) ) ;
	h3DirectionalLight = MMC_NEW H3DirectionalLight( H3Vector3f(1.0f , 1.0f , 1.0f) , H3Vector3f(0.0f , 0.0f , 1.0f) , 0.5f ) ;
	h3PointLight = MMC_NEW H3PointLight( H3Vector3f(0.0f , 0.0f , 0.0f) , H3Vector3f(0.0f , 1.0f , 0.0f) , 1.0f , 1.0f , 0.09 , 0.032f ) ;
	h3SpotLight = MMC_NEW H3SpotLight( H3Vector3f(1.0f , 1.0f , 1.0f) , H3Vector3f(1.0f , 1.0f , 1.0f), H3Vector3f(1.0f , 1.0f , 1.0f) , 1.5f , 45.0f ) ;

	this->addComponent ( h3AmbientLight ) ;
	this->addComponent ( h3DirectionalLight ) ;
	this->addComponent ( h3PointLight ) ;
	this->addComponent ( h3SpotLight ) ;

	this->camera = MMC_NEW H3Camera() ;
	this->camera->initProjection ( 45.0f , 1024.0/720.0,0.1f,10000.0f ) ;
	this->camera->setPosition ( H3Vector3f( 0.0f , 10.0f , -30.0f ) ) ;
	this->camera->rotate ( H3Vector3f ( 1.0f , 0.0f , 0.0f ) , 15.0f ) ;
	this->addComponent ( camera ) ;
	   
	H3Delegate::getInstance()->setBackgroundColor ( H3Vector3f( 1.0f , 1.0f , 1.0f ) ) ;
	H3Delegate::getInstance()->setShadowEnable ( false ) ;
	
	this->inited = false ;

}

CacuMainScene::~CacuMainScene () 
{
}

bool CacuMainScene::init () 
{
	return true ;
}

/*
 brief :
*/
void CacuMainScene::update ( iF32 deltaT ) 
{

	static iF32 deltaTA = 0.0f ;


	iF32 randv = H3ToRadian(deltaTA);

	//h3DirectionalLight->setPosition ( H3Vector3f((iF32)H3Cosine(randv) , (iF32)H3Sine(randv) , (iF32)H3Cosine(randv)) ) ;
	//h3DirectionalLight2->setPosition ( H3Vector3f((iF32)H3Cosine(randv) , (iF32)H3Cosine(randv) , (iF32)H3Sine(randv)) ) ;
	//h3PointLight->setColor ( H3Vector3f((iF32)H3Sine(randv*100.0) , (iF32)H3Cosine(randv) , (iF32)H3Sine(randv)) ) ;
	//h3SpotLight->setColor ( H3Vector3f((iF32)H3Sine(randv*100.0) , (iF32)H3Cosine(randv) , (iF32)H3Sine(randv)) ) ;

	deltaTA = deltaTA + deltaT ;

	if ( deltaTA >= 360.0f )
		deltaTA = deltaTA - 0.0f ;

}

/*
 brief :
*/
void CacuMainScene::resize ( iU32 width , iU32 height ) 
{
	this->width = width ;
	this->height = height ;
	this->camera->initProjection ( 45.0f , (iF32 )width/(iF32 )height , 0.1f , 10000.0f ) ;
}

static void drawMap ()
{
	
	//	draw row
	iF32 r = 0.0f ;
	iS32 row = 0 ;
	for ( row = 0 ; row < 50 ; row ++ ) {
		r = (iF32)row / 50.0f * 100.0f ;		
		if ( 0 == r-50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f , 1.0f , 0.0f , 0.5f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;
		} else {
			H3DrawPrimitives::setColor ( 0.4f , 0.4f , 0.4f , 1.0f ) ;
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
			H3DrawPrimitives::setColor ( 0.0f , 1.0f , 0.0f , 0.5f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;
		} else {
			H3DrawPrimitives::setColor ( 0.4f , 0.4f , 0.4f , 1.0f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;
		}
	}
	r = (iF32)col / 50.0f * 100.0f ;
	H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;

}

/*
 brief :
*/
void CacuMainScene::draw ( iF32 deltaT ) 
{
	
	//drawMap () ;
	
#ifdef UI
    bool show_test_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImColor(114, 144, 154);

        ImGui_ImplGlfwGL3_NewFrame();

		ShowExampleAppMainMenuBar () ;

        // 1. Show a simple window
        // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
        {
            static float f = 0.0f;
            ImGui::Text("Hello, world!");
            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Test Window")) show_test_window ^= 1;
            if (ImGui::Button("Another Window")) show_another_window ^= 1;
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }

        // 2. Show another simple window, this time using an explicit Begin/End pair
        if (show_another_window)
        {
            ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello");
            ImGui::End();
        }

        // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow() 
        if (show_test_window)
        {
            ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
            ImGui::ShowTestWindow(&show_test_window);
        }

        ImGui::Render();
#endif

}

/*
 brief :
*/
void CacuMainScene::keyEvent ( H3KeyEvent k ) 
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
void CacuMainScene::mouseEvent ( H3MouseEvent e ) 
{

	static iF32 theLastThetaX = 0 ;
	static iF32 theLastThetaY = 0 ;

	if ( true == H3MouseEvent_Event_Click(e.e) ) {

		if ( H3MouseEvent_Right_Button != H3KeyEvent_Button(e.e) )
			return ;

		this->mouseClicked = true ;
		this->mouseClickedPosition.x = e.point.x[0] ;
		this->mouseClickedPosition.y = e.point.y[0] ;

		iF32 x = (iF32 )( e.point.x[0] - (this->width >>1 ) ) / ( this->width >> 1 ) ;
		iF32 y = (iF32 )( e.point.y[0] - (this->height >>1 ) ) / ( this->height >> 1 ) ;

		iF32 thetaX = ( x * 45.0f ) - theLastThetaX ;
		iF32 thetaY = ( y * 45.0f ) - theLastThetaY ;

		theLastThetaX = ( x * 45.0f ) ;
		theLastThetaY = ( y * 45.0f ) ;

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

