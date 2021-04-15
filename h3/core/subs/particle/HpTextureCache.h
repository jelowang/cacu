
#ifndef HpTextureCache_H
#define HpTextureCache_H

#include <map> 
#include "HpTexture2D.h"

class HpTextureCache 
{
public :
	/*
	 brief : 
	*/
	HpTextureCache () ;

	/*
	 brief : 
	*/
	~HpTextureCache () ;

	/*
	 brief : 
	*/
	bool init () ;

	/*
	 brief : get singleton handle
	*/
	static HpTextureCache* getInstance () ;

	/*
	 brief : release singleton
	*/
	static void release () ;

	/*
	 brief : 
	*/
	void addTexture ( long long name , HpTexture2D* texture ) ;

	/*
	 brief : 
	*/
	HpTexture2D* getTexture ( long long name ) ;

	/*
	 brief : 
	*/
	void deleteTexture ( long long name ) ;

private :
	std::map < long long , HpTexture2D* > textureMap ;

} ;

#endif