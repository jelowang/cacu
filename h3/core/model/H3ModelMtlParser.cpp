
#include "H3Type.h"
#include "H3Std.h"
#include "H3MathUtil.h"
#include "H3Material.h"
#include "H3Memory.h"
#include "schal.h"
#include "H3ModelMtlParser.h"

#if 0

#define GET_OBJ_TOKENS(looper,counter,counter2,value,str)\
	while (str[looper] != '\0')\
	{\
		if ( '-' == str[looper] || '.' == str[looper] || ('0' <= str[looper] && '9' >= str[looper] ) ) \
		{\
			value[counter][counter2] = str[looper] ;\
			counter2 ++ ;\
		}\
		if ( ' ' == str[looper] ) \
		{\
			counter ++ ;\
			counter2 = 0 ;\
		}\
		looper ++ ;\
	}\

#endif

/*
 brief : mtl parser
*/
H3ModelMtlParser::H3ModelMtlParser ( iS8* data ) 
{

	memset ( &this->ObjToken , 0 , sizeof(this->ObjToken) ) ;
	memset ( &this->Obj , 0 , sizeof(this->Obj) ) ;

	this->Obj.length = strlen(data) ;
	this->Obj.data = (iS8* ) MMC_MALLOC ( this->Obj.length + 1 ) ;
	H3ASSERT(NULL != this->Obj.data) ;
	strcpy ( this->Obj.data , data ) ;

	for ( iU32 looper = 0 ; looper < this->Obj.length ; looper ++ ) {
		if ( 'A' <= this->Obj.data[looper] && this->Obj.data[looper] <= 'Z' ) {
			this->Obj.data[looper] = this->Obj.data[looper] - 'A'+'a' ;		
		}
	}

}

/*
 brief : 
*/
H3ModelMtlParser::~H3ModelMtlParser () 
{
	MMC_FREE(this->Obj.data) ;
}

/*
 brief : 
*/
void H3ModelMtlParser::start () 
{
	iS8 materialName[128] = {0} ;
	iS8 s[100][100] = {"wire_214228153","wire_224086086","wire_196088225"} ;
	int t = 0 ;
	while ( false == finishedLexerv() ) {

		generateLexerv () ;

		if ( H3MODEL_MTL_TOKEN_VALUE_NEWMTL == this->ObjToken.value ) {
			
			H3Material* material = MMC_NEW H3Material () ;
			generateLexerv () ;
			strcpy ( materialName , this->ObjToken.string ) ;

			while (1) {
				generateLexerv () ;
				if ( H3MODEL_MTL_TOKEN_VALUE_KA == this->ObjToken.value ) 
					break ;
				if ( H3MODEL_MTL_TOKEN_VALUE_KD == this->ObjToken.value ) 
					break ;
				if ( H3MODEL_MTL_TOKEN_VALUE_KS == this->ObjToken.value ) 
					break ;
			}

			if ( H3MODEL_MTL_TOKEN_VALUE_KA == generateLexervAhead (1) ) 
				generateLexerv () ;

			if ( H3MODEL_MTL_TOKEN_VALUE_KA == this->ObjToken.value ) {
				
				generateLexerv () ;
				iF32 v1 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v2 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v3 = atof( this->ObjToken.string ) ;

				material->addColor ( H3_MATERIAL_COLOR_AMBIENT , &H3Vector3f(v1,v2,v3) ) ;

			} 
			
			if ( H3MODEL_MTL_TOKEN_VALUE_KD == generateLexervAhead (1) ) 
				generateLexerv () ;

			if ( H3MODEL_MTL_TOKEN_VALUE_KD == this->ObjToken.value ) {
				
				generateLexerv () ;
				iF32 v1 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v2 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v3 = atof( this->ObjToken.string ) ;

				material->addColor ( H3_MATERIAL_COLOR_DIFFUSE , &H3Vector3f(v1,v2,v3) ) ;
				

			} 
			
			if ( H3MODEL_MTL_TOKEN_VALUE_KS == generateLexervAhead (1) ) 
				generateLexerv () ;

			if ( H3MODEL_MTL_TOKEN_VALUE_KS == this->ObjToken.value ) {
				
				generateLexerv () ;
				iF32 v1 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v2 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v3 = atof( this->ObjToken.string ) ;

				material->addColor ( H3_MATERIAL_COLOR_SPECULAR , &H3Vector3f(v1,v2,v3) ) ;
				

			}

			//strcpy ( s[t] , this->ObjToken.string ) ;
			//this->materialMap.insert ( std::map<const iS8* , H3Material* >::value_type((const iS8*)s[t], material) ) ;
			iU32 key = atoi ( materialName+5 ) ;
			this->materialMap[key] = material ;
			t ++ ;

		}

	}

	//H3Material* m = this->getMaterial (s[0])  ;

}

/*
 brief : 
*/
H3Material* H3ModelMtlParser::getMaterial ( const iS8* name ) 
{

	H3Material* material = NULL ;
	iU32 key = atoi ( name+5 ) ;

	std::map < iU32 , H3Material* >::iterator it ;
	it = this->materialMap.find( key ) ;
	if ( it != this->materialMap.end() ) {
		material = materialMap[key] ;
	}

	return material ;

}

/*
 brief : gernerate obj scripts token
*/
void H3ModelMtlParser::generateLexerv() 
{

	bool run = true ;
	iS32 state = 0 ;

	while ( true == run && false == finishedLexerv () ) {

		switch ( state ) {

			case -1 :
				run = false ;
			break ;
	
			case 0 :
			{

				iS8 symbol = getObjValue(0) ;
				iS8 symbolahead = getObjValue(1) ;
				
				if ( MTL_IS_ALPHA(symbol) ) {
					state = 3 ;
				} else if ( ' ' == symbol ) {
					this->ObjToken.string[this->ObjToken.walker] = ' ' ;
					this->ObjToken.string[this->ObjToken.walker+1] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_BLANK ;
					this->ObjToken.walker = 0 ;					
					objNext ( 1 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( ('-' == symbol && MTL_IS_NUMBER(symbolahead)) || (MTL_IS_NUMBER(symbol)) ) {
					state = 1 ;
				} else if ( '#' == symbol ) {
					state = 2 ;
					objNext ( 1 ) ;	
				} else if ( '\t' == symbol ) {
					state = 0 ;
					skipBlank() ;
				} else {
					//	skip this line
					while ( '\n' != getObjValue(0) && false == finishedLexerv() ) {
						this->ObjToken.lastValue = ObjToken.value ;
						this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_NULL ;					
						objNext ( 1 ) ;	
						skipBlank() ;
					}
					objNext ( 1 ) ;							
				}

			}
			break ;
			//	read digital
			case 1 : {

				iS8 symbol = getObjValue(0) ;

				while ( MTL_IS_NUMBER(symbol) || '-' == symbol || '.' == symbol ) {
					this->ObjToken.string[this->ObjToken.walker] = getObjValue(0) ;
					this->ObjToken.walker ++ ;
					objNext(1) ;
					symbol = getObjValue(0) ;

				}

				this->ObjToken.string[this->ObjToken.walker] = '\0' ;
				this->ObjToken.lastValue = ObjToken.value ;
				this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_NUMBER ;
				this->ObjToken.walker = 0 ;
				skipBlank() ;
				state = -1 ;

			}
			break ;
			//	skip commite line
			case 2 :
			{
				iS8 symbol = getObjValue(0) ;
				while ( '\n' != symbol && true != finishedLexerv ()  ) {
					objNext(1) ;
					symbol = getObjValue(0) ;
				}
				state = 0 ;
			}
			break ;

			//	get alpha
			case 3 :

				iS8 symbol = getObjValue(0) ;
	
				while ( MTL_IS_ALPHA(symbol) || MTL_IS_NUMBER(symbol) ) {
					this->ObjToken.string[this->ObjToken.walker] = getObjValue(0) ;
					this->ObjToken.walker ++ ;
					objNext(1) ;
					symbol = getObjValue(0) ;
				}

				this->ObjToken.string[this->ObjToken.walker] = '\0' ;
				this->ObjToken.lastValue = ObjToken.value ;
				if ( 0 == strcmp ( this->ObjToken.string , "newmtl" ) ) {
					this->ObjToken.value = H3MODEL_MTL_TOKEN_VALUE_NEWMTL ;
				} else if ( 0 == strcmp ( this->ObjToken.string , "ka" ) ) {
					this->ObjToken.value = H3MODEL_MTL_TOKEN_VALUE_KA ;
				} else if ( 0 == strcmp ( this->ObjToken.string , "kd" ) ) {
					this->ObjToken.value = H3MODEL_MTL_TOKEN_VALUE_KD ;
				} else if ( 0 == strcmp ( this->ObjToken.string , "ks" ) ) {
					this->ObjToken.value = H3MODEL_MTL_TOKEN_VALUE_KS ;
				} else {
					this->ObjToken.value = H3MODEL_MTL_TOKEN_VALUE_STRING ;
				}
				this->ObjToken.walker = 0 ;
				skipBlank() ;
				state = -1 ;

			break ;

		} 

	} 

}

/*
 brief : gernerate obj scripts token
 return : return lexical value
*/
iS32 H3ModelMtlParser::generateLexervAhead( iS32 lookahead ) 
{

	iS8 string[32] ;
	iS32 value ;
	iS32 walker ;
	iS32 ObjWalker ;
	iS32 lvalue = 0 ;

	//	backup
	ObjWalker = this->Obj.walker ;
	strcpy ( string , this->ObjToken.string ) ;
	value = this->ObjToken.value ;
	walker = this->ObjToken.walker ;

	for ( iS32 looper = 0 ; looper < lookahead ; looper ++ ) {
		generateLexerv() ;
	}
	lvalue = ObjToken.value ;

	//	restore
	this->Obj.walker = ObjWalker ;
	strcpy ( this->ObjToken.string , string ) ;
	this->ObjToken.value = value ;
	this->ObjToken.walker = walker ;	

	return lvalue ;

}

/*
 brief : lexical process has finished
*/
bool H3ModelMtlParser::finishedLexerv() 
{

	if ( this->Obj.walker < this->Obj.length ) {
		return false ;
	}

	return true ;

}

/*
 brief : get value from Obj struct
*/
iS8 H3ModelMtlParser::getObjValue ( iS32 lookahead ) 
{

	if ( this->Obj.walker >= this->Obj.length ) 
		return 0 ;

	if ( this->Obj.walker+lookahead >= this->Obj.length ) 
		return 0 ;

	iS8 ch = this->Obj.data[this->Obj.walker+lookahead] ;

	return ch ;

}

/*
 brief : 
*/
void H3ModelMtlParser::skipBlank () 
{
	while ( ' ' == getObjValue(0) || '\t' == getObjValue(0) ) {
		objNext(1) ;
	}
}

/*
 brief : 
*/
void H3ModelMtlParser::objNext ( iS32 step ) 
{

	if ( this->Obj.walker >= this->Obj.length ) 
		return ;

	this->Obj.walker = this->Obj.walker + step ;

}
