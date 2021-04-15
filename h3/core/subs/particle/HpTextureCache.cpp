
#include "HpTextureCache.h"

static HpTextureCache* gInstance = 0 ;

/*
 brief : 
*/
HpTextureCache::HpTextureCache () 
{
}

/*
 brief : 
*/
HpTextureCache::~HpTextureCache () 
{
}

/*
 brief : 
*/
bool HpTextureCache::init () 
{
	return true ;
}

/*
 brief : 
*/
HpTextureCache* HpTextureCache::getInstance () 
{
    if (!gInstance) {
        gInstance = new HpTextureCache ();
        gInstance->init () ;
    }

    return gInstance;
}

/*
 brief : release singleton
*/
void HpTextureCache::release () 
{
    if (gInstance) {
		MMC_DELETE(gInstance) ;
		gInstance = NULL ;
    }
}

/*
 brief : 
*/
void HpTextureCache::addTexture ( long long name , HpTexture2D* texture ) 
{
	std::map < long long , HpTexture2D* >::iterator it ;
	it = this->textureMap.find( name ) ;
	assert ( it == this->textureMap.end() ) ;

	//this->textureMap[name] = texture ;
	this->textureMap.insert(std::make_pair(name,texture)) ;
}

/*
 brief : 
*/
HpTexture2D* HpTextureCache::getTexture ( long long name ) 
{

	HpTexture2D* texture = 0 ;
	
	std::map < long long , HpTexture2D* >::iterator it ;
	it = this->textureMap.find( name ) ;
	if ( it != this->textureMap.end() ) {
		texture = textureMap[name] ;
	}

	return texture ;

}

/*
 brief : 
*/
void HpTextureCache::deleteTexture ( long long name ) 
{

	if ( 0 != getTexture(name) ) {
		this->textureMap.erase ( name ) ;
	}

}
