
#include "H3ModelGroup.h"

H3ModelGroup::H3ModelGroup() 
{
	this->hasNormal = false ;	
	this->hasTexCoord = false ;	
	this->faces = 0 ;
}

H3ModelGroup::~H3ModelGroup() 
{
	vector<iF32 >(this->positionArray).swap(this->positionArray) ;
	vector<iF32 >(this->texCoordArray).swap(this->texCoordArray) ;
	vector<iF32 >(this->normalArray).swap(this->normalArray) ;

	vector<iS32 >(this->vertexIndices).swap(this->vertexIndices) ;
	vector<iS32 >(this->texCoordIndices).swap(this->texCoordIndices) ;
	vector<iS32 >(this->normalIndices).swap(this->normalIndices) ;
	
	if ( this->material ) 
		this->material->release () ;
}