
/*
 +	H3Model.h
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

#ifndef H3Model_H
#define H3Model_H

#include "H3Std.h"
#include "H3MathUtil.h"
#include "H3Material.h"
#include "H3ModelMtlParser.h"
#include "H3ModelGroup.h"

/*
 brief : model abstractive class
*/
class H3Model : virtual public H3Memory
{
public :

	/*
     brief :
    */
	H3Model( const iS8* file , const iS8* mtl = NULL ) ;

	/*
     brief :
    */
	~H3Model() ;

public :

	vector<H3ModelGroup* > modelGroup ;

} ;

/*
 brief : Obj model
*/
class H3ModelObj : public H3Model
{
public :
    
	/*
     brief :
    */
	H3ModelObj( const iS8* file , const iS8* mtl , bool fromFile ) ;
    
	/*
     brief :
    */
	~H3ModelObj() ;

private :
	/*
     brief :
    */
	void init( iS8* data ) ;

	/*
	 brief : generate position
	*/
	iU32 generatePosition ( H3ModelGroup* model ) ;

	/*
	 brief : generate texCoord
	*/
	iU32 generateTexCoord ( H3ModelGroup* model ) ;

	/*
	 brief : generate normals
	*/
	iU32 generateNormal ( H3ModelGroup* model ) ;

	/*
	 brief : generate mtl 
	*/
	void generateMtl ( H3ModelGroup* model ) ;

	/*
	 brief : generate faces
	*/
	iU32 generateFace ( H3ModelGroup* model , iU32 totallPosition , iU32 totallTexCoord , iU32 totallNormal ) ;

	/*
	 brief :
	*/
	void generateF( iS32* i , iS32* t , iS32* n ) ;

	/*
     brief : gernerate obj scripts token
    */
	void generateLexerv() ;

	/*
	 brief : gernerate obj scripts token
	 return : return lexical value
	*/
	iS32 generateLexervAhead( iS32 lookahead ) ;

	/*
	 brief : lexical process has finished
	*/
	bool finishedLexerv() ;

	/*
	 brief : get value from Obj struct
	*/
	iS8 getObjValue ( iS32 lookahead ) ;

	/*
	 brief : 
	*/
	void skipBlank () ;

	/*
	 brief : 
	*/
	void objNext ( iS32 step ) ;

private :

	H3ModelMtlParser* mtlParser ;

	/*
     brief : save the last token here
    */
	struct
	{
		iS8 string[128] ;
		iS32 lastValue ;
		iS32 value ;
		iS32 walker ;
	} ObjToken ;

	/*
	 brief : load obj script here
	*/
	struct
	{
		iS8* data ;
		iS32 length ;
		iS32 walker ;
		iS32 faces ;
	} Obj ;
	
} ;

#endif
