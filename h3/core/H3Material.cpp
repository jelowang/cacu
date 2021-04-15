
/*

	+	H3Material.cpp
	+	QDL
	
	+	hello3d
	+	realtime 3d engine
	
	+	(C) Quantum Dynamics Lab.
			FACEGOOD.
	+
 
*/

#include "H3Material.h"

/*
 brief :
*/
H3Material::H3Material()
{
	this->refCount = 0 ;
	this->defaultDiffuseTexture = 0 ;
	this->defaultNormalTexture = 0 ;
	//	set white as defualt
	this->addColor ( H3_MATERIAL_COLOR_AMBIENT , &H3Vector3f(1.0,1.0,1.0) ) ;
}

/*
 brief :
*/
H3Material::~H3Material()
{
	
	//	release texture
    std::map < const iS8* , H3Texture* >::iterator it ;
	for ( it = this->textureMap.begin() ; it!= this->textureMap.end() ; ++it ) {
		it->second->release () ;
	}

	//	release vector 
    std::map < const iS8* , H3Vector3f* >::iterator it2 ;
	for ( it2 = this->vector3fMap.begin() ; it2 != this->vector3fMap.end() ; ++it2 ) {
		MMC_DELETE(it2->second) ;
	}

	//	release color 
    std::map < const iS8* , H3Vector3f* >::iterator it3 ;
	for ( it3 = this->colorMap.begin() ; it3 != this->colorMap.end() ; ++it3 ) {
		MMC_DELETE(it3->second) ;
	}

    return ;

}

/*
 brief : this method can add the same name repeatly
*/
void H3Material::addTexture ( const iS8* name , H3Texture* texture , iS32 index ) 
{

	std::map < const iS8* , H3Texture* >::iterator it ;
	it = this->textureMap.find( name ) ;
	
	//	only add once here
	if ( it == this->textureMap.end() ) 
		texture->addRef () ;

	if ( it != this->textureMap.end() ) {
		//	we not allowed add the same texture twice
		H3ASSERT(it != this->textureMap.end()) ;
		this->textureMap.erase ( name ) ;
	}

	this->textureMap[name] = texture ;
	addTextureIndex ( name , index ) ;

}

/*
 brief : this method can add the same name repeatly
*/
void H3Material::addVector ( const iS8* name , H3Vector3f* vector ) 
{

	bool needDelete = false ;
	H3Vector3f* handle = 0 ;
	std::map < const iS8* , H3Vector3f* >::iterator it ;
	it = this->vector3fMap.find( name ) ;
	if ( it != this->vector3fMap.end() ) {
		handle = it->second ;
		needDelete = true ;
		this->vector3fMap.erase ( name ) ;
	}

	//	we will release it on desconstruct method
	H3Vector3f* nvector = MMC_NEW H3Vector3f( vector->x , vector->y , vector->z ) ;
	this->vector3fMap[name] = nvector ;

	//	const H3Vector3f& color sometimes equal to it->second
	//	so delete it here after 'vector' used
	if ( true == needDelete ) {
		MMC_FREE(handle) ;
	}

}

/*
 brief : this method can add the same name repeatly
*/
void H3Material::addFloat ( const iS8* name , iF32 value ) 
{

	std::map < const iS8* , iF32 >::iterator it ;
	it = this->floatMap.find( name ) ;
	if ( it != this->floatMap.end() )
		this->floatMap.erase ( name ) ;
	
	this->floatMap[name] = value ;

}

/*
 brief : this method can add the same name repeatly
*/
void H3Material::addColor ( const iS8* name , H3Vector3f* color ) 
{
	bool needDelete = false ;
	H3Vector3f* handle = 0 ;
	std::map < const iS8* , H3Vector3f* >::iterator it ;
	it = this->colorMap.find( name ) ;
	if ( it != this->colorMap.end() ) {
		handle = it->second ;
		needDelete = true ;
		this->colorMap.erase ( name ) ;
	}

	//	we will release it on desconstruct method
	H3Vector3f* nvector = MMC_NEW H3Vector3f( color->x , color->y , color->z ) ;
	this->colorMap[name] = nvector ;

	//	const H3Vector3f& color sometimes equal to it->second
	//	so delete it here after 'color' used
	if ( true == needDelete ) {
		MMC_FREE(handle) ;
	}

}

/*
 brief :
*/
H3Texture* H3Material::getTexture ( const iS8* name ) 
{
	H3Texture* texture = 0 ;
	
	std::map < const iS8* , H3Texture* >::iterator it ;
	it = this->textureMap.find( name ) ;
	if ( it != this->textureMap.end() ) {
		texture = this->textureMap[name] ;
	}
	
	if ( NULL == texture ) {
		if ( 0 == strcmp ( name ,"diffuseTexture" ) ) {
			if ( 0 == this->defaultDiffuseTexture ) {
				this->defaultDiffuseTexture = H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/default.png" ) ;
				addTexture ( "diffuseTexture" , this->defaultDiffuseTexture , GL_TEXTURE0 ) ;
			}
			return this->defaultDiffuseTexture ;
		} else if ( 0 == strcmp ( name ,"normalTexture" ) ) {
			if ( 0 == this->defaultNormalTexture ) {
				this->defaultNormalTexture = H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/default_normal.png" ) ;
				addTexture ( "normalTexture" , this->defaultNormalTexture , GL_TEXTURE1 ) ;
			}
			return this->defaultNormalTexture ;
		} else {
			return this->defaultDiffuseTexture ;
		}
	}
	return texture ;
}

/*
 brief :
*/
iS32 H3Material::getTextureIndex ( const iS8* name ) 
{
	iS32 value = 0 ; 
	
	std::map < const iS8* , iS32 >::iterator it ;
	it = this->textureIndexMap.find( name ) ;
	if ( it != this->textureIndexMap.end() ) {
		value = this->textureIndexMap[name] ;
	}
	
	return value ;
}

/*
 brief : return absolute index to shader
*/
iS32 H3Material::getTextureIndexEx ( const iS8* name ) 
{
	iS32 value = 0 ;

	std::map < const iS8* , iS32 >::iterator it ;
	it = this->textureIndexMap.find( name ) ;
	if ( it != this->textureIndexMap.end() ) {
		value = textureIndexMap[name] - GL_TEXTURE0 ;
	}

	return value ;
}

/*
 brief :
*/
H3Vector3f* H3Material::getVector ( const iS8* name ) 
{
	H3Vector3f* vector = 0 ;
	
	std::map < const iS8* , H3Vector3f* >::iterator it ;
	it = this->vector3fMap.find( name ) ;
	if ( it != this->vector3fMap.end() ) {
		vector = vector3fMap[name] ;
	}

	if ( 0 == vector ) 
		return &H3Vector3f ( 1.0f , 1.0f , 1.0f ) ;

	return vector ;
}

/*
 brief :
*/
iF32 H3Material::getFloat ( const iS8* name ) 
{
	iF32 value = 0.0f ;

	std::map < const iS8* , iF32 >::iterator it ;
	it = this->floatMap.find( name ) ;
	if ( it != this->floatMap.end() ) {
		value = floatMap[name] ;
	}

	return value ;
}

/*
 brief :
*/
H3Vector3f* H3Material::getColor ( const iS8* name ) 
{
	H3Vector3f* vector = 0 ;
	
	std::map < const iS8* , H3Vector3f* >::iterator it ;
	it = this->colorMap.find( name ) ;
	if ( it != this->colorMap.end() ) {
		vector = colorMap[name] ;
	}

	if ( 0 == vector ) 
		return &H3Vector3f ( 1.0f , 1.0f , 1.0f ) ;

	return vector ;
}

/* 
 brief : call use inside engine 
*/
void H3Material::addRef () 
{
	this->refCount ++ ;
}

/*
 brief : engine will call this method to release material memorys
*/
void H3Material::release () 
{
	this->refCount -- ;

	if ( 0 == this->refCount ) {
		MMC_DELETE(this) ;
	}
}

/*
 brief :
*/
void H3Material::addTextureIndex ( const iS8* name , iS32 index ) 
{
	this->textureIndexMap[name] = index ;
}
