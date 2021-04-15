
#include "H3Object.h"
#include "H3Billboard.h"
#include "StudioWindow.h"
#include "H3MeshRender.h"
#include "H3Raster.h"
#include "H3Image.h"
#include "H3Delegate.h"
#include "editor.h"
#include "H3DrawPrimitives.h"
#include "H3Font.h"
#include "H3FacialRender.h"
#include "H3NavMesh.h"

iS32 gObjectIndex = 0 ;
static H3Vector3f gOrignalMeshColor ;

static H3NavMesh navMesh ;
static float* m_smoothPath = 0 ;
static int m_nsmoothPath = 0;

static void drawMap ()
{
	
	//	draw row
	iF32 r = 0.0f ;
	iS32 row = 0 ;
	for ( row = 0 ; row < 50 ; row ++ ) {
		r = (iF32)row / 50.0f * 100.0f ;		
		if ( 0 == r-50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f , 0.0f , 1.0f , 1.0f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;
		} else {
			H3DrawPrimitives::setColor ( 0.4f , 0.4f , 0.4f , 0.5f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;
		}
	}
	r = (iF32)row / 50.0f * 100.0f ;		
	H3DrawPrimitives::drawLine ( H3Vector3f(-50.0f,0.0f,r-50.0f) , H3Vector3f(50.0f,0.0f,r-50.0f) ) ;

	//draw col
	iS32 col = 0 ;
	for ( col = 0 ; col < 50 ; col ++ ) {
		r = (iF32)col / 50.0f * 100.0f ;
		if ( 0 == r-50.0f ) {
			H3DrawPrimitives::setColor ( 0.0f , 0.0f , 1.0f , 1.0f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;
		} else {
			H3DrawPrimitives::setColor ( 0.4f , 0.4f , 0.4f , 0.5f ) ;
			H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;
		}
	}
	r = (iF32)col / 50.0f * 100.0f ;
	H3DrawPrimitives::drawLine ( H3Vector3f(r-50.0f,0.0f,-50.0f) , H3Vector3f(r-50.0f,0.0f,50.0f) ) ;

	

}

StudioWindow::StudioWindow() 
{
	h3AmbientLight = 0 ;

	this->width = 1920 ;
	this->height = 1080 ;
	 
	h3AmbientLight = MMC_NEW H3AmbientLight( H3Vector3f( 0.5f , 0.5f , 0.5f ) ) ;

	this->addComponent( h3AmbientLight ) ; 

	this->camera = MMC_NEW H3Camera() ;
	this->camera->setPosition( H3Vector3f( 0 , 0 , 0 ) ) ;
	this->addComponent( camera ) ;

	this->inited = false ;

	this->currentObject = 0 ;
	this->intersectPoint = H3Vector3f( 0.0f , 0.0f , 0.0f ) ;
	this->currentSelectedMeshId = -1 ;
	this->currentSelectedObject = 0 ;
	
	this->show_details_windows = false ;
	this->gDetailsType = 0 ;
	this->gDetailsRef = 0 ;
	this->camerType = StudioCameraType_Perspective ;
	this->lastCameraType = 0 ;

	SCClListInit( &this->objectList ) ;	
	SCClListInit( &this->lightList ) ;
	SCClListInit( &this->objectInfoList ) ;
	SCClListInit( &this->billboardList );
	SCClListInit( &this->meshList ) ;

	H3LightAtom* atom1 = ( H3LightAtom* ) H3MemoryMalloc( sizeof( H3LightAtom ) , __FILE__ , __LINE__ ) ;
	H3ASSERT( NULL != atom1 ) ;
	atom1->type = H3COMPONENT_TYPE_AMBIENT_LIGHT ;
	atom1->lightRef = ( void* ) this->h3AmbientLight ;
	SCClListInsert( &this->lightList , atom1 ) ;

	H3Font::init("fonts/msyh.ttf") ;

	this->fontTexture = (H3Texture*)H3Font::Texture() ;

	this->fontData = H3Font::Data() ;

	this->fontWidth = H3Font::TextureWidth() ;
	this->fontHeight = H3Font::TextureHeight() ;

	//test 20160411
	//H3Image* logoImage = createLogoImage("textures\\ball.png") ;
	//iS8 texttest [256] = {0} ;
	//strcpy ( texttest , H3UtilGbk2Utf8("°×ÄãÅ¶°¡µÄ·ç¾°ÅÄÅ¶Êø¸¿Æ¨°¡U¶ÜÊæ·þ").data() ) ;
	//H3Image* wordsImage = createTextImage( texttest ) ;
	//H3Image* resultsImage = creatBillboardImage ( logoImage , wordsImage ) ;
	//
	//H3Texture* fontTexture = H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , resultsImage->getWidth() , resultsImage->getHeight() , resultsImage->getData() ,resultsImage->getType()) ;
	//
	//H3Billboard* billboard = MMC_NEW H3Billboard ( 2.0f ) ;
	//billboard->getMaterial()->addTexture ( "diffuseTexture", fontTexture , GL_TEXTURE0 ) ;
	//billboard->getMaterial()->addColor ( H3_MATERIAL_COLOR_AMBIENT, &H3Vector3f(1.0,1.0,1.0) ) ;
	//billboard->getMaterial()->addFloat ( H3_MATERIAL_SPECULARINTENSITY , 30.0f ) ;
	//billboard->getMaterial()->addFloat ( H3_MATERIAL_SHININESS , 3.0f ) ;
	//billboard->setPosition (H3Vector3f(0.0,3.0,0.0)) ;
	//billboard->setScale ( H3Vector3f(1.0,(float )resultsImage->getHeight()/(float )resultsImage->getWidth(),1.0) ) ;
	//addChild ( billboard ) ;
	//
	//MMC_DELETE( logoImage ) ;
	//MMC_DELETE( wordsImage ) ;
	//MMC_DELETE( resultsImage ) ;

	this->blendShapeWeight = 0.0f ;


	navMesh.load ( "models/all_tiles_tilecache.bin" ) ;


}

StudioWindow::~StudioWindow() 
{
	MMC_DELETE( this->texture ) ;
}

bool StudioWindow::init ( iU32 width , iU32 height ) 
{
	//	check if multi-init
	H3ASSERT ( false == this->inited ) ;

	this->width = width ;
	this->height = height ;

	return true ;
} 

/*
 brief :
*/
#ifdef FACIAL_TEST
H3FacialRender* H3FacialRenderDeformer0 = 0 ;
H3FacialRender* H3FacialRenderDeformer1 = 0 ;
#endif
void StudioWindow::update( iF32 deltaT ) 
{
#ifdef FACIAL_TEST
	if ( 0 == H3FacialRenderDeformer0 ) return ;
#endif
#if 0
	if ( 0 == currentObject ) 
		return ;

	static int times = 0 ;

	times ++ ;

	if ( 0 == times % 2 ) {

		iU32 meshGroupId = rand()%255 ;
		H3Mesh* mesh = this->currentObject->getMesh () ;
		H3ActionHeadMorphing* action = H3ActionHeadMorphing::create (
			this->currentObject , mesh->meshGroupList[meshGroupId] , 0.1 , 0 
		) ;
		this->currentObject->startAction ( H3ActionInterval::create(action,1.0) ) ;

	}
#endif

#if 0
	for ( iU32 walker = 0 ; walker < H3FacialRenderDeformer0->getMesh()->meshGroupList.size () ; walker ++ ) {
		H3MeshGroup* meshGroup0 = H3FacialRenderDeformer0->getMesh()->meshGroupList[walker] ;
		H3MeshGroup* meshGroup1 = H3FacialRenderDeformer1->getMesh()->meshGroupList[walker] ;
		for ( iU32 looper = 0 ; looper < meshGroup0->totallQuad ; looper ++ ) {
			H3Vector3f pos0 ;
			pos0.x = meshGroup0->quad[looper].vertex.x ;
			pos0.y = meshGroup0->quad[looper].vertex.y ;
			pos0.z = meshGroup0->quad[looper].vertex.z ;
	
			H3Vector3f pos1 ;
			pos1.x = meshGroup1->quad[looper].vertex.x ;
			pos1.y = meshGroup1->quad[looper].vertex.y ;
			pos1.z = meshGroup1->quad[looper].vertex.z ;
	
			H3Vector3f pos3 = pos0*blendShapeWeight + pos1*(1.0f-blendShapeWeight) ;
	
			meshGroup1->quad[looper].vertex.x = pos3.x ;
			meshGroup1->quad[looper].vertex.y = pos3.y ;
			meshGroup1->quad[looper].vertex.z = pos3.z ;
	
		}
	}
#endif


}

/*
 brief :
*/
void StudioWindow::resize( iU32 width , iU32 height ) 
{
	init( width , height ) ;
	this->camera->reset () ;

	if ( StudioCameraType_Orthographics == this->lastCameraType ) {
		this->camera->initOrtho( -10.0 , 10.0 , 10.0 , -10.0 , 0.0 , 10000.0 ) ;
		this->camera->rotate(H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;	
		if ( this->currentObject ) {
			//	jelo s
			//currentObject->rotate (H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;
			//	jelo end
		}
	} else if ( StudioCameraType_Perspective == this->lastCameraType ) {
		this->camera->initProjection( 35.0f , (iF32)width / (iF32)height , 0.1f , 10000.0f ) ;
	}
}

/*
 brief :
*/
void StudioWindow::keyEvent( H3KeyEvent k ) 
{
	bool pressed = H3KeyEvent_Event_Press( k.e ) ;

	if ( false == pressed ) {
		return ;
	}

	//extern unsigned int* outTexPixelsRGBA32 ;
	//extern int outTexPixelsWidth ;
	//extern int outTexPixelsHeight ;
	//H3Texture* fontTexture = H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , outTexPixelsWidth , outTexPixelsHeight , (void*)outTexPixelsRGBA32 ) ;

	//ImGuiIO& io = ImGui::GetIO() ;
	//unsigned char* pixels = NULL ;
	//int texWidth, texHeight;
	//io.Fonts->GetTexDataAsRGBA32(&pixels, &texWidth, &texHeight);
	//H3Texture* fontTexture = fontTexture = H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , texWidth , texHeight , pixels ) ;

	//H3Billboard* billboard = MMC_NEW H3Billboard ( 50.0f ) ;
	//billboard->getMaterial()->addTexture ( "diffuseTexture", fontTexture , GL_TEXTURE0 ) ;
	//billboard->getMaterial()->addColor ( H3_MATERIAL_COLOR_AMBIENT, &H3Vector3f(1.0,1.0,1.0) ) ;
	//billboard->getMaterial()->addFloat ( H3_MATERIAL_SPECULARINTENSITY , 30.0f ) ;
	//billboard->getMaterial()->addFloat ( H3_MATERIAL_SHININESS , 3.0f ) ;
	//billboard->setPosition (H3Vector3f(0.0,5.0,0.0)) ;
//	billboard->setScale ( H3Vector3f(1.0,(float )texWidth/(float )texHeight,1.0) ) ;
	//addChild ( billboard ) ;

	iU32 key = H3KeyEvent_Key ( k.e ) ;

	if ( key == H3KeyEvent_Key_UP ) {
		H3Vector3f direction = this->camera->getForward () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	} else if ( key == H3KeyEvent_Key_DOWN ) {
		H3Vector3f direction = this->camera->getBackward () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	} else if ( key == H3KeyEvent_Key_LEFT ) {
		H3Vector3f direction = this->camera->getLeft () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	} else if ( key == H3KeyEvent_Key_RIGHT ) {
		H3Vector3f direction = this->camera->getRight () ;
		direction = direction.mul ( 5.1f ) ;
		H3Vector3f position = this->camera->getPosition () ;
		position = position.add ( direction ) ;
		this->camera->setPosition ( position ) ;
	}
}

/*
 brief :
*/
void StudioWindow::mouseEvent( H3MouseEvent e ) 
{
	static iF32 theLastThetaX = 0 ;
	static iF32 theLastThetaY = 0 ;

	if ( true == H3MouseEvent_Event_Click(e.e) ) {

		if ( H3MouseEvent_Left_Button == H3KeyEvent_Button(e.e) ) {

			this->click.x = (iF32)e.point.x[0] ;
			this->click.y = (iF32)e.point.y[0] ;

			this->screen.x = (iF32)this->width ;
			this->screen.y = (iF32)this->height ;
			
			if( this->camerType == StudioCameraType_Orthographics ) {
				this->ray.setD (this->camera->getForward()) ;
				H3Vector3f O = H3Ray::castRay( this->camera , this->click , this->screen ) + this->camera->getPosition () ;
				O.y = 1000.0f ; 
				this->ray.setO( O ) ;
			} else {
				this->ray.setO( this->camera->getPosition () ) ;
				this->ray.setD( H3Ray::castRay( this->camera , this->click , this->screen ) ) ;
			}

			//all object		
			chooseObject( &this->objectInfoList ) ;

			this->mouseClicked = true ;


			iF32 x = (iF32 )( e.point.x[0] - (this->width >>1 ) ) / ( this->width >> 1 ) ;
			iF32 y = (iF32 )( e.point.y[0] - (this->height >>1 ) ) / ( this->height >> 1 ) ;

			iF32 thetaX = ( x * 45.0f ) - theLastThetaX ;
			iF32 thetaY = ( y * 45.0f ) - theLastThetaY ;

			theLastThetaX = ( x * 45.0f ) ;
			theLastThetaY = ( y * 45.0f ) ;
		
		}

	} else if ( true == H3MouseEvent_Event_Release(e.e) ) {
		this->mouseClicked = false ;
	} else if ( true == H3MouseEvent_Event_Wheel(e.e) ) {
		if( this->camerType == StudioCameraType_Orthographics )
			return ;
		iF32 y = (iF32 )e.point.y[0] ;
		if ( 1.0f == y ) {
			H3Vector3f direction = this->camera->getForward () ;
			direction = direction.mul ( 1.01f );
			H3Vector3f position = this->camera->getPosition () ;
			position = position.add ( direction ) ;
			this->camera->setPosition ( position ) ;
		} else if ( -1.0f == y ) {
			H3Vector3f direction = this->camera->getBackward () ;
			direction = direction.mul ( 1.01f );
			H3Vector3f position = this->camera->getPosition () ;
			position = position.add ( direction ) ;
			this->camera->setPosition ( position ) ;
		}

	} else if ( true == H3MouseEvent_Event_Moving(e.e) ) {

		if ( true == mouseClicked ) {

			iF32 x = (iF32 )( e.point.x[0] - (this->width >>1 ) ) / ( this->width >> 1 ) ;
			iF32 y = (iF32 )( e.point.y[0] - (this->height >>1 ) ) / ( this->height >> 1 ) ;
	
			iF32 thetaX = ( x * 45.0f ) - theLastThetaX ;
			iF32 thetaY = ( y * 45.0f ) - theLastThetaY ;
	
			if( this->camerType == StudioCameraType_Perspective ) {
			this->camera->rotate ( H3Vector3f ( 0.0f , 1.0f , 0.0f ) , thetaX ) ;
			this->camera->rotate ( this->camera->getRight () , thetaY ) ;
			}else if( this->camerType == StudioCameraType_Orthographics ) {
				H3Vector3f direction = camera->getLeft( ) ;
				direction = direction.mul( thetaX ) ;
				H3Vector3f position = camera->getPosition( ) ;
				position = position.add( direction ) ;
				camera->setPosition( position ) ;

				direction = camera->getUpward( ) ;
				direction = direction.mul( thetaY ) ;
				position = camera->getPosition( ) ;
				position = position.add( direction ) ;
				camera->setPosition( position ) ;
			}
	
			theLastThetaX = ( x * 45.0f ) ;
			theLastThetaY = ( y * 45.0f ) ;
		}  
	} 
}

/*
brief :
*/
void StudioWindow::draw( iF32 deltaT )
{


	drawMap () ; 

	//	draw path
	H3DrawPrimitives::setColor ( 0.0f, 1.0f, 0.0f, 1.0f );
	if ( m_nsmoothPath ) {
		for ( int i = 0; i < m_nsmoothPath-1 ; ) {

			H3Vector3f o = H3Vector3f ( m_smoothPath[i * 3], m_smoothPath[i * 3 + 1], m_smoothPath[i * 3 + 2] ) ;
			H3Vector3f d = H3Vector3f ( m_smoothPath[(i + 1) * 3], m_smoothPath[(i + 1) * 3 + 1] + 0.1f, m_smoothPath[(i + 1) * 3 + 2] ) ;			
			H3DrawPrimitives::drawLine ( o, d );
			i = i + 2; 
		}
	}

	//h3glClearColor ( 1.0f , 1.0f , 1.0f , 1.0f ) ;

//	jelo committed start
#if 0
#ifdef WIN32

	Editor::ShowExampleAppMainMenuBar( ) ;

	ImGui::Begin( "Main Menu" ) ;

	//ImGui::Text( "AB" ) ;

	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	Editor::draw2d3dChange( ) ;

	ImGui::Spacing( );
	ImGui::Spacing( );
	ImGui::Spacing( );
	ImGui::Spacing( );
	ImGui::Spacing( );

	static bool show_objects = false ;
	static bool show_cameras = false ;
	static bool show_lights = false ;
	ImGui::SameLine();
	
	static int e = 0;
	if( ImGui::RadioButton( H3UtilGbk2Utf8( string( "Ïà»ú" ) ).data( ) , &e , 0 ) ) {
		show_cameras ^= 1 ;
		show_objects = false ;
		show_lights = false ;
	}
	ImGui::SameLine();

	if( ImGui::RadioButton( H3UtilGbk2Utf8( string( "¹âÕÕ" ) ).data( ) , &e , 1 ) ) {
		show_lights ^= 1 ;
		show_objects = false ;
		show_cameras = false ;
	}
	ImGui::SameLine();

	if( ImGui::RadioButton( H3UtilGbk2Utf8( string( "Â¥²ã" ) ).data( ) , &e , 2 ) ) {
		show_objects ^= 1 ;
		show_cameras = false ;
		show_lights = false ;
	}
	
	if( show_cameras ){
		Editor::drawMenuSelect( 1 ) ;
	}
	if( show_lights ){
		Editor::drawMenuSelect( 2 ) ;
	}
	if( show_objects ){
		Editor::drawMenuSelect( 3 ) ;
	}
	
	if( this->show_details_windows ) {
		Editor::drawDetailsWindow( this->gDetailsType , this->gDetailsRef ) ;
	}

	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	
	ImGui::End( ) ;

	if( this->lastCameraType > this->camerType ) {
		this->camera->getTransform().reset () ;
		this->camera->initOrtho( -10.0 , 10.0 , 10.0 , -10.0 , 0.0001 , 10000.0 ) ;		
		this->camera->setPosition( H3Vector3f( 0.0f , 0.0f , 0.0f ) ) ;
		this->camera->rotate(H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;
	} else if ( this->lastCameraType < this->camerType ) {
		this->camera->getTransform().reset () ;
		this->camera->initProjection( 35.0f , 1024.0 / 720.0 , 0.1f , 10000.0f ) ;
		this->camera->setPosition( H3Vector3f( 0 , 0 , 0 ) ) ;
	}

	this->lastCameraType = this->camerType ;

	//H3DrawPrimitives::drawLine( this->ray.getO() , this->ray.getD()*(10000.0f) ) ;

#endif
#endif

	if( this->lastCameraType > this->camerType ) {
		this->camera->reset () ;
		this->camera->initOrtho( -10.0 , 10.0 , 10.0 , -10.0 , 0.0001 , 10000.0 ) ;		
		this->camera->setPosition( H3Vector3f( 0.0f , 0.0f , 0.0f ) ) ;
		this->camera->rotate(H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;
	} else if ( this->lastCameraType < this->camerType ) {
		this->camera->reset () ;
		this->camera->initProjection( 35.0f , (iF32)this->width / (iF32)this->height , 0.1f , 10000.0f ) ;
		this->camera->setPosition( H3Vector3f( 0 , 0 , 0 ) ) ;
	}

	this->lastCameraType = this->camerType ;

	//H3DrawPrimitives::setColor ( 255.0f , 0 , 0 , 255.0f ) ;
	//H3DrawPrimitives::drawLine( this->ray.getO() , this->ray.getD()*(10000.0f) ) ;

//	jelo committed end
}

/*
 brief :  
*/
void StudioWindow::addObject( H3Object* object )
{
	this->currentObject = object ;
	addChild( object ) ;
}

/*
 brief :  
*/
void StudioWindow::deleteObject( H3Object* object )
{
	SCClListDelete( &this->objectList , object ) ;

	SodaObjectInfo* objectInfo = selectObjectInfo( object ) ;
	SCClListDelete( &this->objectInfoList , objectInfo ) ;

	//iS32 objectIndex = selectObjectIndex ( object ) ;
	//assert(objectIndex != -1 ) ;

	H3Mesh* mesh = (H3Mesh*)object->getMesh() ;
	iS32 meshTotal = mesh->meshGroupList.size() ;

	if( this->currentSelectedObject == object )
	{
		this->currentSelectedObject = NULL ;
		this->currentSelectedMeshId = -1 ;
	}

	if( this->currentObject == object )
		this->currentObject = NULL ;

	this->gDetailsType = -1 ;
	this->gDetailsRef = NULL ;

	object->removeFromParent() ;
}

/*
 brief :
*/
H3Object* StudioWindow::generateObject( iS8* filepath )
{
	H3Material material ;
	material.addFloat ( H3_MATERIAL_SPECULARINTENSITY , 26.0f ) ;
	material.addFloat ( H3_MATERIAL_SHININESS , 0.6f ) ;
	material.addColor ( H3_MATERIAL_COLOR_AMBIENT , &H3Vector3f(1.0f,1.0f,1.0f) ) ;
	material.addFloat ( H3_MATERIAL_SSS , 0.0f ) ;
	material.addFloat ( H3_MATERIAL_FRESNEL , 0.24f ) ;  

	H3MeshRender* render = 0 ;
#ifdef FACIAL_TEST
	H3FacialRenderDeformer0 = MMC_NEW H3FacialRender ( MMC_NEW H3Mesh( "models/Neutral.obj" , &material ) ) ;
	//	test
	H3FacialRenderDeformer1 = MMC_NEW H3FacialRender( MMC_NEW H3Mesh( filepath , &material ) ) ;
	render = H3FacialRenderDeformer1 ;
#else
	render = MMC_NEW H3MeshRender(MMC_NEW H3Mesh(filepath, &material)) ;
#endif

	H3Object* object = MMC_NEW H3Object( ) ;

	object->addComponent ( render );
	object->setPosition( H3Vector3f ( 0.0f , 0.0f , 0.0f ) ) ;
	object->rotate( H3Vector3f( 1.0 , 0.0 , 0 ) , 0.0f ) ;
	
	SodaObjectInfo* objectInfo = ( SodaObjectInfo* )MMC_MALLOC( sizeof( SodaObjectInfo ) ) ;
	assert( objectInfo != NULL ) ;

	//20160408 add     -- Begin
	objectInfo->isclick = false ;
	//                 -- End

	objectInfo->isBillboard = false ;
	objectInfo->objectRef = object ;
	objectInfo->objectIndex = gObjectIndex ;
	objectInfo->meshId = -1 ;
	objectInfo->size = 0 ;

	objectInfo->scale.x = 1.0f ;
	objectInfo->scale.y = 1.0f ;
	objectInfo->scale.z = 1.0f ;

	objectInfo->rotate.axis.x = 1.0f ;
	objectInfo->rotate.axis.y = 0.0f ;
	objectInfo->rotate.axis.z = 0.0f ;
	objectInfo->rotate.amount = 0.0f ;
	
	strcpy( objectInfo->filePath , filepath ) ;

	gObjectIndex ++ ;
	SCClListInsert( &this->objectList , object ) ;
	SCClListInsert( &this->objectInfoList , objectInfo ) ;

	this->currentObject = object ;
	this->currentSelectedObject = object ;

	H3Mesh* mesh = (H3Mesh*)object->getMesh() ;
	iS32 meshTotal = mesh->meshGroupList.size() ;

#ifdef FACIAL_TEST
	H3MeshGroup* meshGroup = mesh->getMeshGroup ( 0 ) ;
	H3Texture* fontTexture = H3Texture::create( H3TEXTURE_TYPE::H3_TEXTURE_PIXEL , "textures/Pain.png" ) ;
	meshGroup->material->addTexture ( "diffuseTexture" , fontTexture , GL_TEXTURE0 )  ;
#endif

	for( iS32 walker = 0 ; walker < meshTotal ; walker ++ ) 
	{
		SodaMesh* meshInfo = ( SodaMesh* )MMC_MALLOC( sizeof( SodaMesh ) ) ;
		assert( objectInfo != NULL ) ;

		meshInfo->meshId = walker ;
		meshInfo->objectIndex = gObjectIndex - 1 ;
		meshInfo->color.alpha = 1.0f ;
		meshInfo->color.blue = 1.0f ;
		meshInfo->color.green = 1.0f ;
		meshInfo->color.red = 1.0f ;
		meshInfo->position.x = 0.0f ;
		meshInfo->position.y = 0.0f ;
		meshInfo->position.z = 0.0f ;

		meshInfo->sss = material.getFloat ( H3_MATERIAL_SSS ) ;
		meshInfo->shininess = material.getFloat ( H3_MATERIAL_SHININESS ) ;
		meshInfo->specularIntensity = material.getFloat ( H3_MATERIAL_SPECULARINTENSITY ) ;
		meshInfo->fresnel = material.getFloat ( H3_MATERIAL_FRESNEL ) ;

		SCClListInsert( &this->meshList , meshInfo ) ;
	}

	return object ;

}

/*
 brief :
*/
H3Image* StudioWindow::createLogoImage ( iS8* filepath )
{
	if( strlen( filepath ) == 0 ) {
		return NULL ;
	}

	H3Image* image = MMC_NEW H3Image(filepath) ;
	
	bool ret = image->init ( filepath ) ;

	if ( false == ret ) {
		iLog ( "get logo data failed" ) ;
		return NULL ;
	}

	if ( H3_RGB_FORMAT_RGB24 == image->getType () ) 
		image->convsert2rgba8888 () ;

	image = setImageRoundRect( image , 0.2 ) ;

	image = setImageBorder( image ) ;

	return image ;

	//int scale = 2 ;
	//H3Image* newImage = 0 ;
	//newImage = setFontScale( image , scale ) ;
	//return newImage ;

	
}

/*
 brief :
*/
H3Image* StudioWindow::createTextImage ( iS8* words )
{
	H3Image* dstimage = MMC_NEW H3Image() ;
	iS32 length = H3UtilGetUtf8Length(words) ;
	if( length == 0 ) {
		return NULL ;
	}
	dstimage->init(H3Font::getWidth(words) , 20,H3_RGB_FORMAT_RGBA32) ;

	iU32 out_char ;
	iS8 texttest [256] = {0} ;
	iS8 in_text [ 32 ] = {""} ;
	iS8 in_text_end [ 32 ] = {""} ;

	iS32 walker = 0 ;
	iS32 widthWalker = 0 ;
	iS32 heightWalker = 20 ;

	//strcpy ( texttest , H3UtilGbk2Utf8(words).data() ) ;
	strcpy ( texttest , words ) ;
	  
	while(texttest[walker] != '\0') {
		unsigned int c = (unsigned int)0xffffff00 | texttest[walker] ;
        if (c < 0xffffff80) { 
			;
        }
        else {
			extern iS32 H3UtilConvertCharFromUtf8 (iU32* out_char, const iS8* in_text, const iS8* in_text_end)  ;
			H3UtilConvertCharFromUtf8 ( &c , texttest+walker , in_text_end ) ;	
            if (c == 0)
                break ;
			walker+=2 ;
        }

		H3Font::TexCoord fontTexCoord = H3Font::getTexCoord( c ) ;

		int X0 = (float )fontTexCoord.U0 * this->fontWidth ;
		int X1 = (float )fontTexCoord.U1 * this->fontWidth ;
		int Y0 = (float )fontTexCoord.V0 * this->fontHeight ;
		int Y1 = (float )fontTexCoord.V1 * this->fontHeight ;

		iU8* tempFontData = (iU8*)this->fontData ;

		iS32 width = X1 - X0 ;

		iS32 height = Y1 - Y0 ;

		//if( 20 -  height > 9)
			//heightWalker = 2 ;

		H3Image srcimage ;
		srcimage.init ( tempFontData , this->fontWidth , this->fontHeight , H3_RGB_FORMAT_RGBA32 ) ;
		H3RasterPen pen = H3RasterPen( H3RasterColor(50,50,50,255), 1) ;
		H3RasterRender::drawImage ( dstimage , &srcimage , H3Vector2i(widthWalker,heightWalker- height-1) , X0 , Y0 , X1 - 1 , Y1 - 1 , &pen ) ;

		widthWalker += width ;

		walker++ ;
	}

	return dstimage ;

	//return dstimage ;
}

/*
 brief :
*/
H3Image* StudioWindow::creatBillboardImage ( H3Image* logoImage , H3Image* wordsImage )
{
	
	//10 pixels width
	iS32 blankRotia = 10 ;
	H3Image* results = 0 ;
	if( logoImage == NULL && wordsImage == NULL ) 
		return NULL ;
	 
	if( logoImage == NULL && wordsImage != NULL ) {
		results = wordsImage ;
	} else if( logoImage != NULL && wordsImage == NULL ) {
		results = logoImage ;
	} else if( logoImage != NULL && wordsImage != NULL ) {
	
		iS32 width = (logoImage->getWidth() +  wordsImage->getWidth()) +  blankRotia  ; // 0.5 ,1 ,0.5
		while(width%2!=0)
			width ++ ;

		iS32 maxHeight = logoImage->getHeight() > wordsImage->getHeight() ? logoImage->getHeight() : wordsImage->getHeight() ;

		iS32 height = maxHeight +  0.5 * blankRotia;
		while(height%2!=0)
			height ++ ;

		iS32 beginHeight = 1 ;
		if( wordsImage->getHeight() < maxHeight ) {
			 beginHeight = ( height - wordsImage->getHeight() ) / 2 ;
		}

		//iF32 scaleX = width / logoImage->getWidth () ;
		//logoImage->setScale ( scaleX ) ;

		//scaleX = floor(width / wordsImage->getWidth ()) ; 
		//wordsImage->setScale ( scaleX ) ;

		H3Image* image = MMC_NEW H3Image() ;
		image->init ( width , height , H3_RGB_FORMAT_RGBA32 ) ;

		if ( H3_RGB_FORMAT_RGB24 == logoImage->getType () ) 
			logoImage->convsert2rgba8888 () ;

		if ( H3_RGB_FORMAT_RGB24 == wordsImage->getType () ) 
			wordsImage->convsert2rgba8888 () ;
		  
		H3RasterRender::drawImage ( image , logoImage , H3Vector2i(0,0) , 0 , 0 , logoImage->getWidth()-1 , logoImage->getHeight()-1 ) ;
		H3RasterRender::drawImage ( image , wordsImage , H3Vector2i(logoImage->getWidth()+blankRotia-1,beginHeight-1) , 0 , 0 , wordsImage->getWidth()-1 , wordsImage->getHeight()-1 ) ;

		results = image ;
	}

	if ( H3_RGB_FORMAT_RGB24 == results->getType () ) 
		results->convsert2rgba8888 () ;

	return results ;

}

/*
 brief :
*/
H3Image* StudioWindow::setImageRoundRect ( H3Image* image , iF32 radiusRatio )
{
	unsigned char* imageData = (unsigned char*)image->getData() ;

	iS32 radius = (radiusRatio * image->getWidth()) ;

	//0 ~ n-1
	iS32 rangeA = image->getWidth() * radiusRatio ;
	iS32 rangeB = image->getWidth() * (1 - radiusRatio) ; 
	iS32 rangeC = image->getHeight() * radiusRatio ;
	iS32 rangeD = image->getHeight() * (1 - radiusRatio) ;

	//pointA ( rangeA-1 , rangeC-1 ) ; pointB ( rangeB-1 , rangeC-1 )
	//pointC ( rangeA-1 , rangeD-1 ) ; pointD ( rangeB-1 , rangeD-1 )

	int stride = 0;
	int pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGBA32 == image->getType () ) 
		pixelBytes = 4 ;
	else
		pixelBytes = 3 ;
	stride = image->getWidth()*pixelBytes ;

	for( iS32 y = 0 ; y < rangeC ; y ++ ) {
		for( iS32 x = 0 ; x < rangeA ; x ++ ) {
			float distance = sqrtf((x-rangeA+1)*(x-rangeA+1)+(y-rangeC+1)*(y-rangeC+1)) ;
			if( distance >= radius ) {
				if( distance - radius <= 1.4f )
					;
				else {
					if ( H3_RGB_FORMAT_RGBA32 == image->getType () ) 
						imageData[y*stride+x*pixelBytes+3] = 0 ;
					else {
						imageData[y*stride+x*pixelBytes+0] = 0 ;
						imageData[y*stride+x*pixelBytes+1] = 0 ;
						imageData[y*stride+x*pixelBytes+2] = 0 ;
					}
				}
			}
		}
	}
	for( iS32 y = 0 ; y < rangeC ; y ++ ) {
		for( iS32 x = rangeB-1 ; x < image->getWidth() ; x ++ ) {
			float distance = sqrtf((x-rangeB+1)*(x-rangeB+1)+(y-rangeC+1)*(y-rangeC+1)) ;
			if( distance >= radius ) {
				if( distance - radius <= 1.4f )
					;
				else {
					if ( H3_RGB_FORMAT_RGBA32 == image->getType () ) 
						imageData[y*stride+x*pixelBytes+3] = 0 ;
					else {
						imageData[y*stride+x*pixelBytes+0] = 0 ;
						imageData[y*stride+x*pixelBytes+1] = 0 ;
						imageData[y*stride+x*pixelBytes+2] = 0 ;
					}
				}
			}
		}
	}

	for( iS32 y = rangeD-1 ; y < image->getHeight() ; y ++ ) {
		for( iS32 x = 0 ; x < rangeA ; x ++ ) {
			float distance = sqrtf((x-rangeA+1)*(x-rangeA+1)+(y-rangeD+1)*(y-rangeD+1)) ;
			if( distance >= radius ) {
				if( distance - radius <= 1.4f )
					;
				else {
					if ( H3_RGB_FORMAT_RGBA32 == image->getType () ) 
						imageData[y*stride+x*pixelBytes+3] = 0 ;
					else {
						imageData[y*stride+x*pixelBytes+0] = 0 ;
						imageData[y*stride+x*pixelBytes+1] = 0 ;
						imageData[y*stride+x*pixelBytes+2] = 0 ;
					}
				}
			}
		}
	}

	for( iS32 y = rangeD-1 ; y < image->getHeight() ; y ++ ) {
		for( iS32 x = rangeB-1 ; x < image->getWidth() ; x ++ ) {
			float distance = sqrtf((x-rangeB+1)*(x-rangeB+1)+(y-rangeD+1)*(y-rangeD+1)) ;
			if( distance >= radius ) {
				if( distance - radius <= 1.4f )
					;
				else {
					if ( H3_RGB_FORMAT_RGBA32 == image->getType () ) 
						imageData[y*stride+x*pixelBytes+3] = 0 ;
					else {
						imageData[y*stride+x*pixelBytes+0] = 0 ;
						imageData[y*stride+x*pixelBytes+1] = 0 ;
						imageData[y*stride+x*pixelBytes+2] = 0 ;
					}
				}
			}
		}
	}

	return image ;
}

/*
 brief :
*/
H3Image* StudioWindow::setImageBorder ( H3Image* image)
{
	unsigned char* imageData = image->getData() ;

	H3RasterPen pen = H3RasterPen( H3RasterColor(127,127,127,1), 1) ;

	int stride = 0;
	int pixelBytes = 0 ;
	if ( H3_RGB_FORMAT_RGBA32 == image->getType () ) 
		pixelBytes = 4 ;
	else
		pixelBytes = 3 ;
	stride = image->getWidth()*pixelBytes ;

	for( iS32 y = 0 ; y < image->getHeight() ; y ++ )
	{
		for( iS32 x = 0 , deltaX =0 ; x < image->getWidth() * pixelBytes ; x+=pixelBytes ,deltaX ++)
		{
			if( x == 0 || y == 0)
				H3RasterRender::drawPoint ( image , &pen , deltaX , y ) ;
		}
	}
	return image ;
}

/*
 brief :
*/
void StudioWindow::addLight( iS32 type )
{
	H3LightAtom* atom = ( H3LightAtom* ) H3MemoryMalloc( sizeof( H3LightAtom ) , __FILE__ , __LINE__ ) ;
	H3ASSERT( NULL != atom ) ;

	atom->type = type ;
	if( atom->type == H3COMPONENT_TYPE_AMBIENT_LIGHT ) {
		H3AmbientLight* h3AmbientLight = MMC_NEW H3AmbientLight( H3Vector3f( 0.5f , 0.5f , 0.5f ) ) ;
		H3ASSERT( NULL != h3AmbientLight ) ;
		atom->lightRef = h3AmbientLight ;
		addComponent( h3AmbientLight ) ; 
		SCClListInsert( &this->lightList , atom ) ;
	}else if( atom->type == H3COMPONENT_TYPE_DIRECTIONAL_LIGHT ) {
		H3DirectionalLight* h3DirectionalLight = MMC_NEW H3DirectionalLight( H3Vector3f( 0.5f , 0.5f , 0.5f ) , H3Vector3f( 0.3f , -1.0f , 1.0f ) , 1.0f ) ;
		H3ASSERT( NULL != h3DirectionalLight ) ;
		atom->lightRef = h3DirectionalLight ;
		addComponent( h3DirectionalLight ) ; 
		SCClListInsert( &this->lightList , atom ) ;
	}
	/*
	else if( atom->type == H3COMPONENT_TYPE_POINT_LIGHT ) {
		H3PointLight* h3PointLight ;
		H3ASSERT( NULL != h3PointLight ) ;
		atom->lightRef = h3PointLight ;
		addComponent( h3PointLight ) ; 
		SCClListInsert( &this->lightList , atom ) ;
	}else if( atom->type == H3COMPONENT_TYPE_SPOT_LIGHT ) {
		H3SpotLight* h3SpotLight ;
		H3ASSERT( NULL != h3SpotLight ) ;
		atom->lightRef = h3SpotLight ;
		addComponent( h3SpotLight ) ; 
		SCClListInsert( &this->lightList , atom ) ;
	}*/
}

/*
 brief :
*/
void StudioWindow::chooseObject( SCClList* objects )
{
	iF32 distanceMin = 999999.0 ;
	H3Object* objectMin =  0 ;

	this->currentSelectedObject = NULL ;
	this->currentSelectedMeshId = -1 ;

	for ( SCClList* walker = objects->next ;walker ; walker = walker->next )
	{
		iS32 meshId = 0 ;

		SodaObjectInfo* objectInfo = ( SodaObjectInfo* )walker->element ;

		if ( true == objectInfo->isBillboard ) 
			continue ;

		H3Object* object = dynamic_cast<H3Object* >( objectInfo->objectRef ) ;

		this->intersectPoint = H3Vector3f( 0.0f , 0.0f , 0.0f ) ;

		meshId = object->intersect( this->ray , &H3Vector3f(0.0,1.0,0.0) , this->intersectPoint ) ;

		if ( -1 == meshId ) 
			continue ;

		H3Vector3f objectPoint = object->getPosition() ;
		H3Vector3f direction = objectPoint - camera->getPosition();
		iF32 distance = direction.mag() ;

		if( distance < distanceMin ) {
			distanceMin = distance ;
			objectMin = object ;	
			this->currentSelectedMeshId = meshId ;
			this->currentSelectedObject = object ;
		}
	}

	currentObject = objectMin ;
	if( currentObject ) {

		//	nav mesh
		navMesh.generatePath ( H3Vector3f ( 0, 0, 0 ), H3Vector3f ( intersectPoint ) );
		navMesh.getPath ( &m_smoothPath, &m_nsmoothPath );
		//

		this->gDetailsType = 4 ;
		this->gDetailsRef = ( void* ) currentObject;

		H3Mesh* mesh = (H3Mesh* )this->currentSelectedObject->getMesh () ;
		//add for temporary
		if(this->currentSelectedMeshId != -1 ) {
			H3MeshGroup* meshGroup = mesh->getMeshGroup( this->currentSelectedMeshId ) ;
			H3Vector3f* color = meshGroup->material->getColor ( H3_MATERIAL_COLOR_AMBIENT ) ;
			gOrignalMeshColor.init ( color->x , color->y , color->z ) ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_AMBIENT , &gOrignalMeshColor.mul(1.5) ) ;
		}
	}
}

int StudioWindow::isCurrentObjectNull ( H3Object* object )
{
	if( object == NULL ) {
		return 1 ;
	} else { 
		return 0 ;
	}
}

/*
 brief : 20160303
*/
void StudioWindow::loadFile ( iS8* filepath )
{

	iS8 postfix[4] = {0} ;
	iS32 pathlength = strlen( filepath ) ;

	for ( iS32 walker = 0 ; walker < 4 ; walker ++ )
	{
		postfix[walker] = filepath[pathlength-3+walker] ;
	}

	if ( !strcmp( postfix , "obj" ) ) {  //open .obj file

		iS8 path[256] = {0} ;
		strcpy( path , filePathTransform(filepath) ) ;
		H3Object* object = generateObject( path ) ;
		addObject( object ) ;

	} else if ( !strcmp( postfix , "map" ) ) {

	}

}

/*
 brief : 
*/
void StudioWindow::setBlendShapeWeight ( iF32 weight ) 
{
	this->blendShapeWeight = weight ;
}

/*
 breif :
*/
iF32 StudioWindow::getBlendShapeWeight () 
{
	return this->blendShapeWeight ;
}

/*
 brief :
*/
SodaObjectInfo* StudioWindow::selectObjectInfo ( H3Object* object )
{
	for ( SCClList* walker = ( SCClList* )this->objectInfoList.next ; walker ; walker = walker->next ) {

		SodaObjectInfo* objectInfo = ( SodaObjectInfo* )walker->element ;
		if( object == objectInfo->objectRef ) {
			return objectInfo ;
		}
	}

}

/*
 brief :
*/
iS32 StudioWindow::selectObjectIndex ( H3Object* object )
{
	for ( SCClList* walker = ( SCClList* )this->objectInfoList.next ; walker ; walker = walker->next ) {

		SodaObjectInfo* objectInfo = ( SodaObjectInfo* )walker->element ;
		if( object == objectInfo->objectRef ) {
			return objectInfo->objectIndex ;
		}
	}

	return -1 ;

}

/*
 brief : 20160310
*/
SodaObjectInfo* StudioWindow::selectBillboardInfo ( iS32 objectIndex , iS32 meshId )
{
	for ( SCClList* walker = ( SCClList* )this->objectInfoList.next ; walker ; walker = walker->next ) {

		SodaObjectInfo* billboardInfo = ( SodaObjectInfo* )walker->element ;
		if ( false == billboardInfo->objectRef->isBillboard ) 
			continue ;
		if( objectIndex == billboardInfo->objectIndex && meshId == billboardInfo->meshId ) {
			return billboardInfo ;
		}
	}

	return NULL ;
}

/*
 brief : 20160315
*/
SodaMesh*  StudioWindow::selectMeshInfo ( iS32 objectIndex , iS32 meshId )
{
	for ( SCClList* walker = ( SCClList* )this->meshList.next ; walker ; walker = walker->next ) {

		SodaMesh* meshInfo = ( SodaMesh* )walker->element ;

		if( objectIndex == meshInfo->objectIndex && meshId == meshInfo->meshId ) {
			return meshInfo ;
		}
	}

	return NULL ;
}

/*
 brief : 20160314
*/
void StudioWindow::scanObject( SCClList* objectList )
{
	for( SCClList* walker = objectList->next ; walker ; walker = walker->next )
	{
		H3Object* object = ( H3Object* )walker->element ;

		iS32 objectIndex = selectObjectIndex( object ) ;
		if( objectIndex == -1) {
			return ;
		}
		
		if( objectIndex > gObjectIndex ) {
			gObjectIndex = objectIndex ;
		}
		
		scanObject( &object->childs ) ;
	}
}

/*
 brief : 
*/
iS8* StudioWindow::filePathTransform( iS8* filepath ) 
{
	iS8 path[256] = {0} ;

	iS32 count = 0 ;
	iS32 walker = 0 ;
 	while( filepath[count] != '\0' )
	{
		if( (filepath[count] == 'H' || filepath[count] == 'h') && filepath[count+1] == '3' ) {
			count += 3 ;
			break ;
		}
		count ++ ;
	}

	while( filepath[walker] != '\0' )
	{
		path[walker] = filepath[walker+count] ;
		if( path[walker] == '\\' ) {
			path[walker] = '/' ;
		}
		walker ++ ;
	}

	return path ;
}

/*
 brief :
*/
void StudioWindow::clean () 
{

	for( SCClList* walker = ( SCClList* )lightList.next ; walker ; walker = walker->next ) {
		H3MemoryFree( walker->element ) ;
	}
	SCClListClear( &this->lightList ) ;

	for( SCClList* walker = ( SCClList* )objectInfoList.next ; walker ; walker = walker->next ) {
		H3MemoryFree( walker->element ) ;
	}
	SCClListClear( &this->objectInfoList ) ;

	for( SCClList* walker = ( SCClList* )meshList.next ; walker ; walker = walker->next ) {
		H3MemoryFree( walker->element ) ;
	}
	SCClListClear( &this->meshList ) ;

	SCClListClear( &this->billboardList ) ;
	 
	SCClListClear( &this->objectList ) ;

	removeAllChildAndComponent () ;

	this->h3AmbientLight = NULL ;
	
	this->currentObject = NULL ;

	this->currentSelectedObject = NULL ;

	this->currentSelectedMeshId = 0 ;

	this->gDetailsRef = NULL ;

	this->gDetailsType = 0 ;
}

/*
 brief :
*/
void StudioWindow::accelerometerEvent ( H3AccelerometerEvent e ) {}