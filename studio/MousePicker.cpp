
#include "H3Mesh.h"
#include "MousePicker.h"
#include "H3Ray.h"
#include "H3DrawPrimitives.h"

static H3Ray gRay ;
MousePicker::MousePicker (){
	clickedObject = nullptr ;
}
MousePicker::~MousePicker (){}

void MousePicker::update ( iF32 deltaT )
{

	//H3Object* object = controlPointRef[0] ;
	//H3Vector3f position = object->getPosition () ;
	//object->setPosition ( position.add ( H3Vector3f (0.,-0.001,0.) ) );
	//changeQuadPosition ( position , object->getPosition () ) ;												  
		 
}

void MousePicker::draw ( iF32 deltaT ) {


	//	draw grid
	H3Vector3f p0;
	H3Vector3f p1;
	H3Vector3f p2;
	H3Vector3f p3;
	H3Vector3f p4;
	H3Vector3f p5;

	H3MeshGroup* meshGroup = roomRef->getMesh ( )->getMeshGroup ( 0 );
	
	for ( int i = 0 ; i < 5 ; i ++ ) {
		iU32 faceIndex = i ;
		H3DrawPrimitives::setColor ( 0.0f, 1.0f, 0.0f, 1.0f );
		p0.x = meshGroup->quad[6 * faceIndex + 0].vertex.x;
		p0.y = meshGroup->quad[6 * faceIndex + 0].vertex.y;
		p0.z = meshGroup->quad[6 * faceIndex + 0].vertex.z - 0.5;

		p1.x = meshGroup->quad[6 * faceIndex + 1].vertex.x;
		p1.y = meshGroup->quad[6 * faceIndex + 1].vertex.y;
		p1.z = meshGroup->quad[6 * faceIndex + 1].vertex.z - 0.5;

		p2.x = meshGroup->quad[6 * faceIndex + 2].vertex.x;
		p2.y = meshGroup->quad[6 * faceIndex + 2].vertex.y;
		p2.z = meshGroup->quad[6 * faceIndex + 2].vertex.z - 0.5;

		p3.x = meshGroup->quad[6 * faceIndex + 3].vertex.x;
		p3.y = meshGroup->quad[6 * faceIndex + 3].vertex.y;
		p3.z = meshGroup->quad[6 * faceIndex + 3].vertex.z - 0.5;

		p4.x = meshGroup->quad[6 * faceIndex + 4].vertex.x;
		p4.y = meshGroup->quad[6 * faceIndex + 4].vertex.y;
		p4.z = meshGroup->quad[6 * faceIndex + 4].vertex.z - 0.5;

		p5.x = meshGroup->quad[6 * faceIndex + 5].vertex.x;
		p5.y = meshGroup->quad[6 * faceIndex + 5].vertex.y;
		p5.z = meshGroup->quad[6 * faceIndex + 5].vertex.z - 0.5;

		if ( i == 1 || i == 4 ) {
			H3DrawPrimitives::drawLine ( p0, p2 );
			H3DrawPrimitives::drawLine ( p2, p1 );
			H3DrawPrimitives::drawLine ( p3, p4 );
			H3DrawPrimitives::drawLine ( p4, p5 );
		} else {
			H3DrawPrimitives::drawLine ( p0, p1 );
			H3DrawPrimitives::drawLine ( p0, p2 );
			H3DrawPrimitives::drawLine ( p1, p4 );
			H3DrawPrimitives::drawLine ( p2, p4 );
		}

	}
	





}

bool MousePicker::mouseEvent ( H3MouseEvent e )
{

	bool ret = false ;

	static iF32 theLastClickedX = 0;
	static iF32 theLastClickedY = 0;
	
	if ( true == H3MouseEvent_Event_Click ( e.e ) ) {

		clicked.x = (iF32)e.point.x[0];
		clicked.y = (iF32)e.point.y[0];
		mouseClicked = true ;

		if ( H3MouseEvent_Left_Button == H3KeyEvent_Button ( e.e ) ) {


			iF32 x = (iF32)(e.point.x[0] - (this->screen.x >> 1)) / (this->screen.x >> 1);
			iF32 y = -1.0f * (iF32)(e.point.y[0] - (this->screen.y >> 1)) / (this->screen.y >> 1);
			

			if ( this->cameraRef->getType ( ) == H3COMPONENT_TYPE_CAMERA_ORTHOGRAPHIC ) {
				gRay.setD ( this->cameraRef->getForward ( ) );
				H3Vector3f O = H3Ray::castRay ( this->cameraRef, H3Vector2f ( this->clicked.x, this->clicked.y ), H3Vector2f ( this->screen.x, this->screen.y ) ) ;
				O.z = -1000.0f;
				gRay.setO ( O );
			}
			else {
				gRay.setO ( this->cameraRef->getPosition ( ) );
				gRay.setD ( H3Ray::castRay ( this->cameraRef, H3Vector2f ( this->clicked.x, this->clicked.y ), H3Vector2f ( this->screen.x, this->screen.y ) ) );
			}

			clickedObject = intersect ( gRay );

			if ( clickedObject  ) {
				theLastClickedX = x;
				theLastClickedY = y;
				iLog ( "hited" );
				ret = true ;
			}


		}

	}
	else if ( true == H3MouseEvent_Event_Release ( e.e ) ) {
		this->mouseClicked = false;
		clickedObject = nullptr ;
	}
	else if ( true == H3MouseEvent_Event_Moving ( e.e ) ) {
		
		if ( clickedObject ) {

			iF32 x = (iF32)(e.point.x[0] - (this->screen.x >> 1)) / (this->screen.x >> 1) ;
			iF32 y = -1.0f * (iF32)(e.point.y[0] - (this->screen.y >> 1)) / (this->screen.y >> 1) ;

			iF32 offsetX = x - theLastClickedX;
			iF32 offsetY = y - theLastClickedY;

			H3Vector3f p0 = clickedObject->getPosition () ;
			H3Vector3f p1( p0.x + offsetX , p0.y + offsetY , p0.z ) ;
			clickedObject->setPosition ( p1 );

			changeQuadPosition ( p0 , p1 );

			theLastClickedX = x;
			theLastClickedY = y;

			ret = true ;

		} 

	}

	return ret ;

}

void MousePicker::changeQuadPosition ( H3Vector3f position, H3Vector3f newPosition )
{

	H3Mesh* mesh = roomRef->getMesh () ;
	H3MeshGroup* meshGroup = mesh->getMeshGroup (0) ;

	for ( int walker = 0 ; walker < meshGroup->totallQuad ; walker ++ ) {
		H3Vector3f vec ( meshGroup->quad[walker].vertex.x , meshGroup->quad[walker].vertex.y , meshGroup->quad[walker].vertex.z ) ;
		if ( vec.equal (position) ) {
			meshGroup->quad[walker].vertex.x = newPosition.x ;
			meshGroup->quad[walker].vertex.y = newPosition.y ;
			meshGroup->quad[walker].vertex.z = newPosition.z ;
		}													 
	}

	mesh->aabb.setEmpty ( );

	for ( int walker = 0; walker < meshGroup->totallQuad; walker++ ) {
		H3Vector3f vec ( meshGroup->quad[walker].vertex.x, meshGroup->quad[walker].vertex.y, meshGroup->quad[walker].vertex.z );
		mesh->aabb.add ( vec );
	}


}

void MousePicker::generateTextureCoord ()
{

	H3Mesh* mesh = roomRef->getMesh ( );
	H3MeshGroup* meshGroup = mesh->getMeshGroup ( 0 );
	iU32 faceIndex = 0;

	meshGroup->material->addTexture ( "diffuseTexture", H3Texture::create ( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL, "textures/room2.png" ), GL_TEXTURE0 );

	//	front
	faceIndex = 2;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = (controlPointRef[0]->getPosition().x + 1.0) / 2.0 ;	//2.0 is the totall width for left to right in orthograph mode
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = (-1.0*controlPointRef[0]->getPosition().y + 1.0) / 2.0;
																					
	meshGroup->quad[6 * faceIndex + 1].texCoord.u = (controlPointRef[1]->getPosition().x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = (-1.0*controlPointRef[1]->getPosition ( ).y + 1.0) / 2.0;
																					
	meshGroup->quad[6 * faceIndex + 2].texCoord.u = (controlPointRef[2]->getPosition().x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = (-1.0*controlPointRef[2]->getPosition ( ).y + 1.0) / 2.0;
																					
	meshGroup->quad[6 * faceIndex + 3].texCoord.u = (controlPointRef[1]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = (-1.0*controlPointRef[1]->getPosition ( ).y + 1.0) / 2.0;
																				 
	meshGroup->quad[6 * faceIndex + 4].texCoord.u = (controlPointRef[3]->getPosition().x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = (-1.0*controlPointRef[3]->getPosition ( ).y + 1.0) / 2.0;
																				 
	meshGroup->quad[6 * faceIndex + 5].texCoord.u = (controlPointRef[2]->getPosition().x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = (-1.0*controlPointRef[2]->getPosition ( ).y + 1.0) / 2.0;

	//	ground
	faceIndex = 0;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = (controlPointRef[2]->getPosition ( ).x + 1.0) / 2.0;	
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = (-1.0*controlPointRef[2]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = (controlPointRef[3]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = (-1.0*controlPointRef[3]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = (controlPointRef[6]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = (-1.0*controlPointRef[6]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = (controlPointRef[3]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = (-1.0*controlPointRef[3]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = (controlPointRef[7]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = (-1.0*controlPointRef[7]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = (controlPointRef[6]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = (-1.0*controlPointRef[6]->getPosition ( ).y + 1.0) / 2.0;

	//	left
	faceIndex = 3;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = (controlPointRef[0]->getPosition ( ).x + 1.0) / 2.0;	
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = (-1.0*controlPointRef[0]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = (controlPointRef[2]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = (-1.0*controlPointRef[2]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = (controlPointRef[4]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = (-1.0*controlPointRef[4]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = (controlPointRef[2]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = (-1.0*controlPointRef[2]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = (controlPointRef[6]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = (-1.0*controlPointRef[6]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = (controlPointRef[4]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = (-1.0*controlPointRef[4]->getPosition ( ).y + 1.0) / 2.0;

	//	sky
	faceIndex = 1;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = (controlPointRef[4]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = (-1.0*controlPointRef[4]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = (controlPointRef[1]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = (-1.0*controlPointRef[1]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = (controlPointRef[0]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = (-1.0*controlPointRef[0]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = (controlPointRef[4]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = (-1.0*controlPointRef[4]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = (controlPointRef[5]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = (-1.0*controlPointRef[5]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = (controlPointRef[1]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = (-1.0*controlPointRef[1]->getPosition ( ).y + 1.0) / 2.0;

	//	right
	faceIndex = 4;
	meshGroup->quad[6 * faceIndex + 0].texCoord.u = (controlPointRef[5]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 0].texCoord.v = (-1.0*controlPointRef[5]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 1].texCoord.u = (controlPointRef[3]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 1].texCoord.v = (-1.0*controlPointRef[3]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 2].texCoord.u = (controlPointRef[1]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 2].texCoord.v = (-1.0*controlPointRef[1]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 3].texCoord.u = (controlPointRef[5]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 3].texCoord.v = (-1.0*controlPointRef[5]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 4].texCoord.u = (controlPointRef[7]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 4].texCoord.v = (-1.0*controlPointRef[7]->getPosition ( ).y + 1.0) / 2.0;

	meshGroup->quad[6 * faceIndex + 5].texCoord.u = (controlPointRef[3]->getPosition ( ).x + 1.0) / 2.0;
	meshGroup->quad[6 * faceIndex + 5].texCoord.v = (-1.0*controlPointRef[3]->getPosition ( ).y + 1.0) / 2.0;
																					   
}																	 				    
																			 		    
H3Object* MousePicker::intersect ( H3Vector2f point ) {

	for ( int walker = 0 ; walker < 8 ; walker ++ ) {

		H3Vector2f pos ( controlPointRef[walker]->getPosition ( ).x , controlPointRef[walker]->getPosition ( ).y ) ;
		H3MeshGroup* meshGroup = controlPointRef[walker]->getMesh()->getMeshGroup(0) ;
		H3AABB aabb = meshGroup->aabb ;
		
		H3Vector2f dir = point - pos;
		if ( 0.2 > dir.mag() ) {
			return controlPointRef[walker];
		}

	}

	return nullptr ;
}

H3Object* MousePicker::intersect ( H3Ray ray ) {

	H3Vector3f hit ;

	for ( int walker = 0 ; walker < 8 ; walker ++ ) {

		if ( 0 == controlPointRef[walker]->intersect ( ray , &H3Vector3f(1.0,0,0), hit ) ) {
			return controlPointRef[walker] ;
		}


	}

	return nullptr ;
}