
/*

	+	H3Action.h
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Action.h"
#include "H3Mesh.h"
#include "H3MeshGroup.h"

static H3ActionPool* gInstance = nullptr ;

H3ActionHeadMorphing::H3ActionHeadMorphing ( H3Object* object , H3MeshGroup* mesGroup , iF32 speed , H3ACTIONCALLBACK callback ) {
	this->objectRef = object ;
	this->meshGroupRef = mesGroup ;
	this->speed = speed ;
	this->callback = callback ;
}

H3ActionHeadMorphing::~H3ActionHeadMorphing () {}

/*
brief : create an instance
param : 
		(1) morping mesh group , if nullptr its means all mesh group needs morping as same amout
		(2) morping speed
		(3) callback when morping finished
*/
H3ActionHeadMorphing* H3ActionHeadMorphing::create ( H3Object* object , H3MeshGroup* mesGroup , iF32 speed , H3ACTIONCALLBACK callback ) {
	H3ActionHeadMorphing* action = MMC_NEW H3ActionHeadMorphing(object,mesGroup,speed,callback) ;
	action->autoRelease () ;
	return action ;
}

void H3ActionHeadMorphing::update ( iF32 delta ) {

	iF32 sign = 1.0f ;

	if ( 0 == rand()% 2 ) {
		sign = -1.0f ;
	} else 
		sign = 1.0f ;

	//if nullptr its means all mesh group needs morping as same amout
	if ( nullptr == this->meshGroupRef ) {
		H3Mesh* mesh = (H3Mesh* )this->objectRef->getMesh() ;
		for ( iU32 walker = 0 ; walker < mesh->meshGroupList.size () ; walker ++ ) {
			H3MeshGroup* meshGroup = mesh->meshGroupList[walker] ;
			for ( iU32 looper = 0 ; looper < meshGroup->totallQuad ; looper ++ ) {
				meshGroup->quad[looper].vertex.y += meshGroup->quad[looper].vertex.y * (this->speed*sign) ;	
			}
		}   

	} else {
		for ( iU32 looper = 0 ; looper < this->meshGroupRef->totallQuad ; looper ++ ) {
			this->meshGroupRef->quad[looper].vertex.y += this->meshGroupRef->quad[looper].vertex.y*(this->speed*sign) ;
		}
	}
}

/*
 brief : action interval 
*/
H3ActionInterval::H3ActionInterval ( H3Action* action , iF32 d ) {
	this->actionRef = action ;
	this->duration = d ;
	this->elapsed = 0.0f ;
	action->use () ;
}

H3ActionInterval::~H3ActionInterval () {}

H3ActionInterval* H3ActionInterval::create ( H3Action* action , iF32 duration ) {
	H3ActionInterval* interval = MMC_NEW H3ActionInterval ( action , duration ) ;
	interval->autoRelease () ;
	return interval ;
}

bool H3ActionInterval::isFinished () {
	if ( this->elapsed >= this->duration )
		return true ;
		return false ;
}

void H3ActionInterval::update ( iF32 delta ) {
	if ( isFinished () ) {
		return ;
	}
	this->elapsed = this->elapsed + delta ;
	if ( true == isFinished () ) {
		this->actionRef->finish () ;
		if ( true == isAutoRelease () ) {
			if ( true == this->release () ) {
				H3ActionPoolDeleteAction ( this ) ;
			}
			return ;
		}
	}
	this->actionRef->update ( delta ) ;
}

/*
 brief : H3ActionEaseOut
*/
H3ActionEaseOut* H3ActionEaseOut::create ( H3Action* act , iF32 r , iF32 d ) 
{
	H3ActionEaseOut* action = MMC_NEW H3ActionEaseOut ( act , r , d ) ;
	action->autoRelease () ;
	return action ;
}

void H3ActionEaseOut::update ( iF32 delta ) 
{
	if ( isFinished () ) {
		return ;
	}

	if ( true == isFinished () ) {
		this->actionRef->finish () ;
		if ( true == isAutoRelease () ) {
			if ( true == this->release () ) {
				H3ActionPoolDeleteAction ( this ) ;
			}
			return ;
		}
	}
	this->actionRef->update ( this->elapsed ) ;
	iF32 ratio = H3Clampf ( 1.0f / powf ( this->elapsed , rate ) , 0.0f , 1.0f ) ; 
	this->elapsed = this->elapsed + (ratio*delta) ;
}

/*
 brief : H3ActionPool
*/
H3ActionPool::H3ActionPool () {
	SCClListInit ( &deleteList ) ;
}

H3ActionPool::~H3ActionPool () {
	//	release H3Action , ignore its refCount here , wo release these straightforward
	std::map < H3Action* , H3Action* >::iterator it ;
	for ( it = this->actionMap.begin() ; it != this->actionMap.end() ; ++it ) {
		MMC_DELETE(it->second) ;
	}
	SCClListClear ( &deleteList ) ;
}

H3ActionPool* H3ActionPool::getInstance () {
	if ( nullptr == gInstance) {
		gInstance = MMC_NEW H3ActionPool ();
	}
	return gInstance;
}

void H3ActionPool::release () {
	MMC_DELETE(this) ;
}

void H3ActionPool::addAction ( H3Action* action ) {
	std::map < H3Action* , H3Action* >::iterator it ;
	it = this->actionMap.find( action ) ;
	if ( it != this->actionMap.end() ) {
		iLog ( "FE : Action has already added" ) ;
		H3ASSERT ( it != this->actionMap.end() ) ;
		return ;
	}
	this->actionMap[action] = action ;
	action->use () ;
}

void H3ActionPool::deleleAction ( H3Action* action ) {
	//	just save the handle and delete it at removeJucks method
	SCClListInsert ( &deleteList , action ) ;
}

void H3ActionPool::removeJunks (){
	if ( true == this->actionMap.empty () ) {
		return ;
	}
	SCClList* walker = 0 ;
	std::map < H3Action* , H3Action* >::iterator it ;
	for ( walker = deleteList.next ; walker ; walker = walker->next ) {
		H3Action* handle = (H3Action* )walker->element ;
		it = this->actionMap.find( handle ) ;
		if ( it != this->actionMap.end() )
			this->actionMap.erase ( handle ) ;
	}
	
}

void H3ActionPool::update ( iF32 delta ) {		
	if ( true == this->actionMap.empty () ) {
		return ;
	}
	std::map < H3Action* , H3Action* >::iterator it ;
	for ( it = this->actionMap.begin() ; it != this->actionMap.end() ; ++it ) {
		it->second->update ( delta ) ;
	}
	removeJunks () ;
}

void H3ActionPoolDeleteAction ( void* action ) {
	H3ActionPool::getInstance()->deleleAction ( (H3Action* )action ) ;
}
