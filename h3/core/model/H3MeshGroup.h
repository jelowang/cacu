
/*
 +	H3MeshGroup.h
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

#ifndef H3MeshGroup_H
#define H3MeshGroup_H

#include "H3Std.h"
#include "H3AABB.h"
#include "H3Material.h"

class H3MeshGroup : virtual public H3Memory
{
public :

	/*
	 brief :
	*/
	H3MeshGroup () ;

	/*
	 brief :
	*/
	~H3MeshGroup () ;

	/*
	 brief :
	*/
	void generateQuad () ;

	/*
	 brief :
	*/
	void releaseVertex () ;

public :

	iU32 id ;

	/*
	 brief :
	*/
	H3_Quad_4* quad ;

	/*
	 brief :
	*/
	iU32 totallQuad ;

	/*
	 brief : aabb per group
	*/
	H3AABB aabb ;

	/*
	 brief :
	*/
	H3Material* material ;

public :

	/* 
     brief :
    */
	struct 
	{
		iF32* position_data ;
		iU32 position_walker ;

		iF32* texCoord_data ;
		iU32 texCoord_walker ;

		iF32* normal_data ;
		iU32 normal_walker ;

		iF32* tangent_data ;
		iU32 tangent_walker ;

	} vertex ;

	/* 
     brief :
    */
	struct 
	{
		//	only holds vertex index here
		iU32* data ;
		iU32 length ;
	} face ;

} ;

#endif