
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3ModelGroup_H
#define H3ModelGroup_H

#include "H3Std.h"
#include "H3Material.h"

/*
 brief : primitive data is saved here
*/
class H3ModelGroup : virtual public H3Memory 
{

public :

	H3ModelGroup();

	~H3ModelGroup() ;

	inline void addPosition ( iF32 v ) {
		this->positionArray.push_back ( v ) ;
	}
	inline void addTexCoord ( iF32 v ) {
		this->texCoordArray.push_back ( v ) ;
	}
	inline void addNormal ( iF32 v ) {
		this->normalArray.push_back ( v ) ;
	}
	inline void addTangent ( iF32 v ) {
		this->tangentArray.push_back ( v ) ;
	}
	inline void addVertexIndices ( iS32 v ) {
		this->vertexIndices.push_back ( v ) ;
	}
	inline void addTexCoordIndices ( iS32 v ) {
		this->texCoordIndices.push_back ( v ) ;
	}
	inline void addNormalIndices ( iS32 v ) {
		this->normalIndices.push_back ( v ) ;
	}
	inline void addMaterial ( const iS8* name , iF32 value ) ;
	inline void setHasNormal ( bool has ) {
		this->hasNormal = has ;
	}
	inline void setHasTexCoord ( bool has ) {
		this->hasTexCoord = has ;
	}

public :

	iU32 id ;
	iU32 faces ;
	bool hasNormal ;
	bool hasTexCoord ;

	vector<iF32> positionArray ;
	vector<iF32> texCoordArray ;
	vector<iF32> normalArray ;
	vector<iF32> tangentArray ;
	
	/*
	 brief : faces indices
	*/
	vector<iS32> vertexIndices ;
	vector<iS32> texCoordIndices ;
	vector<iS32> normalIndices ;

	/*
	 brief : material
	*/
	H3Material* material ;

} ;
#endif
