
/*
 +	H3Mesh.h
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

#ifndef H3Mesh_H
#define H3Mesh_H

#include "H3Std.h"
#include "H3Model.h"
#include "H3Object.h"
#include "H3MathUtil.h"
#include "H3Material.h"
#include "H3AABB.h"
#include "H3MeshGroup.h"

class H3Mesh : virtual public H3Memory
{
public :

	/*
     brief :
    */
	H3Mesh () ;

	/*
	 brief :
	*/
	H3Mesh ( std::vector<iF32> vertices , std::vector<iU32> indices , H3Material* material = nullptr ) ;

	/*
     brief :
    */
	H3Mesh ( const iS8* file , const iS8* mtl = NULL , bool fromFile = true ) ; 

	/*
	 brief :
	*/
	H3Mesh( const iS8* file , H3Material* material ) ;

	/* 
     brief :
    */
	~H3Mesh () ;

	/* 
	 brief : calling inside engine 
	*/
	void addRef () ;

	/*
	 brief : engine will call this method to release material memorys
	*/
	void release () ;

	/*
	 brief : get mesh group
	*/
	H3MeshGroup* getMeshGroup ( iU32 id ) ;

public :

	/*
	 brief : aabb for the whole mesh
	*/
	H3AABB aabb ;

	/*
	 brief :
	*/
	vector<H3MeshGroup* > meshGroupList ;

private :

	iS32 refCount ;

	/*
	 brief :
	*/
	void init ( const iS8* file , const iS8* mtl , bool fromFile , H3Material* material ) ;

	/*
	 brief :
	*/
	bool initWithModel ( H3Model* model , H3Material* material ) ;

	/*
	 brief :
	*/
	bool constructFaces ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup ) ;

	/*
	 brief :
	*/
	void saveVertex ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) ;

	/*
	 brief :
	*/
	void saveTexCoord ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) ;

	/*
	 brief :
	*/
	void saveNormal ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) ;

	/*
	 brief :
	*/
	void saveTangent ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) ;

	/*
	 brief : caculate normals
	*/
	bool caculateNormals ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup ) ;

	/*
	 brief :
	*/
	bool caculateTagents ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup ) ;


} ;

#endif