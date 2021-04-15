
#include "ImHierarchyWindow.h"
#include "StudioWindow.h"

extern StudioWindow* gStudioWindow ;

void ImHierarchyWindow::OnGui ()
{
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
	if( ImGui::RadioButton( H3UtilGbk2Utf8( string( "Camera" ) ).data( ) , &e , 0 ) ) {
		show_cameras ^= 1 ;
		show_objects = false ;
		show_lights = false ;
	}
	ImGui::SameLine();

	if( ImGui::RadioButton( H3UtilGbk2Utf8( string( "Light" ) ).data( ) , &e , 1 ) ) {
		show_lights ^= 1 ;
		show_objects = false ;
		show_cameras = false ;
	}
	ImGui::SameLine();

	if( ImGui::RadioButton( H3UtilGbk2Utf8( string( "Object" ) ).data( ) , &e , 2 ) ) {
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

	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;
	ImGui::Spacing( ) ;

}

void ImHierarchyWindow::OnMenu ()
{
	Editor::ShowExampleAppMainMenuBar () ;
}