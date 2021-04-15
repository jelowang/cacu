
#include "H3MeshRender.h"
#include "H3Object.h"
#include "TextureAutoMapping.h"
#include "H3Raster.h"
#include "H3Image.h"
#include "H3Delegate.h"
#include "H3DrawPrimitives.h"
#include "H3PlaneRender.h"
#include "ShapePlane.h"
#include "MousePicker.h"

static bool rotateRoom = false ;
static H3Object* room ;
static H3Object* object5 ;
static H3Object* object6 ;
static H3Object* object7 ;
static H3Object* object8 ;
static H3Object* controlPoint0 ;
static H3Object* controlPoint1;
static H3Object* controlPoint2;
static H3Object* controlPoint3;
static H3Object* controlPoint4;
static H3Object* controlPoint5;
static H3Object* controlPoint6;
static H3Object* controlPoint7;
static H3Object* wallscreen ;

static H3AmbientLight* h3AmbientLight = 0 ;
static H3DirectionalLight* h3DirectionalLight = 0 ;
static H3DirectionalLight* h3DirectionalLight2 = 0 ;
static H3PointLight* h3PointLight = 0 ;
static H3SpotLight* h3SpotLight = 0 ;
MousePicker gMousePicker;
TextureAutoMapping::TextureAutoMapping()
{

	H3Delegate::getInstance()->setBackgroundColor(H3Vector3f(0.5f, 0.5f, 0.5f));


	H3Material material3;
	material3.addFloat ( H3_MATERIAL_SPECULARINTENSITY, 26.0f );
	material3.addFloat ( H3_MATERIAL_SHININESS, 0.6f );
	material3.addColor ( H3_MATERIAL_COLOR_AMBIENT, &H3Vector3f ( 1.0f, 1.0f, 1.0f ) );
	material3.addFloat ( H3_MATERIAL_SSS, 0.0f );
	material3.addFloat ( H3_MATERIAL_FRESNEL, 0.24f );
	material3.addTexture ( "diffuseTexture", H3Texture::create ( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL, "textures/room2.png" ), GL_TEXTURE0 );

	H3MeshRender* render = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( "/models/wall.obj", &material3 ) );
	wallscreen = MMC_NEW H3Object ( );
	wallscreen->addComponent ( render );
	wallscreen->setPosition ( H3Vector3f ( 0, 0.0, 0.0 ) );
	addChild ( wallscreen );
	
	std::vector<iF32> vertices;
	std::vector<iU32> indices;

	//	ground start
	vertices.push_back(-1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(4.164473);

	vertices.push_back(1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(4.164473);

	vertices.push_back(-1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(0.0);

	vertices.push_back(1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(0.0);

	indices.push_back(1	- 1);
	indices.push_back(2 - 1);
	indices.push_back(3 - 1);

	indices.push_back(2 - 1);
	indices.push_back(4 - 1);
	indices.push_back(3 - 1);
	//	ground end

	//	skywall start
	vertices.push_back(-1.0);
	vertices.push_back(1.0);
	vertices.push_back(4.164473);

	vertices.push_back(1.0);
	vertices.push_back(1.0);
	vertices.push_back(4.164473);

	vertices.push_back(-1.0);
	vertices.push_back(1.0);
	vertices.push_back(0.0);

	vertices.push_back(1.0);
	vertices.push_back(1.0);
	vertices.push_back(0.0);

	indices.push_back(7 - 1);
	indices.push_back(6 - 1);
	indices.push_back(5 - 1);

	indices.push_back(7 - 1);
	indices.push_back(8 - 1);
	indices.push_back(6 - 1);
	//	skywall end

	//	front start
	vertices.push_back(-1.0);
	vertices.push_back(1.0);
	vertices.push_back(4.164473);

	vertices.push_back(1.0);
	vertices.push_back(1.0);
	vertices.push_back(4.164473);

	vertices.push_back(-1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(4.164473);

	vertices.push_back(1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(4.164473);

	indices.push_back(9 - 1);
	indices.push_back(10 - 1);
	indices.push_back(11 - 1);

	indices.push_back(10 - 1);
	indices.push_back(12 - 1);
	indices.push_back(11 - 1);
	//	front end

	//	left start
	vertices.push_back(-1.0);
	vertices.push_back(1.0);
	vertices.push_back(4.164473);

	vertices.push_back(-1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(4.164473);

	vertices.push_back(-1.0);
	vertices.push_back(1.0);
	vertices.push_back(0.0);

	vertices.push_back(-1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(0.0);

	indices.push_back(13 - 1);
	indices.push_back(14 - 1);
	indices.push_back(15 - 1);

	indices.push_back(14 - 1);
	indices.push_back(16 - 1);
	indices.push_back(15 - 1);
	//	left end

	//	right start
	vertices.push_back(1.0);
	vertices.push_back(1.0);
	vertices.push_back(4.164473);

	vertices.push_back(1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(4.164473);

	vertices.push_back(1.0);
	vertices.push_back(1.0);
	vertices.push_back(0.0);

	vertices.push_back(1.0);
	vertices.push_back ( -1.0 );
	vertices.push_back(0.0);

	indices.push_back(19 - 1);
	indices.push_back(18 - 1);
	indices.push_back(17 - 1);

	indices.push_back(19 - 1);
	indices.push_back(20 - 1);
	indices.push_back(18 - 1);
	//	right end

	//	add box
	H3Material material;
	material.addFloat(H3_MATERIAL_SPECULARINTENSITY, 26.0f);
	material.addFloat(H3_MATERIAL_SHININESS, 0.6f);
	material.addColor ( H3_MATERIAL_COLOR_AMBIENT, &H3Vector3f ( 1.0f, 1.0f, 1.0f ) );
	material.addFloat(H3_MATERIAL_SSS, 0.0f);
	material.addFloat(H3_MATERIAL_FRESNEL, 0.24f);
	   
	H3MeshRender* planeMeshRenderMaterial = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( vertices, indices, &material ) );
	room = MMC_NEW H3Object();
	room->addComponent(planeMeshRenderMaterial);
	room->setBasicsAlignEnable ( false ) ;

#if 0
	//	texCoords
	H3Mesh* mesh = room->getMesh () ;
	H3MeshGroup* meshGroup = mesh->getMeshGroup ( 0 );
	iU32 faceIndex = 2 ;

	//	front
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = 0.28f ;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = 0.42f;
														 
	meshGroup->quad[6 * faceIndex + 1].texCoord.u = 0.74f ;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = 0.42f ;
														 
	meshGroup->quad[6 * faceIndex + 2].texCoord.u = 0.28f ;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = 0.76f;
														 
	meshGroup->quad[6 * faceIndex + 3].texCoord.u = 0.74f ;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = 0.42f;
														 
	meshGroup->quad[6 * faceIndex + 4].texCoord.u = 0.74f ;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = 0.76f;
														 
	meshGroup->quad[6 * faceIndex + 5].texCoord.u = 0.28f ;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = 0.76f;

	//	sky
	faceIndex = 1 ;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = 0.0f;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = 0.0f;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = 0.42f;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = 0.42f;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = 0.0f;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = 0.0f;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = 1.0f;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = 0.0f;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = 0.65f; 
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = 0.42f;

	//	left
	faceIndex = 3;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = 0.42f;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = 0.76f;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = 0.0f;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = 0.0f;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = 0.76f;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = 0.0f;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = 0.88f;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = 0.0f;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = 0.0f;

	//	right
	faceIndex = 4;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = 1.0f;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = 0.0f;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = 0.76f;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = 0.42f;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = 1.0f;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = 0.0f;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = 1.0f;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = 0.88f;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = 0.76f;

	//	ground
	faceIndex = 0;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = 0.76f;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = 0.76f;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = 1.0f;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = 0.76f;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = 0.65f;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = 1.0f;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = 0.28f;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = 1.0f;
#endif

	addChild(room);

	//	add outter control point
	H3Material material2;
	material2.addFloat(H3_MATERIAL_SPECULARINTENSITY, 26.0f);
	material2.addFloat(H3_MATERIAL_SHININESS, 0.6f);
	material2.addColor(H3_MATERIAL_COLOR_AMBIENT, &H3Vector3f(0.0f, 0.0f, 1.0f));
	material2.addFloat(H3_MATERIAL_SSS, 0.0f);
	material2.addFloat(H3_MATERIAL_FRESNEL, 0.24f);

	planeMeshRenderMaterial = MMC_NEW H3MeshRender(MMC_NEW H3Mesh("/models/box.obj", &material2));
	controlPoint4 = MMC_NEW H3Object();
	controlPoint4->addComponent(planeMeshRenderMaterial);
	controlPoint4->setScale(H3Vector3f(0.2, 0.2, 0.2));
	controlPoint4->setPosition(H3Vector3f(-1.0, 1.0, 0));
	controlPoint4->setBasicsAlignEnable ( false );
	addChild(controlPoint4);

	planeMeshRenderMaterial = MMC_NEW H3MeshRender(MMC_NEW H3Mesh("/models/box.obj", &material2));
	controlPoint5 = MMC_NEW H3Object();
	controlPoint5->addComponent(planeMeshRenderMaterial);
	controlPoint5->setScale(H3Vector3f(0.2, 0.2, 0.2));
	controlPoint5->setPosition(H3Vector3f(1.0, 1.0, 0));
	controlPoint5->setBasicsAlignEnable ( false );
	addChild(controlPoint5);

	planeMeshRenderMaterial = MMC_NEW H3MeshRender(MMC_NEW H3Mesh("/models/box.obj", &material2));
	controlPoint6 = MMC_NEW H3Object();
	controlPoint6->addComponent(planeMeshRenderMaterial);
	controlPoint6->setScale(H3Vector3f(0.2, 0.2, 0.2));
	controlPoint6->setPosition ( H3Vector3f ( -1.0, -1.0, 0 ) );
	controlPoint6->setBasicsAlignEnable ( false );
	addChild(controlPoint6);

	planeMeshRenderMaterial = MMC_NEW H3MeshRender(MMC_NEW H3Mesh("/models/box.obj", &material2));
	controlPoint7 = MMC_NEW H3Object();
	controlPoint7->addComponent(planeMeshRenderMaterial);
	controlPoint7->setScale(H3Vector3f(0.2, 0.2, 0.2));
	controlPoint7->setPosition ( H3Vector3f ( 1.0, -1.0, 0 ) );
	controlPoint7->setBasicsAlignEnable ( false );
	addChild(controlPoint7);

	//	add inner control point 
	planeMeshRenderMaterial = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( "/models/box.obj", &material ) );
	controlPoint0 = MMC_NEW H3Object ( );
	controlPoint0->addComponent ( planeMeshRenderMaterial );
	controlPoint0->setScale ( H3Vector3f ( 0.2, 0.2, 0.2 ) );
	controlPoint0->setPosition ( H3Vector3f ( -1.0, 1.0, 4.164473 ) );
	controlPoint0->setBasicsAlignEnable ( false );
	addChild ( controlPoint0 );

	planeMeshRenderMaterial = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( "/models/box.obj", &material ) );
	controlPoint1 = MMC_NEW H3Object ( );
	controlPoint1->addComponent ( planeMeshRenderMaterial );
	controlPoint1->setScale ( H3Vector3f ( 0.2, 0.2, 0.2 ) );
	controlPoint1->setPosition ( H3Vector3f ( 1.0, 1.0, 4.164473 ) );
	controlPoint1->setBasicsAlignEnable ( false );
	addChild ( controlPoint1 );

	planeMeshRenderMaterial = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( "/models/box.obj", &material ) );
	controlPoint2 = MMC_NEW H3Object ( );
	controlPoint2->addComponent ( planeMeshRenderMaterial );
	controlPoint2->setScale ( H3Vector3f ( 0.2, 0.2, 0.2 ) );
	controlPoint2->setPosition ( H3Vector3f ( -1.0, -1.0, 4.164473 ) );
	controlPoint2->setBasicsAlignEnable ( false );
	addChild ( controlPoint2 );

	planeMeshRenderMaterial = MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( "/models/box.obj", &material ) );
	controlPoint3 = MMC_NEW H3Object ( );
	controlPoint3->addComponent ( planeMeshRenderMaterial );
	controlPoint3->setScale ( H3Vector3f ( 0.2, 0.2, 0.2 ) );
	controlPoint3->setPosition ( H3Vector3f ( 1.0, -1.0, 4.164473 ) );
	controlPoint3->setBasicsAlignEnable ( false );
	addChild ( controlPoint3 );

	h3AmbientLight = MMC_NEW H3AmbientLight(H3Vector3f(0.5f, 0.5f, 0.5f));
	h3DirectionalLight = MMC_NEW H3DirectionalLight(H3Vector3f(1.0f, 1.0f, 1.0f), H3Vector3f(0.0f, 0.0f, 1.0f), 0.5f);

	this->addComponent(h3DirectionalLight); 
	this->addComponent(h3AmbientLight); 
	  
	this->camera = MMC_NEW H3Camera();
	this->camera->setPosition(H3Vector3f(0.0f, 0.0f, 0.0f));
	this->addComponent(this->camera);

	//shapePlane = MMC_NEW ShapePlane ( H3Vector3f ( -1.0, 1.0, 0.0 ), H3Vector3f ( 1.0, 1.0, 0.0 ), H3Vector3f ( -1.0, 0.0, 0.0 ), H3Vector3f ( 1.0, 0.0, 0.0 ), &material3 );

	//	add object to mouse picker
	gMousePicker.cameraRef = this->camera ;
	gMousePicker.roomRef = room ;
	gMousePicker.controlPointRef[0] = controlPoint0 ;
	gMousePicker.controlPointRef[1] = controlPoint1 ;
	gMousePicker.controlPointRef[2] = controlPoint2 ;
	gMousePicker.controlPointRef[3] = controlPoint3 ;

	gMousePicker.controlPointRef[4] = controlPoint4 ;
	gMousePicker.controlPointRef[5] = controlPoint5 ;
	gMousePicker.controlPointRef[6] = controlPoint6 ;
	gMousePicker.controlPointRef[7] = controlPoint7 ;


}

TextureAutoMapping::~TextureAutoMapping () 
{
}

bool TextureAutoMapping::init () 
{
	return true ;
} 

/*
 brief :
*/
void TextureAutoMapping::update ( iF32 deltaT ) 
{

	if ( this->camera->getType ( ) == H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC ) {
		this->camera->setPosition ( H3Vector3f ( 0.0, 0.0, 10.0f ) );
	}

	static iF32 deltaTA = 0.0f ;
	gMousePicker.update ( deltaT ) ;

	if ( rotateRoom  ) {

		room->rotate ( H3Vector3f(0,1.0,0) , 3.2f ) ;

	}

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
void TextureAutoMapping::draw ( iF32 deltaT ) 
{ 

	gMousePicker.draw ( deltaT ) ;

	//drawMap () ;

	//H3DrawPrimitives::setColor ( 1.0f , 0.0f , 0.0f , 1.0f ) ;
	//H3DrawPrimitives::drawLine ( ray.getO() , ray.getD().mul(10000.0f) ) ;

	//H3DrawPrimitives::drawLine ( H3Vector3f(0.0,0.0,0.3) , H3Vector3f(1.0,1.0,0.3) ) ;
}
 
/*
 brief :
*/
void TextureAutoMapping::resize ( iU32 width , iU32 height ) 
{
	this->width = width ;
	this->height = height ;
	this->camera->reset () ;
	this->camera->initOrtho ( -1.0 , 1.0 , 1.0 , -1.0 , 0.0f , 10000.0f ) ;
	this->camera->setPosition ( H3Vector3f(0,0.0,1000.0) ) ;
	gMousePicker.screen.x = width ;
	gMousePicker.screen.y = height ;
}

/*
 brief :
*/
void TextureAutoMapping::keyEvent ( H3KeyEvent k ) 
{
	bool pressed = H3KeyEvent_Event_Press(k.e) ;

	if ( false == pressed ) {
		return ;
	}

	iU32 key = H3KeyEvent_Key ( k.e ) ;

	if ( key == H3KeyEvent_Key_W ) {
		wallscreen->setVisible ( false ) ;
		gMousePicker.generateTextureCoord () ;

		
		this->camera->reset ( );
		this->camera->initProjection ( 41.0f, (iF32)width / (iF32)height, 0.1f, 10000.0f );
		this->camera->setPosition ( H3Vector3f ( 0., 0., -1.0 ) );
	
	}

	if ( key == H3KeyEvent_Key_S ) {
		wallscreen->setVisible ( true ) ;
		room->getMesh()->getMeshGroup ( 0 )->material->addTexture ( "diffuseTexture", H3Texture::create ( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL, "textures/default.png" ), GL_TEXTURE0 );
	} 

	if ( key == H3KeyEvent_Key_A ) {
		rotateRoom = !rotateRoom ;
	} 

}

/*
 brief :
*/
#include "H3Ray.h"
static H3Ray ray ;
static H3Vector2f clicked ;
static H3Vector2f screen ;
void TextureAutoMapping::mouseEvent ( H3MouseEvent e ) 
{

	if ( gMousePicker.mouseEvent ( e ) ) 
		return ;

	static iF32 theLastThetaX = 0;
	static iF32 theLastThetaY = 0;

	if ( true == H3MouseEvent_Event_Click ( e.e ) ) {

		if ( H3MouseEvent_Left_Button == H3KeyEvent_Button ( e.e ) ) {

			this->mouseClicked = true;
			this->mouseClickedPosition.x = e.point.x[0];
			this->mouseClickedPosition.y = e.point.y[0];

			iF32 x = (iF32)(e.point.x[0] - (this->width >> 1)) / (this->width >> 1);
			iF32 y = (iF32)(e.point.y[0] - (this->height >> 1)) / (this->height >> 1);

			iF32 thetaX = (x * 45.0f) - theLastThetaX;
			iF32 thetaY = (y * 45.0f) - theLastThetaY;

			theLastThetaX = (x * 45.0f);
			theLastThetaY = (y * 45.0f);

		}

	}
	else if ( true == H3MouseEvent_Event_Release ( e.e ) ) {

		if ( H3MouseEvent_Left_Button == H3KeyEvent_Button ( e.e ) ) {

		}

		this->mouseClicked = false;
	}
	else if ( true == H3MouseEvent_Event_Wheel ( e.e ) ) {
		if ( this->camera->getType ( ) == H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC )
			return;
		iF32 y = (iF32)e.point.y[0];
		if ( 1.0f == y ) {
			H3Vector3f direction = this->camera->getForward ( );
			direction = direction.mul ( 1.01f );
			H3Vector3f position = this->camera->getPosition ( );
			position = position.add ( direction );
			this->camera->setPosition ( position );
		}
		else if ( -1.0f == y ) {
			H3Vector3f direction = this->camera->getBackward ( );
			direction = direction.mul ( 1.01f );
			H3Vector3f position = this->camera->getPosition ( );
			position = position.add ( direction );
			this->camera->setPosition ( position );
		}

	}
	else if ( true == H3MouseEvent_Event_Moving ( e.e ) ) {

		if ( true == mouseClicked ) {

			iF32 x = (iF32)(e.point.x[0] - (this->width >> 1)) / (this->width >> 1);
			iF32 y = (iF32)(e.point.y[0] - (this->height >> 1)) / (this->height >> 1);

			iF32 thetaX = (x * 45.0f) - theLastThetaX;
			iF32 thetaY = (y * 45.0f) - theLastThetaY;

			if ( this->camera->getType ( ) == H3COMPONENT_TYPE_CAMERA_PERSPECTIVE ) {
				this->camera->rotate ( H3Vector3f ( 0.0f, 1.0f, 0.0f ), thetaX );
				this->camera->rotate ( this->camera->getRight ( ), thetaY );
			}

			theLastThetaX = (x * 45.0f);
			theLastThetaY = (y * 45.0f);
		}
	}

}

/*
 brief :
*/
void TextureAutoMapping::accelerometerEvent ( H3AccelerometerEvent e ) 
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
void TextureAutoMapping::loadObject ( const iS8* model , const iS8* mtl ) 
{
	H3MeshRender* h3MeshRender3 = MMC_NEW H3MeshRender( MMC_NEW H3Mesh( model , mtl , false ) ) ;
	H3Object* object = MMC_NEW H3Object() ;        
	object->addComponent( h3MeshRender3 ) ;  
	object->setPosition ( H3Vector3f ( 0.0f , 0.0f , 0.0f ) ) ;  
	addChild ( object ) ;
}