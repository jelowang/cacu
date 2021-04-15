
#ifndef NavMeshTest_H
#define NavMeshTest_H

#include "H3Std.h"
#include "H3Scene.h"

class NavMeshTest : public H3Scene, virtual public H3Memory
{
public:
	NavMeshTest ( );
	~NavMeshTest ( );

	/*
	brief :
	*/
	virtual void update ( iF32 deltaT );

	/*
	brief :
	*/
	virtual void draw ( iF32 deltaT );

	/*
	brief :
	*/
	virtual void resize ( iU32 width, iU32 height );

	/*
	brief :
	*/
	virtual void keyEvent ( H3KeyEvent k );

	/*
	brief :
	*/
	virtual void mouseEvent ( H3MouseEvent e );

	/*
	brief :
	*/
	virtual void accelerometerEvent ( H3AccelerometerEvent e );

	virtual void loadObject ( const iS8* model, const iS8* mtl );


	H3Ray ray;
	H3Vector2f click;
	H3Vector2f screen;
	bool mouseClicked;

};

#endif