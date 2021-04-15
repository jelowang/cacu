
/*
 +	H3Model.h
 +	QDL
 
 +	hello3d
 +	realtime 3d engine
 
 
 +	(C) Quantum Dynamics Lab.
        FACEGOOD.
 +
 
*/

#include "H3Memory.h"
#include "H3Model.h"
#include "schal.h"

/*
 brief : H3Model
*/
H3Model::H3Model( const iS8* file , const iS8* mtl ) 
{
}

#if 0
/*
 brief :
*/
H3Model::H3Model( H3Quad* quad , iU32 totallQuad , H3Indices* faces , iU32 totallFaces , H3Indices* texCoordIndices , iU32 totallTexCoordIndices ) 
{
	

	for ( iS32 walker = 0 ; walker < (iS32 )totallQuad ; walker ++ ) {
		this->positionArray.push_back ( quad[walker].vertex.x ) ;
		this->positionArray.push_back ( quad[walker].vertex.y ) ;
		this->positionArray.push_back ( quad[walker].vertex.z ) ;
	}

	for ( iS32 walker = 0 ; walker < (iS32 )totallQuad ; walker ++ ) {
		this->texCoordArray.push_back ( quad[walker].texCoord.x ) ;
		this->texCoordArray.push_back ( quad[walker].texCoord.y ) ;
	}

	for ( iS32 walker = 0 ; walker < (iS32 )totallFaces ; walker ++ ) {
		this->vertexIndices.push_back ( faces[walker].index ) ;
	}

	for ( iS32 walker = 0 ; walker < (iS32 )totallTexCoordIndices ; walker ++ ) {
		this->texCoordIndices.push_back ( texCoordIndices[walker].index ) ;
	}

	this->hasTexCoord = true ;

}
#endif

/*
 brief :
*/
H3Model::~H3Model() 
{

	vector<H3ModelGroup*>::iterator iter;  
	for ( iter = this->modelGroup.begin(); iter != this->modelGroup.end(); iter++ ) {  
        MMC_DELETE(*iter) ;
    }
	vector<H3ModelGroup* >(this->modelGroup).swap(this->modelGroup) ;

}

/*  
 brief : obj model
*/
H3ModelObj::H3ModelObj( const iS8* file , const iS8* mtl , bool fromFile ) : H3Model(file,mtl)
{

	iU32 length = 0 ;
	iS8* data = NULL ;
	this->mtlParser = NULL ;
	 
	//	parse mtl
	if ( NULL != mtl ) {

		//	use 'file' as data striaght-forward
		do {
			if ( false == fromFile ) {
				data = (iS8* )mtl ;
				mtlParser = MMC_NEW H3ModelMtlParser ( data ) ;
				mtlParser->start () ;
			} else {

				void* fl = (void* )SCHalFileOpen ( mtl , "rb" ) ;

				if ( 0 == fl ) 
					break ;

				length = SCHalFileLength(fl) ;
				H3ASSERT( length > 0 ) ;

				data = (iS8* ) MMC_MALLOC( length );
	
				if ( 0 == data ) {
					iLog( "Mtl Memory Malloc Failed" ) ;
					SCHalFileClose ( fl ) ;
					break ;
				}

				SCHalFileRead ( fl , data , length ) ;
				SCHalFileClose ( fl ) ;

				mtlParser = MMC_NEW H3ModelMtlParser ( data ) ;
				mtlParser->start () ;
				MMC_FREE(data) ;

			}

		} while (0) ;

	} 

	//	parse obj
	do {

		//	use 'file' as data striaght-forward
		if ( false == fromFile ) {
			data = (iS8* )file ;
			init ( data ) ; 
		} else {

			void* fl = (void* )SCHalFileOpen ( file , "rb" ) ;

			if ( NULL == fl ) {
				iLog( "Obj %s is not exits" , file ) ;
				break ;
			}

			length = SCHalFileLength(fl) ;
			H3ASSERT( length > 0 ) ;

			data = (iS8* ) MMC_MALLOC( length );
	
			if ( 0 == data ) {
				iLog( "Obj Memory Malloc Failed" ) ;
				SCHalFileClose ( fl ) ;
				break ;
			}

			SCHalFileRead ( fl , data , length ) ;
			SCHalFileClose ( fl ) ;

			init ( data ) ;
			MMC_FREE(data) ;

		}

	} while (0) ;
	
}

/*
 brief :
*/
H3ModelObj::~H3ModelObj() 
{
}

/*
 brief :
*/
void H3ModelObj::init( iS8* data ) 
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

	iU32 id = 0 ;
	iU32 lastTotallVertex = 0 ;
	iU32 lastTotallTexCoord = 0 ;
	iU32 lastTotallNormal = 0 ;
	iU32 lastTotallFace = 0 ;
	while ( false == finishedLexerv() ) {

		H3ModelGroup* model = MMC_NEW H3ModelGroup() ;
		model->id = id ;
		
		iU32 totallVertex = 0 ;
		iU32 totallTexCoord = 0 ;
		iU32 totallNormal = 0 ;
		iU32 totallFace = 0 ;

		totallVertex = generatePosition ( model ) ;
		totallTexCoord = generateTexCoord ( model ) ;
		totallNormal = generateNormal ( model ) ;
		generateMtl ( model ) ;
		totallFace = generateFace ( model , lastTotallVertex , lastTotallTexCoord , lastTotallNormal ) ;

		this->modelGroup.push_back ( model ) ;

		lastTotallVertex += totallVertex ;
		lastTotallTexCoord += totallTexCoord ;
		lastTotallNormal += totallNormal ;
		lastTotallFace += 	totallFace ;
		  
		this->Obj.faces = this->Obj.faces + totallFace ;
		id ++ ;

	}

	//	clear
	MMC_FREE(this->Obj.data) ;
	this->Obj.length = 0 ;
	this->Obj.walker = 0 ;
	MMC_DELETE(this->mtlParser) ;
	this->mtlParser = NULL ;

	//iLog( "\"%s\" loaded , %d faces" , data , this->Obj.faces ) ;
	iLog( "%d faces" , this->Obj.faces ) ;  

}

/*
 brief : generate position
*/
iU32 H3ModelObj::generatePosition ( H3ModelGroup* model ) 
{
	iU32 totall = 0 ;
	bool stop = false ;
	while ( false == stop && false == finishedLexerv() ) {

		switch ( this->ObjToken.value ) {

			case H3MODEL_OBJ_TOKEN_VALUE_USEMTL :
				stop = true ;
			break ;	

			case H3MODEL_OBJ_TOKEN_VALUE_V :
			{

				generateLexerv () ;
				iF32 v1 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v2 = atof( this->ObjToken.string ) ;
				generateLexerv () ;

				iF32 v3 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
            
				model->addPosition ( v1 ) ;
				model->addPosition ( v2 ) ;
				model->addPosition ( v3 ) ;
				totall ++ ;

			}
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VT :
			{
				stop = true ;
			}
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VN :
			{
				stop = true ;
			}
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_F :
			{
				stop = true ;
			}
			break ;

			default :
				generateLexerv () ;
	
		} 

	}

	return totall ;

}

/*
 brief : generate texCoord
*/
iU32 H3ModelObj::generateTexCoord ( H3ModelGroup* model ) 
{
	iU32 totall = 0 ;
	bool stop = false ;
	while ( false == stop && false == finishedLexerv() ) {

		switch ( this->ObjToken.value ) {

			case H3MODEL_OBJ_TOKEN_VALUE_USEMTL :
				stop = true ;
			break ;	

			case H3MODEL_OBJ_TOKEN_VALUE_V :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VT :
			{
				generateLexerv () ;
				iF32 v1 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v2 = atof( this->ObjToken.string ) ;
				generateLexerv () ;

				model->addTexCoord ( v1 ) ;
				model->addTexCoord ( v2 ) ;
				model->setHasTexCoord ( true ) ;
				totall ++ ;
			}
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VN :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_F :
				stop = true ;
			break ;

			default :
				generateLexerv () ;
	
		} 

	}

	return totall ;

}

/*
 brief : generate normals
*/
iU32 H3ModelObj::generateNormal ( H3ModelGroup* model ) 
{
	iU32 totall = 0 ;
	bool stop = false ;
	while ( false == stop && false == finishedLexerv() ) {

		switch ( this->ObjToken.value ) {

			case H3MODEL_OBJ_TOKEN_VALUE_USEMTL :
				stop = true ;
			break ;	

			case H3MODEL_OBJ_TOKEN_VALUE_V :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VT :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VN :		
			{
				generateLexerv () ;
				iF32 v1 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v2 = atof( this->ObjToken.string ) ;
				generateLexerv () ;
				iF32 v3 = atof( this->ObjToken.string ) ;
				generateLexerv () ;

				model->addNormal ( v1 ) ;
				model->addNormal ( v2 ) ;
				model->addNormal ( v3 ) ;
				model->setHasNormal ( true ) ;
				totall ++ ;

			}
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_F :
				stop = true ;
			break ;

			default :
				generateLexerv () ;
	
		} 

	}

	return totall ;

}

/*
 brief : generate mtl 
*/
void H3ModelObj::generateMtl ( H3ModelGroup* model ) 
{

	bool stop = false ;
	while ( false == stop && false == finishedLexerv() ) {

		switch ( this->ObjToken.value ) {

			case H3MODEL_OBJ_TOKEN_VALUE_USEMTL :
			{
				generateLexerv () ;
				if ( this->mtlParser ) {
					model->material = this->mtlParser->getMaterial ( this->ObjToken.string ) ;
					model->material->addRef () ;
				} else {
					model->material = NULL ;
				}
			}
			break ;	

			case H3MODEL_OBJ_TOKEN_VALUE_V :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VT :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VN :		
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_F :
				stop = true ;
			break ;

			default :
				generateLexerv () ;
		
		}
	}


}

/*
 brief : generate faces
*/
iU32 H3ModelObj::generateFace ( H3ModelGroup* model , iU32 totallPosition , iU32 totallTexCoord , iU32 totallNormal ) 
{
	iU32 totall = 0 ;
	bool stop = false ;
	while ( false == stop && false == finishedLexerv() ) {

		switch ( this->ObjToken.value ) {

			case H3MODEL_OBJ_TOKEN_VALUE_USEMTL :
				stop = true ;
			break ;	

			case H3MODEL_OBJ_TOKEN_VALUE_V :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VT :
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_VN :		
				stop = true ;
			break ;

			case H3MODEL_OBJ_TOKEN_VALUE_F :
			{
				iS32 i1 = 0 ;
				iS32 i2 = 0 ;
				iS32 i3 = 0 ;
				iS32 i4 = 0 ;
				iS32 t1 = 0 ;
				iS32 t2 = 0 ;
				iS32 t3 = 0 ;
				iS32 t4 = 0 ;
				iS32 n1 = 0 ;
				iS32 n2 = 0 ;
				iS32 n3 = 0 ;
				iS32 n4 = 0 ;

				generateF ( &i1 , &t1 , &n1 ) ;
				generateF ( &i2 , &t2 , &n2 ) ; 
				generateF ( &i3 , &t3 , &n3 ) ;
			
				//	vertex index
				model->addVertexIndices ( i1-totallPosition ) ;
				model->addVertexIndices ( i2-totallPosition ) ;
				model->addVertexIndices ( i3-totallPosition ) ;

				//	texCoord index
				model->addTexCoordIndices ( t1-totallTexCoord ) ;
				model->addTexCoordIndices ( t2-totallTexCoord ) ;
				model->addTexCoordIndices ( t3-totallTexCoord ) ;

				//	normal index
				if ( -1 != n1 ) {
					model->addNormalIndices ( n1-totallNormal ) ;
					model->addNormalIndices ( n2-totallNormal ) ;
					model->addNormalIndices ( n3-totallNormal ) ;
				}

				model->faces ++ ;
				totall ++ ;

				//	check four points face
				iS32 lvalue = generateLexervAhead(1) ;
				if ( H3MODEL_OBJ_TOKEN_VALUE_NUMBER == lvalue ) {
					//	four points face
					generateF ( &i4 , &t4 , &n4 ) ;

					//	vertex index
					model->addVertexIndices ( i1-totallPosition ) ;
					model->addVertexIndices ( i3-totallPosition ) ;
					model->addVertexIndices ( i4-totallPosition ) ;

					//	texCoord index
					model->addTexCoordIndices ( t1-totallTexCoord ) ;
					model->addTexCoordIndices ( t3-totallTexCoord ) ;
					model->addTexCoordIndices ( t4-totallTexCoord ) ;

					//	normal index
					if ( -1 != n1 ) {
						model->addNormalIndices ( n1-totallNormal ) ;
						model->addNormalIndices ( n3-totallNormal ) ;
						model->addNormalIndices ( n4-totallNormal ) ;
					}

					model->faces ++ ;
					totall ++ ;
				}

			}
			break ;

			default :
				generateLexerv () ;
	
		} 

	}

	return totall ;
}

/*
 brief :
*/
void H3ModelObj::generateF( iS32* i , iS32* t , iS32* n ) 
{

	iS32 lvalue = 0 ;

	*i = -1 ;
	*t = -1 ;
	*n = -1 ;

	generateLexerv () ;
	*i = atoi( this->ObjToken.string )-1 ;	
	lvalue = generateLexervAhead (1) ;

	if ( H3MODEL_OBJ_TOKEN_VALUE_SLASH == lvalue ) {

		//	skip slash
		generateLexerv () ;

		lvalue = generateLexervAhead (1) ;

		//	normals
		//	x//x
		if ( H3MODEL_OBJ_TOKEN_VALUE_SLASH == lvalue ) {

			//	skip slash
			generateLexerv () ;
			//	generate number		
			generateLexerv () ;
			*n = atoi( this->ObjToken.string )-1 ;		

		}
		//	texCoords
		//	x/x
		else {
			//	generate number
			generateLexerv () ;
			*t = atoi( this->ObjToken.string )-1 ;
			lvalue = generateLexervAhead (1) ;

			//	x/x/x
			if ( H3MODEL_OBJ_TOKEN_VALUE_SLASH == lvalue ) {

				//	skip slash
				generateLexerv () ;
				//	generate number		
				generateLexerv () ;
				*n = atoi( this->ObjToken.string )-1 ;		

			} 

		}
	
	}

#if 0
	if ( H3MODEL_OBJ_TOKEN_VALUE_SLASH == lvalue ) {
		//	skip slash
		generateLexerv () ;
		//	generate number
		generateLexerv () ;
		*t = atoi( this->ObjToken.string )-1 ;
		lvalue = generateLexervAhead (1) ;
		
		if ( H3MODEL_OBJ_TOKEN_VALUE_SLASH == lvalue ) {
			//	skip slash
			generateLexerv () ;
			//	generate number		
			generateLexerv () ;
			*n = atoi( this->ObjToken.string )-1 ;
		}
	
	}
#endif

	return ;

}

/*
 brief : gernerate obj scripts token
*/
void H3ModelObj::generateLexerv() 
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

				if ( 'v' == symbol && ' ' == symbolahead ) {
					this->ObjToken.string[this->ObjToken.walker] = 'v' ;
					this->ObjToken.string[this->ObjToken.walker+1] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_V ;
					this->ObjToken.walker = 0 ;					
					objNext ( 1 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( 'v' == symbol && 't' == symbolahead ) {
					this->ObjToken.string[this->ObjToken.walker] = 'v' ;
					this->ObjToken.string[this->ObjToken.walker+1] = 't' ;
					this->ObjToken.string[this->ObjToken.walker+2] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_VT ;
					this->ObjToken.walker = 0 ;					
					objNext ( 2 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( 'v' == symbol && 'n' == symbolahead ) {
					this->ObjToken.string[this->ObjToken.walker] = 'v' ;
					this->ObjToken.string[this->ObjToken.walker+1] = 'n' ;
					this->ObjToken.string[this->ObjToken.walker+2] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_VN ;
					this->ObjToken.walker = 0 ;					
					objNext ( 2 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( 'f' == symbol && ' ' == symbolahead ) {
					this->ObjToken.string[this->ObjToken.walker] = 'f' ;
					this->ObjToken.string[this->ObjToken.walker+1] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_F ;
					this->ObjToken.walker = 0 ;					
					objNext ( 1 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( ' ' == symbol ) {
					this->ObjToken.string[this->ObjToken.walker] = ' ' ;
					this->ObjToken.string[this->ObjToken.walker+1] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_BLANK ;
					this->ObjToken.walker = 0 ;					
					objNext ( 1 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( '/' == symbol ) {
					this->ObjToken.string[this->ObjToken.walker] = '/' ;
					this->ObjToken.string[this->ObjToken.walker+1] = '\0' ;
					this->ObjToken.lastValue = ObjToken.value ;
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_SLASH ;
					this->ObjToken.walker = 0 ;					
					objNext ( 1 ) ;	
					skipBlank() ;
					state = -1 ;
				} else if ( ('-' == symbol && MTL_IS_NUMBER(symbolahead)) || (MTL_IS_NUMBER(symbol)) ) {
					state = 1 ;
				} else if ( '#' == symbol ) {
					state = 2 ;
					objNext ( 1 ) ;	
				} else if ( MTL_IS_ALPHA(symbol) ) {
					state = 3 ;
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

				while ( MTL_IS_NUMBER(symbol) || 'e' == symbol || '-' == symbol || '.' == symbol ) {
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
				if ( 0 == strcmp ( this->ObjToken.string , "usemtl" ) ) {
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_USEMTL ;
				} else {
					this->ObjToken.value = H3MODEL_OBJ_TOKEN_VALUE_STRING ;
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
iS32 H3ModelObj::generateLexervAhead( iS32 lookahead ) 
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
bool H3ModelObj::finishedLexerv() 
{

	if ( this->Obj.walker < this->Obj.length ) {
		return false ;
	}

	return true ;

}

/*
 brief : get value from Obj struct
*/
iS8 H3ModelObj::getObjValue ( iS32 lookahead ) 
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
void H3ModelObj::skipBlank () 
{
	while ( ' ' == getObjValue(0) || '\t' == getObjValue(0) ) {
		objNext(1) ;
	}
}

/*
 brief : 
*/
void H3ModelObj::objNext ( iS32 step ) 
{

	if ( this->Obj.walker >= this->Obj.length ) 
		return ;

	this->Obj.walker = this->Obj.walker + step ;

}

