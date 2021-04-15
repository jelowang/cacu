
#if defined(_MSC_VER)
#pragma once
#endif

#ifndef H3ModelMtlParser_H
#define H3ModelMtlParser_H

#define H3MODEL_MTL_TOKEN_VALUE_NULL -1
#define H3MODEL_MTL_TOKEN_VALUE_NEWMTL 0
#define H3MODEL_MTL_TOKEN_VALUE_STRING 1
#define H3MODEL_MTL_TOKEN_VALUE_KA 2
#define H3MODEL_MTL_TOKEN_VALUE_KD 3
#define H3MODEL_MTL_TOKEN_VALUE_KS 4

#define H3MODEL_OBJ_TOKEN_VALUE_NULL -1
#define H3MODEL_OBJ_TOKEN_VALUE_BLANK 0
#define H3MODEL_OBJ_TOKEN_VALUE_USEMTL 1
#define H3MODEL_OBJ_TOKEN_VALUE_STRING 2
#define H3MODEL_OBJ_TOKEN_VALUE_V 3
#define H3MODEL_OBJ_TOKEN_VALUE_VT 4
#define H3MODEL_OBJ_TOKEN_VALUE_VN 5
#define H3MODEL_OBJ_TOKEN_VALUE_F 6
#define H3MODEL_OBJ_TOKEN_VALUE_NUMBER 7
#define H3MODEL_OBJ_TOKEN_VALUE_SLASH 8
#define MTL_IS_ALPHA(x) (('a' <= x && 'z' >= x )||('A' <= x && 'Z' >= x )||('_' == x))
#define MTL_IS_NUMBER(x) ('0' <= x && '9' >= x )

class H3ModelMtlParser : virtual public H3Memory
{
public :
	H3ModelMtlParser ( iS8* data ) ;
	~H3ModelMtlParser () ;
	void start () ;
	H3Material* getMaterial ( const iS8* name ) ;
private :
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
	std::map < iU32 , H3Material* > materialMap ;

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