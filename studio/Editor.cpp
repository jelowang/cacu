#include <windows.h>

#include "H3MeshRender.h"
#include "H3Object.h"
#include "StudioWindow.h"
#include "H3Raster.h"
#include "H3Image.h"
#include "H3Delegate.h"
#include "editor.h"
#include "imgui_internal.h"

#define STB_TRUETYPE_IMPLEMENTATION 

#include "stb_truetype.h"

extern StudioWindow* gStudioWindow ;
bool closeObjectRotate = true ;
bool show_texture = false ;
bool temp_set_mix_texture = false ;
iS8 textureFilepath[MAX_PATH] = {0} ;

/*
 brief :
*/
static iS8* getFilePath( iS32 type )
{
	TCHAR szBuffer[MAX_PATH] = { 0 } ;
	TCHAR defaultPath[MAX_PATH] = { 0 } ;
	GetCurrentDirectory ( sizeof( defaultPath ) , defaultPath ) ;
	
	OPENFILENAME ofn = { 0 } ;
	memset ( &ofn , 0 , sizeof( ofn ) ) ;
	ofn.lStructSize = sizeof( ofn ) ;
	ofn.hwndOwner = NULL ;
	ofn.lpstrFilter = L"*.obj\0*.obj\0\0" ;
	ofn.lpstrInitialDir = ( LPCWSTR )H3UtilGbk2Utf8( string( "d:\\" ) ).data() ;
	ofn.lpstrFile = szBuffer ;
	ofn.nMaxFile = sizeof( szBuffer ) / sizeof( *szBuffer ) ;
	ofn.nFilterIndex = 0 ;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;
	
	if( type == 0 ) {
		bool bSEL = GetOpenFileName( &ofn ) ;
	} else if( type == 1 ) {
		bool bSEL = GetSaveFileName( &ofn ) ;
	}
	
	SetCurrentDirectory ( defaultPath ) ;

	iS8 filepath[MAX_PATH] = { 0 } ;

	iS32 iLength ;
	iLength = WideCharToMultiByte( CP_ACP , 0 , szBuffer , -1 , NULL , 0 , NULL , NULL ) ;
	WideCharToMultiByte( CP_ACP , 0 , szBuffer , -1 , filepath , iLength , NULL , NULL ) ;

	return filepath ;
}

/*
 brief :
*/
iS32 Editor::draw2d3dChange( )
{
	ImGui::Spacing( );
	ImGui::Spacing( );
	ImGui::Spacing( );

	const char* items[] = { "2D", "3D" };
    static int item = 1;

	const char* wireFrame[] = { "Normal", "Wire Frame" };
    static int wireFrameIndex = 0;

    ImGui::Combo("Mode", &item, items, IM_ARRAYSIZE(items));
	ImGui::Combo("Render", &wireFrameIndex, wireFrame, IM_ARRAYSIZE(wireFrame));
	ImGui::SameLine();

	if( item == 0 ) {
		//gStudioWindow->camera->initOrtho( -10.0 , 10.0 , 10.0 , -10.0 , 0.0001 , 10000.0 ) ;		
		//gStudioWindow->camera->setPosition( H3Vector3f( 0.0f , 5.0f , 0.0f ) ) ;
		//gStudioWindow->camera->rotate(H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;
		gStudioWindow->camerType = StudioCameraType_Orthographics ;
		closeObjectRotate = false ;
		
	}else if( item == 1 ) {
		//gStudioWindow->camera->initProjection( 35.0f , 1024.0 / 720.0 , 0.1f , 10000.0f ) ;
		gStudioWindow->camerType = StudioCameraType_Perspective ;
		closeObjectRotate = true ;
	}

	if ( 0 == wireFrameIndex ) 
		H3Delegate::getInstance()->setWireFrameEnable ( false ) ;
	else 
		H3Delegate::getInstance()->setWireFrameEnable ( true ) ;

	return 0 ;
}

/*
 brief :
*/
void Editor::drawMenuSelect ( iS32 index ) 
{
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ; 
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	if( index == 1 ) {

		bool show_add_camera = false ;

	    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	    ImGui::BeginChild( "Sub1" , ImVec2( 0 , 70 ) , true ) ;
	        
		char buf[32];
	    sprintf( buf , "Camera %d" , 1 );
		if( ImGui::Button( buf , ImVec2( -1.0f , 0.0f ) ) ) {
			gStudioWindow->show_details_windows = true ;
			gStudioWindow->gDetailsType = 1 ;
		}

		if( ImGui::Button(H3UtilGbk2Utf8( string( "Add Camera" ) ).data()) ) show_add_camera ^= 1 ;

		ImGui::EndChild() ;
	    ImGui::PopStyleVar() ;
	}

	if( index == 2 ) {

		bool show_add_light = false ;
		
	    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	    ImGui::BeginChild( "Sub2" , ImVec2( 0 , gStudioWindow->lightList.totall * 35 + 35 ) , true ) ;

		iS32 count = 0 ;
		for( SCClList* walker = ( SCClList* )gStudioWindow->lightList.next ; walker ; walker = walker->next ) {
		
			char buf[32] ;
			if( count == 0 ) {
				sprintf( buf , H3UtilGbk2Utf8( string( "Ambitient Light" ) ).data() , count );
			}else{
				sprintf( buf , "Light %d" , count+1 );
			}
			if( ImGui::Button( buf , ImVec2( -1.0f , 0.0f ) ) ) {
				H3LightAtom* atom =  ( H3LightAtom* )walker->element ;
				if( atom->type == H3COMPONENT_TYPE_AMBIENT_LIGHT ) {
					gStudioWindow->show_details_windows = true ;
					gStudioWindow->gDetailsType = 2 ;
				} else if( atom->type = H3COMPONENT_TYPE_DIRECTIONAL_LIGHT ) {
					gStudioWindow->show_details_windows = true ;
					gStudioWindow->gDetailsType = 3 ;
					gStudioWindow->gDetailsRef = (void*)atom->lightRef ;
				} else if( atom->type = H3COMPONENT_TYPE_POINT_LIGHT ) {
					gStudioWindow->show_details_windows = false ;
				} else if( atom->type = H3COMPONENT_TYPE_SPOT_LIGHT ) {
					gStudioWindow->show_details_windows = false ;
				}
			}
			count ++ ;
		}
		char str1[64] = {0} ;
		strcpy( str1 , H3UtilGbk2Utf8( string( "Directional Light" ) ).data()) ;
		const char* items[] = { str1 };
        static int item = 0 ;
        ImGui::Combo("", &item, items, IM_ARRAYSIZE(items));
		ImGui::SameLine();

		if( ImGui::Button(H3UtilGbk2Utf8( string( "Add Light" ) ).data()) ) show_add_light ^= 1 ;

		if( show_add_light ) {

			if( item == 0 ) {
				gStudioWindow->addLight( H3COMPONENT_TYPE_DIRECTIONAL_LIGHT ) ;
			}
			/*
			else if( item == 1 ) {	
				gStudioWindow->addLight( H3COMPONENT_TYPE_POINT_LIGHT ) ;
			}else if( item == 2 ) {
				gStudioWindow->addLight( H3COMPONENT_TYPE_SPOT_LIGHT ) ;
			}*/
		}

		ImGui::EndChild() ;
	    ImGui::PopStyleVar() ;
	}

	if( index == 3 ) {
		bool show_add_object = false ;
		bool show_delete_object = false ;
		

	    ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
	    ImGui::BeginChild( "Sub3" , ImVec2( 0 , gStudioWindow->objectList.totall * 35 + 35 ) , true ) ;

		iS32 count = 0 ;
		//20160408 add     -- Begin
		H3Object* clickObject = NULL ;
		SCClList tempClickObject ;
		SCClListInit( &tempClickObject ) ;
		//                 -- End
		for( SCClList* walker = ( SCClList* )gStudioWindow->objectList.next ; walker ; walker = walker->next ) {
	        char buf[32];
			H3Object* object = (H3Object*) walker->element ;
			SodaObjectInfo* objectInfo = gStudioWindow->selectObjectInfo( object ) ;

			//20160408 add     -- Begin
			char buf1[32] ;
			sprintf( buf1 , "%d" , count+1 );
			ImGui::Checkbox(buf1 , &objectInfo->isclick ) ;
			ImGui::SameLine();
			if( objectInfo->isclick ) {
				//clickObject = object ;
				SCClListInsert( &tempClickObject , object ) ;
			}
			//                 -- End

			sprintf( buf , "Object %d - %s" , count+1 , objectInfo->filePath );
			//if( ImGui::Button( buf , ImVec2( -1.0f , 0.0f ) ) ) {
            if( ImGui::Button( buf ) ) {
				gStudioWindow->show_details_windows = true ;
				gStudioWindow->gDetailsType = 4 ;
				gStudioWindow->gDetailsRef = ( void* )object ;
				gStudioWindow->currentSelectedObject = object ;
				gStudioWindow->currentObject = object ;
			}
			count ++ ;
		}

		if( ImGui::Button(H3UtilGbk2Utf8( string( "Add Object" ) ).data()) ) show_add_object ^= 1 ;

		if( show_add_object ) {
			iS8 filepath[MAX_PATH] = {0} ;
			strcpy( filepath , getFilePath(0) ) ;
			gStudioWindow->loadFile( filepath ) ;
		}
		
		//delete object (20160408)     -- Begin
		ImGui::SameLine(200);
		if( count != 0 ) {
			if( ImGui::Button( H3UtilGbk2Utf8( string( "Delete" ) ).data()) ) show_delete_object ^= 1 ;
			if( show_delete_object ) {

				for( SCClList* walker = ( SCClList* )tempClickObject.next ; walker ; walker = walker->next ) {
					gStudioWindow->deleteObject( ( H3Object* )walker->element ) ;
				}	
			}
		}
		//                             -- End

	    ImGui::EndChild() ;
	    ImGui::PopStyleVar() ;
	}
}

/*
 brief :
*/
void Editor::drawDetailsWindow ( iS32 type , void* ref )
{
	//	jelo committed start
	//ImGui::Begin( "Details" ) ;
	//	jelo committed end

	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	if( type == 1 ) {  //相机

		if( gStudioWindow->currentObject != NULL ){
			if( closeObjectRotate ) 
			Editor::drawCameraRotateEditor( gStudioWindow->camera ) ;
		}
		if ( gStudioWindow->camera ) 
			Editor::drawCameraMoveEditor( gStudioWindow->camera ) ;		
		if( gStudioWindow->camera ) 
			Editor::drawCameraSetEditor( gStudioWindow->camera ) ;

	} else if( type == 2 ) {  //环境光
		//if( ImGui::TreeNode( H3UtilGbk2Utf8( string( "环境光" ) ).data( ) ) ){
			ImGui::Text( H3UtilGbk2Utf8( string( "Ambitient Light" ) ).data( ) ) ;
			ImGui::Spacing( );
			ImGui::Spacing( );

			H3AmbientLight* h3AmbientLight = gStudioWindow->h3AmbientLight ;
		    static ImVec4 ambientColor = ImColor( h3AmbientLight->color.x , h3AmbientLight->color.y , h3AmbientLight->color.z , 1.0f ) ;
		    ImGui::Spacing( );
			ImGui::Text( H3UtilGbk2Utf8( string( "Color" ) ).data( ) ) ;
			ImGui::ColorEdit3( "##editambient" , ( float* )&ambientColor ) ;
			h3AmbientLight->setColor( H3Vector3f( ambientColor.x , ambientColor.y , ambientColor.z ) ) ;

			//	jelo comitted start
			//ImGui::Spacing( );
			//static float intensity = 0.5 ;
			//ImGui::Text( H3UtilGbk2Utf8( string( "强度" ) ).data( ) ) ;
			//if( ImGui::DragFloat( H3UtilGbk2Utf8( string( "X轴" ) ).data() , &intensity , 0.01f ) ) {
			//	h3AmbientLight->intensity = intensity ;
			//}
			//	//	jelo comitted end
            //ImGui::TreePop();
		//}
			  
	} else if ( type == 3 ) {  //平行光

		H3DirectionalLight* h3DirectionalLight = (H3DirectionalLight*)ref ;
        //if( ImGui::TreeNode( H3UtilGbk2Utf8( string( "平行光" ) ).data( ) ) ){
			ImGui::Text( H3UtilGbk2Utf8( string( "Directional Light" ) ).data( ) ) ;
			ImGui::Spacing( );  
			ImGui::Spacing( );
			  
			static ImVec4 directionColor = ImColor( h3DirectionalLight->color.x , h3DirectionalLight->color.y , h3DirectionalLight->color.z , 1.0f ) ;
			ImGui::Spacing( );
			ImGui::Text( H3UtilGbk2Utf8( string( "Color" ) ).data( ) );
			ImGui::ColorEdit3( "##editdirectional" , ( float* )&directionColor ) ;
			h3DirectionalLight->setColor( H3Vector3f( directionColor.x , directionColor.y , directionColor.z ) ) ;

			ImGui::Text( H3UtilGbk2Utf8( string( "Direction" ) ).data() ) ;
			ImGui::Columns( 3 , "mixed" ) ;
			ImGui::Separator( ) ;  
			ImGui::Spacing( ) ;
			ImGui::Spacing( ) ;
		
			ImGui::DragFloat( "x" , &h3DirectionalLight->direction.x , 0.01f ) ;
		
			ImGui::Spacing( ) ;
			ImGui::NextColumn( ) ;
			ImGui::Spacing( ) ;
			ImGui::Spacing( ) ;
			
			ImGui::DragFloat( "y", &h3DirectionalLight->direction.y , 0.01f ) ;
		
			ImGui::Spacing( ) ;
			ImGui::NextColumn( ) ;
			ImGui::Spacing( ) ;
			ImGui::Spacing( ) ;
			
			ImGui::DragFloat( "z" , &h3DirectionalLight->direction.z , 0.01f ) ;

			ImGui::NextColumn( ) ;
			ImGui::Columns( 1 ) ;
			ImGui::Separator( ) ;

			ImGui::Spacing( );
			static float intensity = 0.5 ;
			ImGui::Text( H3UtilGbk2Utf8( string( "Directional Light" ) ).data( ) ) ;
			if( ImGui::DragFloat( H3UtilGbk2Utf8( string( "Intensity" ) ).data() , &intensity , 0.01f ) ) {
				h3DirectionalLight->intensity = intensity ;
			}

           // ImGui::TreePop();
        //}

	} else if ( type == 4 ) {  // 物体
		if( ref ){
			if( closeObjectRotate ) 
				Editor::drawObjectRotateEditor( (H3Object*)ref ) ;
			Editor::drawObjectScaleEditor( (H3Object*)ref ) ;
			Editor::drawObjectMaterialEditor ( (H3Object*)ref ) ;
			Editor::drawObjectMoveEditor( (H3Object*)ref ) ;
		}
		H3Object* object = (H3Object*)ref ;
		SodaObjectInfo* objectInfo = gStudioWindow->selectObjectInfo( object ) ;
		H3Mesh* mesh = (H3Mesh*)object->getMesh() ;
		iS32 meshTotal = mesh->meshGroupList.size() ;
		
		ImGui::Spacing( ) ;
		ImGui::Spacing( ) ;
		ImGui::Spacing( ) ;
		ImGui::Spacing( ) ;
		ImGui::Spacing( ) ;

		//设置scroll bar位置
		extern ImGuiWindow* FindWindowByNameJelo(const char* name) ;
		ImGuiWindow* window = FindWindowByNameJelo("Details");
		bool isNodeOpen = false ;
	
		// 选中mesh优先显示
		if( gStudioWindow->currentSelectedMeshId > -1 ) {
			
			ImGui::Separator( ) ;
			ImGui::Text( H3UtilGbk2Utf8( string( "Selected" ) ).data( ) ) ;
			ImGui::Spacing( ) ;
			ImGui::Spacing( ) ;			
			SodaMesh* meshInfo = gStudioWindow->selectMeshInfo( objectInfo->objectIndex , gStudioWindow->currentSelectedMeshId ) ;

			//	jelo comitted end

			ImGui::Text( H3UtilGbk2Utf8( string( "Color" ) ).data( ) ) ;
			
			static ImVec4 meshColor = ImColor( 1.0f , 1.0f , 1.0f , 1.0f ) ;
			meshColor.x = meshInfo->color.red ;
			meshColor.y = meshInfo->color.green ;
			meshColor.z = meshInfo->color.blue ;
		    ImGui::Spacing( );
			ImGui::ColorEdit3( "##editambient" , ( float* )&meshColor ) ;
			meshInfo->color.red = meshColor.x ;
			meshInfo->color.green = meshColor.y ;
			meshInfo->color.blue = meshColor.z ;
		
			//add for temporary     -- Begin
			gStudioWindow->currentSelectedObject = object ;
			//                      -- End

			H3Mesh* mesh = (H3Mesh* )gStudioWindow->currentSelectedObject->getMesh () ;
			H3MeshGroup* meshGroup = mesh->getMeshGroup( gStudioWindow->currentSelectedMeshId ) ;
			meshGroup->material->addColor ( H3_MATERIAL_COLOR_AMBIENT , &H3Vector3f(meshInfo->color.red,meshInfo->color.green,meshInfo->color.blue) ) ;


		}


	}

	//	jelo committed start
	//ImGui::End( ) ;
	//	jelo committed end
}

/*
 brief :
*/
void Editor::drawObjectRotateEditor( H3Object* object )
{
	static float objectX = 0.0f ;
    static float objectY = 0.0f ;
	static float objectZ = 0.0f ;
	static float objecTemp = 0.0f ;
	bool show_object_reset = false ;

	if( 1 == gStudioWindow->isCurrentObjectNull( object ) )
		return ;

	if ( -1 == gStudioWindow->currentSelectedMeshId ) 
		return ;

	SodaObjectInfo* objectInfo = gStudioWindow->selectObjectInfo( object ) ;

	ImGui::Text( H3UtilGbk2Utf8( string( "Object Rotate" ) ).data( ) ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "x axis" ) ).data( ) , &objectX , 0.0f , 360.0f ) ){
		object->rotate( H3Vector3f( 1.0 , 0.0 , 0.0 ) , objectX - objecTemp ) ;

		objecTemp = objectX ;
	}
	   ImGui::Spacing( ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "y axis" ) ).data( ) , &objectY , 0.0f , 360.0f ) ){
		object->rotate( H3Vector3f( 0.0 , 1.0 , 0.0 ) , objectY - objecTemp ) ;
		objecTemp = objectY ;
	}
	ImGui::Spacing( ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "z axis" ) ).data( ) , &objectZ , 0.0f , 360.0f ) ){
		object->rotate( H3Vector3f( 0.0 , 0.0 , 1.0 ) , objectZ - objecTemp ) ;
		objecTemp = objectZ ;
	}
	ImGui::Spacing( ) ;

}

/*
 brief :
*/
void Editor::drawObjectScaleEditor( H3Object* object )
{

	ImGui::Text( H3UtilGbk2Utf8( string( "Object Scale" ) ).data() ) ;
	ImGui::Columns( 3 , "mixed" ) ;
	ImGui::Separator( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	SodaObjectInfo* objectInfo = gStudioWindow->selectObjectInfo( object ) ;

	static float scaleX = objectInfo->scale.x ;
	static float scaleY = objectInfo->scale.y ;
	static float scaleZ = objectInfo->scale.z ;

	if( ImGui::DragFloat( H3UtilGbk2Utf8( string( "X Axis" ) ).data() , &scaleX , 0.05f ) ) {
		object->setScale( H3Vector3f( scaleX , scaleY , scaleZ ) ) ;
		objectInfo->scale.x = scaleX ;
		objectInfo->scale.y = scaleY ;
		objectInfo->scale.z = scaleZ ;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	
	if( ImGui::DragFloat( H3UtilGbk2Utf8( string( "Y Axis" ) ).data() , &scaleY , 0.05f ) ) {
		object->setScale( H3Vector3f( scaleX , scaleY , scaleZ ) ) ;
		objectInfo->scale.x = scaleX ;
		objectInfo->scale.y = scaleY ;
		objectInfo->scale.z = scaleZ ;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	
	if( ImGui::DragFloat( H3UtilGbk2Utf8( string( "Z轴" ) ).data() , &scaleZ , 0.05f ) ) {
		object->setScale( H3Vector3f( scaleX , scaleY , scaleZ ) ) ;
		objectInfo->scale.x = scaleX ;
		objectInfo->scale.y = scaleY ;
		objectInfo->scale.z = scaleZ ;
	}

	ImGui::NextColumn( ) ;
	ImGui::Columns( 1 ) ;
	ImGui::Separator( ) ;
}

/*
 brief :
*/
void Editor::drawObjectMaterialEditor( H3Object* object )
{
	static float objectX = 0.0f ;
    static float objectY = 0.0f ;
	static float objectZ = 0.0f ;

	static float specobjectX = 0.0f ;
    static float specobjectY = 0.0f ;
	static float specobjectZ = 0.0f ;

	static float specularIntensity = 0.0f ;
	static float shininess = 0.0f ;
	static float sss = 0.8 ;
	static float fresnel = 0.5f ;

	bool show_object_reset = false ;

	if( 1 == gStudioWindow->isCurrentObjectNull( object ) )
		return ;

	if ( -1 == gStudioWindow->currentSelectedMeshId ) 
		return ;

	SodaObjectInfo* objectInfo = gStudioWindow->selectObjectInfo( object ) ;

	SodaMesh* meshInfo = gStudioWindow->selectMeshInfo( objectInfo->objectIndex , gStudioWindow->currentSelectedMeshId ) ;

	specularIntensity = meshInfo->specularIntensity ;
	shininess = meshInfo->shininess ;
	sss = meshInfo->sss ;
	fresnel = meshInfo->fresnel ; 

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Specular Intensity" ) ).data( ) , &specularIntensity , 0.0f , 50.0f ) ){
		H3Mesh* mesh = (H3Mesh* )object->getMesh () ;
		H3MeshGroup* meshGroup = mesh->getMeshGroup ( gStudioWindow->currentSelectedMeshId ) ;
		H3Material* material = meshGroup->material ;
		material->addFloat ( H3_MATERIAL_SPECULARINTENSITY , specularIntensity ) ;
		meshInfo->specularIntensity = specularIntensity ;
	}

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Roughness" ) ).data( ) , &shininess , 0.0f , 1.0f ) ){
		H3Mesh* mesh = (H3Mesh* )object->getMesh () ;
		H3MeshGroup* meshGroup = mesh->getMeshGroup ( gStudioWindow->currentSelectedMeshId ) ;
		H3Material* material = meshGroup->material ;
		material->addFloat ( H3_MATERIAL_SHININESS , shininess ) ;
		meshInfo->shininess = shininess ;
	} 

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Subsurface Scatter" ) ).data( ) , &sss , 0.0f , 10.0f ) ){
		H3Mesh* mesh = (H3Mesh* )object->getMesh () ;
		H3MeshGroup* meshGroup = mesh->getMeshGroup ( gStudioWindow->currentSelectedMeshId ) ;
		H3Material* material = meshGroup->material ;
		material->addFloat ( H3_MATERIAL_SSS , sss ) ; 
		meshInfo->sss = sss ;
	} 

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Fresnel" ) ).data( ) , &fresnel , 0.0f , 1.0f ) ){
		H3Mesh* mesh = (H3Mesh* )object->getMesh () ;
		H3MeshGroup* meshGroup = mesh->getMeshGroup ( gStudioWindow->currentSelectedMeshId ) ;
		H3Material* material = meshGroup->material ;
		material->addFloat ( H3_MATERIAL_FRESNEL , fresnel ) ; 
		meshInfo->fresnel = fresnel ;
	}  

}

/*
 brief :
*/
void Editor::drawObjectMoveEditor( H3Object* object )
{	

	ImGui::Text( H3UtilGbk2Utf8( string( "Object Location" ) ).data() ) ;
	ImGui::Columns( 3 , "mixed" ) ;
	ImGui::Separator( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	H3Vector3f position = object->getPosition( ) ;

	if( ImGui::DragFloat( "x" , &position.x , 0.5f ) ) {
		object->setPosition( position ) ;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	
	position = object->getPosition( ) ;
	if( ImGui::DragFloat( "y", &position.y , 0.5f ) ) {
		object->setPosition( position ) ;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	
	position = object->getPosition( ) ;
	if( ImGui::DragFloat( "z" , &position.z , 0.5f ) ) {
		object->setPosition( position ) ;
	}

	ImGui::NextColumn( ) ;
	ImGui::Columns( 1 ) ;
	ImGui::Separator( ) ;
}

/*
 brief :
*/
void Editor::drawObjectBlendShapeEditor ( H3Object* object ) 
{
	static iF32 weight = 0.0f ;
	ImGui::Text( H3UtilGbk2Utf8( string( "Blend Shape" ) ).data() ) ;
	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Weight" ) ).data() , &weight , 0.0f , 1.0f ) ) {
		gStudioWindow->setBlendShapeWeight ( weight ) ;
	}
}

/*
 brief :
*/
void Editor::drawCameraRotateEditor( H3Camera* camera )
{
	static float cameraX = 0.0f ;
	static float cameraY = 0.0f ;
	static float cameraZ = 0.0f ;
	static float fov = 35.0f ;
	static float left = 0.0f ;
	static float ahead = 0.0f ;
	static float cameraMove = 0.0f ;
	static float cameraRotate = 0.0f ;
	bool show_camera_reset = false ;

	ImGui::Text( H3UtilGbk2Utf8( string( "Camera FOV" ) ).data() ) ;
	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "FOV" ) ).data() , &fov , 0.0f , 180.0f ) ) {
		camera->initProjection( fov , (iF32)gStudioWindow->width / (iF32)gStudioWindow->height , 0.1f , 10000.0f ) ;
	}

	ImGui::Text( H3UtilGbk2Utf8( string( "Camera Roate" ) ).data() ) ;
	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Around X axis" ) ).data() , &cameraX , 0.0f , 360.0f ) ) {
		camera->rotate( H3Vector3f( 1.0f , 0.0f , 0.0f ) , cameraX - cameraRotate ) ;
		gStudioWindow->currentObject->rotate( H3Vector3f( 1.0f , 0.0f , 0.0f ) , cameraX - cameraRotate ) ;
		cameraRotate = cameraX ;
	}

	ImGui::Spacing( ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Around Y axis" ) ).data() , &cameraY , 0.0f , 360.0f ) ) {
		camera->rotate( H3Vector3f( 0.0f , 1.0f , 0.0f ) , cameraY - cameraRotate ) ;
		gStudioWindow->currentObject->rotate( H3Vector3f( 0.0f , 1.0f , 0.0f ) , cameraY - cameraRotate ) ;
		cameraRotate = cameraY ;
	}

	ImGui::Spacing( ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Around Z axis" ) ).data() , &cameraZ , 0.0f , 360.0f ) ) {
		camera->rotate( H3Vector3f( 0.0f , 0.0f , 1.0f ) , cameraZ - cameraRotate ) ;
		gStudioWindow->currentObject->rotate( H3Vector3f( 0.0f , 0.0f , 1.0f ) , cameraZ - cameraRotate ) ;
		cameraRotate = cameraZ ;
	}

	ImGui::Spacing( ) ;
}

/*
 brief :
*/
void Editor::drawCameraMoveEditor( H3Camera* camera )
{
	static float cameraX = 0.0f ;
	static float cameraY = 0.0f ;
	static float cameraZ = 0.0f ;
	static float left = 0.0f ;
	static float ahead = 0.0f ;
	static float cameraMove = 0.0f ;
	static float cameraRotate = 0.0f ;
	bool show_camera_reset = false ;

	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	ImGui::Text( H3UtilGbk2Utf8( string( "Camera Translate" ) ).data() ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Lef or Right" ) ).data() , &left , -50.0f , 50.0f ) ) {
		H3Vector3f direction = camera->getLeft( ) ;
		direction = direction.mul( left-cameraMove ) ;
		H3Vector3f position = camera->getPosition( ) ;
		position = position.add( direction ) ;
		camera->setPosition( position ) ;
		cameraMove = left ;
	}

	ImGui::Spacing( ) ;

	if( ImGui::SliderFloat( H3UtilGbk2Utf8( string( "Forward or Backward" ) ).data() , &ahead , -50.0f , 50.0f ) ) {
		H3Vector3f direction = camera->getForward( ) ;
		direction = direction.mul( ahead-cameraMove ) ;
		H3Vector3f position = camera->getPosition( ) ;
		position = position.add( direction ) ;
		camera->setPosition( position ) ;
		cameraMove = ahead ;
	}

	ImGui::Spacing( ) ;
}

/*
 brief :
*/
void Editor::drawCameraSetEditor( H3Camera* camera )
{
	ImGui::Text( H3UtilGbk2Utf8( string( "Camera Position" ) ).data() ) ;
	ImGui::Columns( 3 , "mixed" ) ;
	ImGui::Separator( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	H3Vector3f position = camera->getPosition( ) ;

	if( ImGui::DragFloat( "X" , &position.x , 0.5f ) ) {
		camera->setPosition( position ) ;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	position = camera->getPosition( ) ;
	if( ImGui::DragFloat( "Y" , &position.y , 0.5f ) ) {
		camera->setPosition( position ) ;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	position = camera->getPosition( ) ;
	if( ImGui::DragFloat( "Z" , &position.z , 0.5f ) ) {
		camera->setPosition ( position ) ;
	}

	ImGui::NextColumn( ) ;
	ImGui::Columns( 1 ) ;
	ImGui::Separator( ) ;

	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	ImGui::Text( H3UtilGbk2Utf8( string( "Camera Direction" ) ).data() ) ;
	ImGui::Columns( 3 , "mixed" ) ;
	ImGui::Separator( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	H3Vector3f direction = camera->getForward() ;

	if( ImGui::DragFloat( "X " , &direction.x , 0.5f ) ) {
		;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	direction = camera->getForward() ;
	if( ImGui::DragFloat( "Y " , &direction.y , 0.5f ) ) {
		;
	}

	ImGui::Spacing( ) ;
	ImGui::NextColumn( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

	direction = camera->getForward() ;
	if( ImGui::DragFloat( "Z " , &direction.z , 0.5f ) ) {
		;
	}

	ImGui::NextColumn( ) ;
	ImGui::Columns( 1 ) ;
	ImGui::Separator( ) ;
}

#ifdef WIN32
/*
 brief :
*/
void Editor::ShowExampleMenuFile( )
{
	if( ImGui::MenuItem( H3UtilGbk2Utf8( string( "New" ) ).data() ) ) {

		gStudioWindow->clean () ;
		gStudioWindow->camera = MMC_NEW H3Camera() ;
		gStudioWindow->camera->setPosition( H3Vector3f( 0 , 0 , 0 ) ) ;
		gStudioWindow->addComponent( gStudioWindow->camera ) ;

		if ( StudioCameraType_Orthographics == gStudioWindow->lastCameraType ) {
			gStudioWindow->camera->initOrtho( -10.0 , 10.0 , 10.0 , -10.0 , 0.0 , 10000.0 ) ;
			gStudioWindow->camera->rotate(H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;	
			if ( gStudioWindow->currentObject ) {
				//	jelo s
				//currentObject->rotate (H3Vector3f( 1.0f , 0.0f , 0.0f ), 90.f) ;
				//	jelo end
			}
		} else if ( StudioCameraType_Perspective == gStudioWindow->lastCameraType ) {
			gStudioWindow->camera->initProjection( 35.0f , (iF32)gStudioWindow->width / (iF32)gStudioWindow->height , 0.1f , 10000.0f ) ;
		}
		 
		H3AmbientLight* h3AmbientLight = MMC_NEW H3AmbientLight( H3Vector3f( 0.5f , 0.5f , 0.5f ) ) ;
		gStudioWindow->h3AmbientLight = h3AmbientLight ;
		gStudioWindow->addComponent ( h3AmbientLight ) ;
		H3LightAtom* atom1 = ( H3LightAtom* ) H3MemoryMalloc( sizeof( H3LightAtom ) , __FILE__ , __LINE__ ) ;
		H3ASSERT( NULL != atom1 ) ;
		atom1->type = H3COMPONENT_TYPE_AMBIENT_LIGHT ;
		atom1->lightRef = ( void* ) gStudioWindow->h3AmbientLight ;
		SCClListInsert( &gStudioWindow->lightList , atom1 ) ;

	}
	if( ImGui::MenuItem( H3UtilGbk2Utf8( string( "Open" ) ).data() , "Ctrl+O" ) ) {
		
		//20160303
		iS8 filepath[MAX_PATH] = {0} ;
		strcpy( filepath , getFilePath(0) ) ;
		gStudioWindow->loadFile( filepath ) ;
	}
	if( ImGui::MenuItem( H3UtilGbk2Utf8( string( "Save As" ) ).data() , "Ctrl+S" ) ) {

	}
	ImGui::Separator( ) ;

	if( ImGui::MenuItem( H3UtilGbk2Utf8( string( "Exit" ) ).data() , "Alt+F4" ) ) {
		//gStudioWindow->clean () ;
		//SodaMapFileLoader::clean () ;
	}
}

/*
 brief :
*/
void Editor::ShowExampleAppMainMenuBar( )
{
	static bool show_app_about = false ;
	//	jelo comitted start
	//if( ImGui::BeginMainMenuBar( ) )
	//	jelo comitted end
	{
		if( ImGui::BeginMenu( H3UtilGbk2Utf8( string( "File" ) ).data() ) )
		{
			ShowExampleMenuFile( ) ;
			ImGui::EndMenu( ) ;
		}
		if( ImGui::BeginMenu( H3UtilGbk2Utf8( string( "Help" ) ).data( ) ) )
		{
			ImGui::MenuItem( H3UtilGbk2Utf8( string( "About Cacu Facial Studio" ) ).data() , NULL , &show_app_about ) ;
			ImGui::EndMenu( ) ;
		}

		if( show_app_about )
		{
			ImGui::Begin( "About Soda 3D Studio" , &show_app_about ) ;
			ImGui::SetNextWindowSize( ImVec2( 200 , 200 ) ) ;
			ImGui::Text( "Soda 3D Studio %s" , H3Delegate::getVersion( ) ) ;
			ImGui::Text( H3UtilGbk2Utf8( string( "Render %s" ) ).data( ) , H3Delegate::getVersion( ) ) ;
			ImGui::Separator( ) ;
			ImGui::Text(H3UtilGbk2Utf8( string( "QDL Inc. （C）All Rights Reserved" ) ).data( ) ) ;
			ImGui::End( ) ;
		}
		//	jelo comitted start
		//ImGui::EndMainMenuBar( ) ;
		//	jelo comitted end
	}
}
#endif

