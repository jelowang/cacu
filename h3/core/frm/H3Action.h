
/*

	+	H3Action.h
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

#ifndef H3Action_H
#define H3Action_H

#include "H3Std.h"
#include "H3Release.h"
#include "H3MathUtil.h"

/*
 breif : only a H3Object instance could have an action
*/
class H3Object ;
class H3Mesh ;
class H3MeshGroup ;

typedef void (H3Object::*H3ACTIONCALLBACK)( H3Object* ) ;
extern void H3ActionPoolDeleteAction ( void* action ) ;

/*
 brief : Base Action
*/
class H3Action : public H3Release , virtual public H3Memory
{
public :
	H3Action () {
		this->objectRef = nullptr ;
		this->callback = nullptr ;
	}
	virtual ~H3Action () {}
	virtual H3Object* getObjectRef () {
		return this->objectRef ;
	}
	virtual void finish () {
		if ( nullptr != callback ) {
			(objectRef->*callback) ( objectRef ) ;
		}
		if ( true == this->isAutoRelease () ) {
			if ( true == this->release () ) {
				H3ActionPoolDeleteAction ( this ) ;
			}
		}
	}
	virtual void update ( iF32 delta ) {}
protected :
	H3Object* objectRef ;
	H3ACTIONCALLBACK callback ;
} ;

/*
 brief : Head Direction Morphing Action
*/
class H3ActionHeadMorphing : public H3Action , virtual public H3Memory
{
protected :
	H3ActionHeadMorphing ( H3Object* object , H3MeshGroup* meshGroup , iF32 speed , H3ACTIONCALLBACK callback ) ;
public :
	virtual ~H3ActionHeadMorphing () ;

	/*
	 brief : create an instance
	 param : 
			(1) morping mesh group , if nullptr its means all mesh group needs morping as same amout
			(2) morping speed
			(3) callback when morping finished
	*/
	static H3ActionHeadMorphing* create ( H3Object* object , H3MeshGroup* meshGroup , iF32 speed , H3ACTIONCALLBACK callback ) ;

	virtual void update ( iF32 delta ) ;

protected :
	H3MeshGroup* meshGroupRef ;
	iF32 speed ;

} ;

/*
 brief : time interval of an action
*/
class H3ActionInterval : public H3Action , virtual public H3Memory
{
protected :
	H3ActionInterval ( H3Action* action , iF32 duration ) ;
public :
	virtual ~H3ActionInterval () ;
	static H3ActionInterval* create ( H3Action* action , iF32 duration ) ;
	virtual bool isFinished () ;
	virtual void update ( iF32 delta ) ;	
protected :
	H3Action* actionRef ;
	iF32 duration ;
	iF32 elapsed ;
} ;

/*
 brief : ease out
*/
class H3ActionEaseOut : public H3ActionInterval , virtual public H3Memory
{
private :
	H3ActionEaseOut( H3Action* action , iF32 r , iF32 d ):H3ActionInterval(action,d){
		this->actionRef = action ;
		this->rate = r ;
		this->duration = d ;
	}
public :
	virtual ~H3ActionEaseOut(){}
	static H3ActionEaseOut* create ( H3Action* action , iF32 r , iF32 d ) ;
	virtual void update ( iF32 delta ) ;

private :
	iF32 rate ;

} ;

/*
 breif : Action Pool , this is a skeleton 
*/ 
class H3ActionPool : virtual H3Memory 
{
public :
	H3ActionPool () ;

	~H3ActionPool () ;

	static H3ActionPool* getInstance () ;

	void release () ;

	void addAction ( H3Action* action ) ;

	void deleleAction ( H3Action* action ) ;

	void update ( iF32 delta ) ;

private :
	void removeJunks () ;

private :
	std::map < H3Action* , H3Action* > actionMap ;
	SCClList deleteList ;
	static H3ActionPool* instance ;
} ;

#endif