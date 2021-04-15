
/*
 +	H3Mesh.cpp
 +	QDL
 
 +	hello3d
 +	realtime 3d engine
 
 
 +	(C) Quantum Dynamics Lab.
        FACEGOOD.
 +
 
*/

#include "H3Mesh.h"
#include "H3Vector.h"

/*
 brief :
*/
H3Mesh::H3Mesh () 
{
	this->refCount = 0 ;
}

/*
brief :
*/
H3Mesh::H3Mesh(std::vector<iF32> vertices , std::vector<iU32> indices , H3Material* material )
{
	this->aabb.setEmpty();
	this->refCount = 0;

	H3ModelGroup* h3ModelGroup = MMC_NEW H3ModelGroup () ;

	for ( iU32 walker = 0 ; walker < vertices.size() ; walker += 3 ) {

		iF32 v0 = vertices[walker] ;
		iF32 v1 = vertices[walker+1] ;
		iF32 v2 = vertices[walker+2] ;

		h3ModelGroup->addPosition ( v0 );
		h3ModelGroup->addPosition ( v1 );
		h3ModelGroup->addPosition ( v2 );
								 	  
	}

	for (iU32 walker = 0; walker < indices.size() ; walker += 3) {

		iF32 v0 = indices[walker] ;
		iF32 v1 = indices[walker + 1] ;
		iF32 v2 = indices[walker + 2] ;

		h3ModelGroup->addVertexIndices ( v0 );
		h3ModelGroup->addVertexIndices ( v1 );
		h3ModelGroup->addVertexIndices ( v2 );

	}

	//
	//	caculate normal if it is necessary
	//	needs normal effects
	//	what ever this->needNormal is need , we always caculate it here
	//	always caculate normals
	bool needCaculateNormal = false;

	//	generate group
	{

		H3ModelGroup* modelGroup = h3ModelGroup;
		H3MeshGroup* meshGroup = MMC_NEW H3MeshGroup();
		meshGroup->id = modelGroup->id;

		meshGroup->vertex.normal_data = (iF32*)MMC_MALLOC(3 * sizeof(iF32)*modelGroup->vertexIndices.size());
		if (NULL == meshGroup->vertex.normal_data){
			iLog("FE : H3Mesh::initWithModel Failed , Not Enough Memory for Vbo Normal");
			goto ReleaseMem;
		}
		memset(meshGroup->vertex.normal_data, 0x0, 3 * sizeof(iF32)*modelGroup->vertexIndices.size());

		//	always has normal , if not caculate automatically
		//	ignores normals from obj , we will caculate ourself
		modelGroup->setHasNormal(true);

		meshGroup->vertex.tangent_data = (iF32*)MMC_MALLOC(3 * sizeof(iF32)*modelGroup->vertexIndices.size());
		if (NULL == meshGroup->vertex.tangent_data){
			if (0 != meshGroup->vertex.normal_data)
				MMC_FREE(meshGroup->vertex.normal_data);
			iLog("not Enough Memory for tangent");
			goto ReleaseMem;
		}
		memset(meshGroup->vertex.tangent_data, 0x0, 3 * sizeof(iF32)*modelGroup->vertexIndices.size());

		//	faces construction
		bool cface = constructFaces(meshGroup, modelGroup);
		if (false == cface){
			if (0 != meshGroup->vertex.normal_data)
				MMC_FREE(meshGroup->vertex.normal_data);
			if (0 != meshGroup->vertex.tangent_data)
				MMC_FREE(meshGroup->vertex.tangent_data);
			goto ReleaseMem;
		}

		//	greate new material
		if ( nullptr != material ){

			H3Vector3f* ambientColor = material->getColor ( H3_MATERIAL_COLOR_AMBIENT );
			H3Vector3f* diffuseColor = material->getColor ( H3_MATERIAL_COLOR_DIFFUSE );
			H3Vector3f* specularColor = material->getColor ( H3_MATERIAL_COLOR_SPECULAR );

			meshGroup->material = MMC_NEW H3Material ( );
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_AMBIENT, ambientColor );
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_DIFFUSE, diffuseColor );
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_SPECULAR, specularColor );

			meshGroup->material->addFloat ( H3_MATERIAL_SPECULARINTENSITY, material->getFloat ( H3_MATERIAL_SPECULARINTENSITY ) );
			meshGroup->material->addFloat ( H3_MATERIAL_SHININESS, material->getFloat ( H3_MATERIAL_SHININESS ) );
			meshGroup->material->addFloat ( H3_MATERIAL_SSS, material->getFloat ( H3_MATERIAL_SSS ) );
			meshGroup->material->addFloat ( H3_MATERIAL_FRESNEL, material->getFloat ( H3_MATERIAL_FRESNEL ) );
			meshGroup->material->addTexture ( H3_MATERIAL_TEXTURE_DIFFUSE, material->getTexture ( H3_MATERIAL_TEXTURE_DIFFUSE ), material->getTextureIndex ( H3_MATERIAL_TEXTURE_DIFFUSE ) );
			meshGroup->material->addTexture ( H3_MATERIAL_TEXTURE_NORMAL, material->getTexture ( H3_MATERIAL_TEXTURE_NORMAL ), material->getTextureIndex ( H3_MATERIAL_TEXTURE_NORMAL ) );
			meshGroup->material->addRef ( );

		}

		meshGroup->generateQuad();
		meshGroup->releaseVertex();
		this->meshGroupList.push_back(meshGroup);

	}

	return ;

ReleaseMem:
	//	todo
	return ;


}

/*
 brief :
*/
H3Mesh::H3Mesh( const iS8* file , const iS8* mtl , bool fromFile )
{

	this->aabb.setEmpty () ;
	this->refCount = 0 ;
	init ( file , mtl , fromFile , 0 ) ;

} 

/*
 brief :
*/
H3Mesh::H3Mesh( const iS8* file , H3Material* material ) 
{
	this->aabb.setEmpty () ;
	this->refCount = 0 ;
	init ( file , 0 , true , material ) ;
}


/*
 brief :
*/
H3Mesh::~H3Mesh() 
{
	for ( iU32 looper = 0 ; looper < this->meshGroupList.size() ; looper ++ ) {
		H3MeshGroup* group = this->meshGroupList[looper] ;
		MMC_DELETE(group) ;
	}
}

/* 
 brief : call use inside engine 
*/
void H3Mesh::addRef () 
{
	this->refCount ++ ;
}

/*
 brief : engine will call this method to release material memorys
*/
void H3Mesh::release () 
{
	this->refCount -- ;

	if ( 0 == this->refCount ) {
		MMC_DELETE(this) ;
	}
}

/*
 brief : get mesh group
*/
H3MeshGroup* H3Mesh::getMeshGroup ( iU32 id ) 
{
	H3ASSERT( id < this->meshGroupList.size() ) ;
	return this->meshGroupList[id] ;
}

/*
 brief :
*/
void H3Mesh::init( const iS8* file , const iS8* mtl , bool fromFile , H3Material* material )
{

	H3Model* model = MMC_NEW H3ModelObj( file , mtl , fromFile ) ;	
	H3ASSERT(NULL != model) ;

	initWithModel ( model , material ) ;
	MMC_DELETE(model) ;
}

/*
 brief :
*/
bool H3Mesh::initWithModel ( H3Model* model , H3Material* material ) 
{

	//	caculate normal if it is necessary
	//	needs normal effects
	//	what ever this->needNormal is need , we always caculate it here
	//	always caculate normals
	bool needCaculateNormal = true ;

	//	generate group
	for ( iU32 looper = 0 ; looper < model->modelGroup.size() ; looper ++ ) {

		H3ModelGroup* modelGroup = model->modelGroup[looper] ;
		H3MeshGroup* meshGroup = MMC_NEW H3MeshGroup () ;
		meshGroup->id = modelGroup->id ;

		meshGroup->vertex.normal_data = (iF32* )MMC_MALLOC( 3*sizeof(iF32)*modelGroup->vertexIndices.size() ) ;
		if ( NULL == meshGroup->vertex.normal_data ){		
			iLog( "FE : H3Mesh::initWithModel Failed , Not Enough Memory for Vbo Normal" ) ;
			goto ReleaseMem ;
		}
		memset ( meshGroup->vertex.normal_data , 0x0 , 3*sizeof(iF32)*modelGroup->vertexIndices.size() ) ;

		//	always has normal , if not caculate automatically
		//	ignores normals from obj , we will caculate ourself
		modelGroup->setHasNormal ( true ) ;

		meshGroup->vertex.tangent_data = (iF32* )MMC_MALLOC( 3*sizeof(iF32)*modelGroup->vertexIndices.size() ) ;
		if ( NULL == meshGroup->vertex.tangent_data ){		
			if ( 0 != meshGroup->vertex.normal_data ) 
				MMC_FREE(meshGroup->vertex.normal_data) ;	
			iLog( "not Enough Memory for tangent" ) ;
			goto ReleaseMem ;
		}
		memset ( meshGroup->vertex.tangent_data , 0x0 , 3*sizeof(iF32)*modelGroup->vertexIndices.size() ) ;

		//	faces construction
		bool cface = constructFaces ( meshGroup , modelGroup ) ;
		if ( false == cface ){		
			if ( 0 != meshGroup->vertex.normal_data ) 
				MMC_FREE(meshGroup->vertex.normal_data) ;	
			if ( 0 != meshGroup->vertex.tangent_data ) 
				MMC_FREE(meshGroup->vertex.tangent_data) ;	
			goto ReleaseMem ;
		}

		//	greate new material
		if ( modelGroup->material ){
			H3Vector3f* ambientColor = modelGroup->material->getColor ( H3_MATERIAL_COLOR_AMBIENT ) ;
			H3Vector3f* diffuseColor = modelGroup->material->getColor ( H3_MATERIAL_COLOR_DIFFUSE ) ;
			H3Vector3f* specularColor = modelGroup->material->getColor ( H3_MATERIAL_COLOR_SPECULAR ) ;

			meshGroup->material = MMC_NEW H3Material () ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_AMBIENT , ambientColor ) ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_DIFFUSE , diffuseColor ) ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_SPECULAR , specularColor ) ;

			meshGroup->material->addFloat ( H3_MATERIAL_SPECULARINTENSITY , modelGroup->material->getFloat(H3_MATERIAL_SPECULARINTENSITY) ) ;
			meshGroup->material->addFloat ( H3_MATERIAL_SHININESS , modelGroup->material->getFloat(H3_MATERIAL_SHININESS) ) ;
			meshGroup->material->addFloat ( H3_MATERIAL_SSS , modelGroup->material->getFloat(H3_MATERIAL_SSS) ) ;
			meshGroup->material->addFloat ( H3_MATERIAL_FRESNEL , modelGroup->material->getFloat(H3_MATERIAL_FRESNEL) ) ;

			meshGroup->material->addTexture ( H3_MATERIAL_TEXTURE_DIFFUSE , modelGroup->material->getTexture ( H3_MATERIAL_TEXTURE_DIFFUSE ) , modelGroup->material->getTextureIndex ( H3_MATERIAL_TEXTURE_DIFFUSE ) );
			meshGroup->material->addTexture ( H3_MATERIAL_TEXTURE_NORMAL , modelGroup->material->getTexture ( H3_MATERIAL_TEXTURE_NORMAL ) , modelGroup->material->getTextureIndex ( H3_MATERIAL_TEXTURE_NORMAL ) );

			meshGroup->material->addRef () ;

		} else if ( NULL != material ) {
			H3Vector3f* ambientColor = material->getColor ( H3_MATERIAL_COLOR_AMBIENT ) ;
			H3Vector3f* diffuseColor = material->getColor ( H3_MATERIAL_COLOR_DIFFUSE ) ;
			H3Vector3f* specularColor = material->getColor ( H3_MATERIAL_COLOR_SPECULAR ) ;

			meshGroup->material = MMC_NEW H3Material () ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_AMBIENT , ambientColor ) ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_DIFFUSE , diffuseColor ) ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_SPECULAR , specularColor ) ;

			meshGroup->material->addFloat ( H3_MATERIAL_SPECULARINTENSITY , material->getFloat(H3_MATERIAL_SPECULARINTENSITY) ) ;
			meshGroup->material->addFloat ( H3_MATERIAL_SHININESS , material->getFloat(H3_MATERIAL_SHININESS) ) ;
			meshGroup->material->addFloat ( H3_MATERIAL_SSS , material->getFloat(H3_MATERIAL_SSS) ) ;
			meshGroup->material->addFloat ( H3_MATERIAL_FRESNEL , material->getFloat(H3_MATERIAL_FRESNEL) ) ;
			meshGroup->material->addTexture ( H3_MATERIAL_TEXTURE_DIFFUSE , material->getTexture ( H3_MATERIAL_TEXTURE_DIFFUSE ) , material->getTextureIndex ( H3_MATERIAL_TEXTURE_DIFFUSE ) );
			meshGroup->material->addTexture ( H3_MATERIAL_TEXTURE_NORMAL , material->getTexture ( H3_MATERIAL_TEXTURE_NORMAL ) , material->getTextureIndex ( H3_MATERIAL_TEXTURE_NORMAL ) );
			meshGroup->material->addRef () ;
		}

		meshGroup->generateQuad () ;
		meshGroup->releaseVertex() ;
		this->meshGroupList.push_back ( meshGroup ) ;

	}

	return true ;

ReleaseMem :
	//	todo
	return false ;

}

//
/*
 brief : construct faces base on indices
*/
bool H3Mesh::constructFaces( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup ) 
{

	meshGroup->face.data = (iU32* )MMC_MALLOC ( sizeof(iU32)*modelGroup->vertexIndices.size() ) ;

	if ( NULL == meshGroup->face.data ){
		iLog( "Not Enough Memory for Vbo faces" ) ;
		return false ;
	}
	meshGroup->face.length = 0 ;

	//	texCoords
    meshGroup->vertex.texCoord_data = (iF32* )MMC_MALLOC( 2 * sizeof(iF32)*modelGroup->vertexIndices.size() ) ;
	meshGroup->vertex.texCoord_walker = 0 ;

	if ( NULL == meshGroup->vertex.texCoord_data ){	
		if ( 0 != meshGroup->face.data )
			MMC_FREE(meshGroup->face.data) ;
		iLog( "not enough memory for texCoord" ) ;
		return false ;
	}

	//	positions	
	meshGroup->vertex.position_data = (iF32* )MMC_MALLOC( 3 * sizeof(iF32)*modelGroup->vertexIndices.size() ) ;
	if ( NULL == meshGroup->vertex.position_data ){		
		if ( 0 != meshGroup->face.data )
			MMC_FREE(meshGroup->face.data) ;
		if ( 0 != meshGroup->vertex.texCoord_data )
			MMC_FREE(meshGroup->vertex.texCoord_data) ;
		iLog( "FE : H3Mesh::initWithModel Failed , Not Enough Memory for Vbo position Data" ) ;
		return false ;
	}

	caculateNormals ( meshGroup , modelGroup ) ;
	caculateTagents ( meshGroup , modelGroup ) ;

	for ( iS32 looper = 0 ; looper < (iS32 )modelGroup->vertexIndices.size() ; looper ++ ){

		//	save indices
		saveVertex ( meshGroup , modelGroup , looper ) ;
		saveTexCoord ( meshGroup , modelGroup , looper ) ;
		saveNormal ( meshGroup , modelGroup , looper ) ;
		saveTangent ( meshGroup , modelGroup , looper ) ;

		meshGroup->face.data[meshGroup->face.length] = looper ;
		meshGroup->face.length = meshGroup->face.length + 1 ;

	} 

	return true ;

}

/*
 brief :
*/
void H3Mesh::saveVertex ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) 
{

	//	position
	H3Vector3f v ( 0.0f , 0.0f , 0.0f ) ;

	iU32 indexFormFile = modelGroup->vertexIndices[index] ;

	v.init ( modelGroup->positionArray[indexFormFile*3] , modelGroup->positionArray[indexFormFile*3+1] , modelGroup->positionArray[indexFormFile*3+2] ) ;

	//	aabb for the whole mesh
	this->aabb.add ( v ) ;
	//	aabb for per group
	meshGroup->aabb.add ( v ) ;

	meshGroup->vertex.position_data[meshGroup->vertex.position_walker] = v.x ;
	meshGroup->vertex.position_data[meshGroup->vertex.position_walker+1] = v.y ;
	meshGroup->vertex.position_data[meshGroup->vertex.position_walker+2] = v.z ;
	meshGroup->vertex.position_walker = meshGroup->vertex.position_walker + 3 ;

}

/*
 brief :
*/
void H3Mesh::saveTexCoord ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) 
{
	//	texCoord
	H3Vector2f v ( 0.0f , 0.0f ) ;
	iU32 indexFormFile = 0 ;

	if ( 0 < modelGroup->texCoordIndices.size() ) 
		indexFormFile = modelGroup->texCoordIndices[index] ;

	if ( 0 < modelGroup->texCoordArray.size() )
		v.init ( modelGroup->texCoordArray[indexFormFile*2] , modelGroup->texCoordArray[indexFormFile*2+1] ) ;

	meshGroup->vertex.texCoord_data[meshGroup->vertex.texCoord_walker] = v.x ;
	meshGroup->vertex.texCoord_data[meshGroup->vertex.texCoord_walker+1] = v.y ;
	meshGroup->vertex.texCoord_walker = meshGroup->vertex.texCoord_walker + 2 ;
}

/*
 brief :
*/
void H3Mesh::saveNormal ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) 
{
	//	position
	H3Vector3f v ( 0.0f , 0.0f , 0.0f ) ;
	iU32 indexFormFile = 0 ;

	if ( 0 < modelGroup->normalIndices.size() ) 
		indexFormFile = modelGroup->normalIndices[index] ;
	else if ( 0 < modelGroup->vertexIndices.size() ) 
		indexFormFile = modelGroup->vertexIndices[index] ;

	if ( 0 < modelGroup->normalArray.size() )
		v.init ( modelGroup->normalArray[indexFormFile*3] , modelGroup->normalArray[indexFormFile*3+1] , modelGroup->normalArray[indexFormFile*3+2] ) ;

	meshGroup->vertex.normal_data[meshGroup->vertex.normal_walker] = v.x ;
	meshGroup->vertex.normal_data[meshGroup->vertex.normal_walker+1] = v.y ;
	meshGroup->vertex.normal_data[meshGroup->vertex.normal_walker+2] = v.z ;
	meshGroup->vertex.normal_walker = meshGroup->vertex.normal_walker + 3 ;
}

/*
 brief :
*/
void H3Mesh::saveTangent ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup , iS32 index ) 
{
	//	position
	H3Vector3f v ( 0.0f , 0.0f , 0.0f ) ;
	iU32 indexFormFile = 0 ;

	if ( 0 == modelGroup->tangentArray.size() ) 
		return ;

	if ( 0 < modelGroup->vertexIndices.size() ) 
		indexFormFile = modelGroup->vertexIndices[index] ;

	if ( 0 < modelGroup->tangentArray.size() )
		v.init ( modelGroup->tangentArray[indexFormFile*3] , modelGroup->tangentArray[indexFormFile*3+1] , modelGroup->tangentArray[indexFormFile*3+2] ) ;

	meshGroup->vertex.tangent_data[meshGroup->vertex.tangent_walker] = v.x ;
	meshGroup->vertex.tangent_data[meshGroup->vertex.tangent_walker+1] = v.y ;
	meshGroup->vertex.tangent_data[meshGroup->vertex.tangent_walker+2] = v.z ;
	meshGroup->vertex.tangent_walker = meshGroup->vertex.tangent_walker + 3 ;
}


/*
 brief : caculate normals
*/
bool H3Mesh::caculateNormals ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup ) 
{

	//	have orignal normals from file
	if ( 0 < modelGroup->normalIndices.size () ) {
		return true ;
	}

	//	record index informations which is idedicating the current index is saved or not
	iS32* normalSetled = (iS32* ) MMC_MALLOC ( sizeof(iU32)*modelGroup->vertexIndices.size() ) ;

	if ( NULL == normalSetled ) {		
		iLog( "FE : H3Mesh::initWithModel Failed , Not Enough Memory for Vbo Data 2" ) ;
		return false ;
	}

	for ( iS32 looper = 0 ; looper < (iS32 )modelGroup->vertexIndices.size() ; looper += 3 ) {

		iS32 i1 = modelGroup->vertexIndices[looper] ; 
		iS32 i2 = modelGroup->vertexIndices[looper+1] ; 
		iS32 i3 = modelGroup->vertexIndices[looper+2] ; 

		//	caculate normals
		{
			H3Vector3f v0 ;
			H3Vector3f v1 ;
			H3Vector3f v2 ;
			H3Vector3f v3 ;
			H3Vector3f v4 ;

			v0.init ( modelGroup->positionArray[i1*3] , modelGroup->positionArray[i1*3+1] , modelGroup->positionArray[i1*3+2] ) ;			
			v1.init ( modelGroup->positionArray[i2*3] , modelGroup->positionArray[i2*3+1] , modelGroup->positionArray[i2*3+2] ) ;
			v2.init ( modelGroup->positionArray[i3*3] , modelGroup->positionArray[i3*3+1] , modelGroup->positionArray[i3*3+2] ) ;

			v3 = v1.sub ( v0 ) ;
			v4 = v2.sub ( v0 ) ;

			//	normalize it with shader
			H3Vector3f normal = v3.cross(v4) ;

			if ( 0 == normalSetled[i1] ) {
				meshGroup->vertex.normal_data[i1*3] = normal.x ;
				meshGroup->vertex.normal_data[i1*3+1] = normal.y ;
				meshGroup->vertex.normal_data[i1*3+2] = normal.z ; 
				meshGroup->vertex.normal_walker = meshGroup->vertex.normal_walker + 3 ;
				normalSetled[i1] = 1 ;			
			}

			if ( 0 == normalSetled[i2] ) {
				meshGroup->vertex.normal_data[i2*3] = normal.x ;
				meshGroup->vertex.normal_data[i2*3+1] = normal.y ;
				meshGroup->vertex.normal_data[i2*3+2] = normal.z ; 
				meshGroup->vertex.normal_walker = meshGroup->vertex.normal_walker + 3 ;
				normalSetled[i2] = 1 ;				
			}

			if ( 0 == normalSetled[i3] ) {
				meshGroup->vertex.normal_data[i3*3] = normal.x ;
				meshGroup->vertex.normal_data[i3*3+1] = normal.y ;
				meshGroup->vertex.normal_data[i3*3+2] = normal.z ; 
				meshGroup->vertex.normal_walker = meshGroup->vertex.normal_walker + 3 ;
				normalSetled[i3] = 1 ;
			}
			
			meshGroup->vertex.normal_data[i1*3] = meshGroup->vertex.normal_data[i1*3] + normal.x ;
			meshGroup->vertex.normal_data[i1*3+1] = meshGroup->vertex.normal_data[i1*3+1] + normal.y ;
			meshGroup->vertex.normal_data[i1*3+2] = meshGroup->vertex.normal_data[i1*3+2] +  normal.z ; 
			
			meshGroup->vertex.normal_data[i2*3] = meshGroup->vertex.normal_data[i2*3] + normal.x ;
			meshGroup->vertex.normal_data[i2*3+1] = meshGroup->vertex.normal_data[i2*3+1] + normal.y ;
			meshGroup->vertex.normal_data[i2*3+2] = meshGroup->vertex.normal_data[i2*3+2] +  normal.z ;

			meshGroup->vertex.normal_data[i3*3] = meshGroup->vertex.normal_data[i3*3] + normal.x ;
			meshGroup->vertex.normal_data[i3*3+1] = meshGroup->vertex.normal_data[i3*3+1] + normal.y ;
			meshGroup->vertex.normal_data[i3*3+2] = meshGroup->vertex.normal_data[i3*3+2] +  normal.z ;
		
		}

	}
	MMC_FREE(normalSetled) ;
	modelGroup->normalArray.clear () ;

	for ( int counter = 0 ; counter < (iS32 )meshGroup->vertex.normal_walker ; counter += 3 ) {
		modelGroup->normalArray.push_back ( meshGroup->vertex.normal_data[counter] ) ;
		modelGroup->normalArray.push_back ( meshGroup->vertex.normal_data[counter+1] ) ;
		modelGroup->normalArray.push_back ( meshGroup->vertex.normal_data[counter+2] ) ;
	}
	meshGroup->vertex.normal_walker = 0 ;
	 
	return true ;


}

/*
 brief :
*/
bool H3Mesh::caculateTagents ( H3MeshGroup* meshGroup , H3ModelGroup* modelGroup ) 
{

	if (0 ==  modelGroup->texCoordArray.size () ) {

		//	always set tangent data , because we need this to sovle tbnMat at shader
		for ( int counter = 0 ; counter < (iS32 )modelGroup->positionArray.size () ; counter += 3 ) {
			modelGroup->tangentArray.push_back ( 1.0f ) ;
			modelGroup->tangentArray.push_back ( 1.0f ) ;
			modelGroup->tangentArray.push_back ( 1.0f ) ;
		}
		meshGroup->vertex.tangent_walker = 0 ;

		return false ;
	}

	//	record index informations which is idedicating the current index is saved or not
	iS32* tangentSetled = (iS32* ) MMC_MALLOC ( sizeof(iU32)*modelGroup->vertexIndices.size() ) ;

	if ( NULL == tangentSetled ) {		
		iLog( "FE : H3Mesh::initWithModel Failed , Not Enough Memory for Vbo Data 2" ) ;
		return false ;
	}

	for ( iS32 looper = 0 ; looper < (iS32 )modelGroup->vertexIndices.size() ; looper += 3 ) {

		iS32 i1 = modelGroup->vertexIndices[looper] ; 
		iS32 i2 = modelGroup->vertexIndices[looper+1] ; 
		iS32 i3 = modelGroup->vertexIndices[looper+2] ; 

		//	caculate tangent
		{
			H3Vector3f v0 ;
			H3Vector3f v1 ;
			H3Vector3f v2 ;
			H3Vector3f e1 ;
			H3Vector3f e2 ;

			iF32 deltaU1 = 0 ;			
			iF32 deltaV1 = 0 ;
			iF32 deltaU2 = 0 ;
			iF32 deltaV2 = 0 ;

			iF32 dofuv = 1.0f / ( deltaU1*deltaV2 - deltaU2*deltaV1 ) ;

			//	U1
			deltaU1 = modelGroup->texCoordArray[i2*2] - modelGroup->texCoordArray[i1*2] ;
			//	V1
			deltaV1 = modelGroup->texCoordArray[i2*2+1] - modelGroup->texCoordArray[i1*2+1] ;

			//	U2
			deltaU2 = modelGroup->texCoordArray[i3*2] - modelGroup->texCoordArray[i1*2] ;
			//	V2
			deltaV2 = modelGroup->texCoordArray[i3*2+1] - modelGroup->texCoordArray[i1*2+1] ;

			v0.init ( modelGroup->positionArray[i1*3] , modelGroup->positionArray[i1*3+1] , modelGroup->positionArray[i1*3+2] ) ;			
			v1.init ( modelGroup->positionArray[i2*3] , modelGroup->positionArray[i2*3+1] , modelGroup->positionArray[i2*3+2] ) ;
			v2.init ( modelGroup->positionArray[i3*3] , modelGroup->positionArray[i3*3+1] , modelGroup->positionArray[i3*3+2] ) ;

			e1 = v1.sub ( v0 ) ;
			e2 = v2.sub ( v0 ) ;
			
			H3Vector3f tangent ;

			tangent.x = ( e1.x*deltaV2 - e2.x*deltaV1 ) ;
			tangent.y = ( e1.y*deltaV2 - e2.y*deltaV1 ) ;
			tangent.z = ( e1.z*deltaV2 - e2.z*deltaV1 ) ;

			if ( 0 == tangentSetled[i1] ) {
				meshGroup->vertex.tangent_data[i1*3] = tangent.x ;
				meshGroup->vertex.tangent_data[i1*3+1] = tangent.y ;
				meshGroup->vertex.tangent_data[i1*3+2] = tangent.z ; 
				meshGroup->vertex.tangent_walker = meshGroup->vertex.tangent_walker + 3 ;
				tangentSetled[i1] = 1 ;			
			}

			if ( 0 == tangentSetled[i2] ) {
				meshGroup->vertex.tangent_data[i2*3] = tangent.x ;
				meshGroup->vertex.tangent_data[i2*3+1] = tangent.y ;
				meshGroup->vertex.tangent_data[i2*3+2] = tangent.z ; 
				meshGroup->vertex.tangent_walker = meshGroup->vertex.tangent_walker + 3 ;
				tangentSetled[i2] = 1 ;				
			}

			if ( 0 == tangentSetled[i3] ) {
				meshGroup->vertex.tangent_data[i3*3] = tangent.x ;
				meshGroup->vertex.tangent_data[i3*3+1] = tangent.y ;
				meshGroup->vertex.tangent_data[i3*3+2] = tangent.z ; 
				meshGroup->vertex.tangent_walker = meshGroup->vertex.tangent_walker + 3 ;
				tangentSetled[i3] = 1 ;
			}

			meshGroup->vertex.tangent_data[i1*3] = meshGroup->vertex.tangent_data[i1*3] + tangent.x ;
			meshGroup->vertex.tangent_data[i1*3+1] = meshGroup->vertex.tangent_data[i1*3+1] + tangent.y ;
			meshGroup->vertex.tangent_data[i1*3+2] = meshGroup->vertex.tangent_data[i1*3+2] +  tangent.z ; 
			
			meshGroup->vertex.tangent_data[i2*3] = meshGroup->vertex.tangent_data[i2*3] + tangent.x ;
			meshGroup->vertex.tangent_data[i2*3+1] = meshGroup->vertex.tangent_data[i2*3+1] + tangent.y ;
			meshGroup->vertex.tangent_data[i2*3+2] = meshGroup->vertex.tangent_data[i2*3+2] +  tangent.z ;

			meshGroup->vertex.tangent_data[i3*3] = meshGroup->vertex.tangent_data[i3*3] + tangent.x ;
			meshGroup->vertex.tangent_data[i3*3+1] = meshGroup->vertex.tangent_data[i3*3+1] + tangent.y ;
			meshGroup->vertex.tangent_data[i3*3+2] = meshGroup->vertex.tangent_data[i3*3+2] +  tangent.z ;
		
		}

	}
	MMC_FREE(tangentSetled) ;
	modelGroup->tangentArray.clear () ;

	for ( int counter = 0 ; counter < (iS32 )meshGroup->vertex.tangent_walker ; counter += 3 ) {
		modelGroup->tangentArray.push_back ( meshGroup->vertex.tangent_data[counter] ) ;
		modelGroup->tangentArray.push_back ( meshGroup->vertex.tangent_data[counter+1] ) ;
		modelGroup->tangentArray.push_back ( meshGroup->vertex.tangent_data[counter+2] ) ;
	}
	meshGroup->vertex.tangent_walker = 0 ;

	return true ;

}