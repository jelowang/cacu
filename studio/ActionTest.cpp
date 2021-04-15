
#include "ActionTest.h"
#include "H3MeshRender.h"
#include "H3Mesh.h"
#include "H3DrawPrimitives.h"

static H3Object* gObject ;

ActionTest::ActionTest ( ){


	H3Material material ;
	material.addFloat ( H3_MATERIAL_SPECULARINTENSITY, 26.0f );
	material.addFloat ( H3_MATERIAL_SHININESS, 0.6f );
	material.addColor ( H3_MATERIAL_COLOR_AMBIENT, &H3Vector3f ( 1.0f, 1.0f, 1.0f ) );
	material.addFloat ( H3_MATERIAL_SSS, 0.0f );
	material.addFloat ( H3_MATERIAL_FRESNEL, 0.24f );

	gObject = MMC_NEW H3Object ( );
	gObject->addComponent ( MMC_NEW H3MeshRender ( MMC_NEW H3Mesh ( "models/111.obj", &material ) ) );
	gObject->setPosition ( H3Vector3f ( 0.0f, 0.0f, 0.0f ) );
	gObject->setBasicsAlignEnable ( true );
	addChild ( gObject );

	H3AmbientLight* am = MMC_NEW H3AmbientLight ( H3Vector3f ( 0.5f, 0.5f, 0.5f ) );
	this->addComponent ( am );
	H3DirectionalLight* dl = MMC_NEW H3DirectionalLight ( H3Vector3f ( 0.5f, 0.5f, 0.5f ), H3Vector3f ( 0.3f, -1.0f, 1.0f ), 1.0f );
	this->addComponent ( dl );

	this->camera = MMC_NEW H3Camera ( );
	this->camera->setPosition ( H3Vector3f ( 0, 0, 0 ) );
	this->addComponent ( camera );

}
ActionTest::~ActionTest ( ){}

void ActionTest::update ( iF32 deltaT ) {

	static int times = 0 ;
	if ( 0 == times % 2 ) {
	
		H3Mesh* mesh = gObject->getMesh ( );
		iU32 meshGroupId = rand ( ) % mesh->meshGroupList.size ();
		H3ActionHeadMorphing* action = H3ActionHeadMorphing::create (
			gObject, mesh->meshGroupList[meshGroupId], 0.1, 0
			);
		gObject->startAction ( H3ActionInterval::create ( action, 1.0 ) );

	}
}

void ActionTest::draw ( iF32 deltaT ){
	//	draw row
	iF32 r = 0.0f;
	iS32 row = 0;
	for ( row = 0; row < 50; row++ ) {
		r = (iF32)row / 50.0f * 100.0f;
		if ( 0 == r - 50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f, 0.0f, 1.0f, 1.0f );
			H3DrawPrimitives::drawLine ( H3Vector3f ( -50.0f, 0.0f, r - 50.0f ), H3Vector3f ( 50.0f, 0.0f, r - 50.0f ) );
		}
		else {
			H3DrawPrimitives::setColor ( 0.4f, 0.4f, 0.4f, 0.5f );
			H3DrawPrimitives::drawLine ( H3Vector3f ( -50.0f, 0.0f, r - 50.0f ), H3Vector3f ( 50.0f, 0.0f, r - 50.0f ) );
		}
	}
	r = (iF32)row / 50.0f * 100.0f;
	H3DrawPrimitives::drawLine ( H3Vector3f ( -50.0f, 0.0f, r - 50.0f ), H3Vector3f ( 50.0f, 0.0f, r - 50.0f ) );

	//draw col
	iS32 col = 0;
	for ( col = 0; col < 50; col++ ) {
		r = (iF32)col / 50.0f * 100.0f;
		if ( 0 == r - 50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f, 0.0f, 1.0f, 1.0f );
			H3DrawPrimitives::drawLine ( H3Vector3f ( r - 50.0f, 0.0f, -50.0f ), H3Vector3f ( r - 50.0f, 0.0f, 50.0f ) );
		}
		else {
			H3DrawPrimitives::setColor ( 0.4f, 0.4f, 0.4f, 0.5f );
			H3DrawPrimitives::drawLine ( H3Vector3f ( r - 50.0f, 0.0f, -50.0f ), H3Vector3f ( r - 50.0f, 0.0f, 50.0f ) );
		}
	}
	r = (iF32)col / 50.0f * 100.0f;
	H3DrawPrimitives::drawLine ( H3Vector3f ( r - 50.0f, 0.0f, -50.0f ), H3Vector3f ( r - 50.0f, 0.0f, 50.0f ) );
}

void ActionTest::resize ( iU32 width, iU32 height ){
	this->width = width ;
	this->height = height ;
	this->camera->initProjection ( 35.0f, (iF32)width / (iF32)height, 0.1f, 10000.0f );
	this->camera->setPosition ( H3Vector3f ( 0, 10.0, -30.0 ) );
	this->camera->rotate ( H3Vector3f ( 1, 0, 0 ), 15.0 );
}

void ActionTest::keyEvent ( H3KeyEvent k ){}

void ActionTest::mouseEvent ( H3MouseEvent e ){
	static iF32 theLastThetaX = 0;
	static iF32 theLastThetaY = 0;

	if ( true == H3MouseEvent_Event_Click ( e.e ) ) {

		{

			this->click.x = (iF32)e.point.x[0];
			this->click.y = (iF32)e.point.y[0];

			this->screen.x = (iF32)this->width;
			this->screen.y = (iF32)this->height;

			if ( this->camera->getType() == H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC ) {
				this->ray.setD ( this->camera->getForward ( ) );
				H3Vector3f O = H3Ray::castRay ( this->camera, this->click, this->screen ) + this->camera->getPosition ( );
				O.y = 1000.0f;
				this->ray.setO ( O );
			}
			else {
				this->ray.setO ( this->camera->getPosition ( ) );
				this->ray.setD ( H3Ray::castRay ( this->camera, this->click, this->screen ) );
			}

			this->mouseClicked = true;


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
		if ( this->camera->getType() == H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC )
			return;
		iF32 y = (iF32)e.point.y[0];
		if ( 1.0f == y ) {
			H3Vector3f direction = this->camera->getForward ( );
			direction = direction.mul ( 5.0f );
			H3Vector3f position = this->camera->getPosition ( );
			position = position.add ( direction );
			this->camera->setPosition ( position );
		}
		else if ( -1.0f == y ) {
			H3Vector3f direction = this->camera->getBackward ( );
			direction = direction.mul ( 5.0f );
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

			if ( this->camera->getType() == H3COMPONENT_TYPE_CAMERA_PERSPECTIVE ) {
				this->camera->rotate ( H3Vector3f ( 0.0f, 1.0f, 0.0f ), thetaX );
				this->camera->rotate ( this->camera->getRight ( ), thetaY );
			}
			else if ( this->camera->getType() == H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC ) {
				H3Vector3f direction = camera->getLeft ( );
				direction = direction.mul ( thetaX );
				H3Vector3f position = camera->getPosition ( );
				position = position.add ( direction );
				camera->setPosition ( position );

				direction = camera->getUpward ( );
				direction = direction.mul ( thetaY );
				position = camera->getPosition ( );
				position = position.add ( direction );
				camera->setPosition ( position );
			}

			theLastThetaX = (x * 45.0f);
			theLastThetaY = (y * 45.0f);
		}
	}
}

void ActionTest::accelerometerEvent ( H3AccelerometerEvent e ){}
void ActionTest::loadObject ( const iS8* model, const iS8* mtl ){}