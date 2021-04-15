
#ifndef MousePicker_H
#define MousePicker_H

#include "H3Std.h"
#include "H3Object.h"
#include "H3Camera.h"

class MousePicker
{
public:
	MousePicker();
	~MousePicker();

	void update ( iF32 deltaT );
	void draw ( iF32 deltaT );
	bool mouseEvent ( H3MouseEvent e );
	void generateTextureCoord ();

	H3Camera* cameraRef ;
	H3Object* roomRef ;
	H3Object* controlPointRef[8] ;
	H3Object* clickedObject ;

	bool mouseClicked ;
	H3Vector2i clicked ;
	H3Vector2i screen ;

private :
	void changeQuadPosition ( H3Vector3f position , H3Vector3f newPosition ) ;
	
	H3Object* intersect ( H3Vector2f point ) ;
	H3Object* intersect ( H3Ray ray );


} ;

#endif