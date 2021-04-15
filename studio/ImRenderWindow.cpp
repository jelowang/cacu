
#include "ImRenderWindow.h"
#include "StudioWindow.h"
#include "H3Delegate.h"

extern StudioWindow* gStudioWindow ;

void ImRenderWindow::OnGui ()
{
	static ImVec2 lastContentSize ;

	extern ImVec2 gRenderWindowSize ;
	extern ImVec2 gRenderWindowPos ;
	extern iU32 gRenderWindowHeightPadding ;
	extern iU32 gRenderWindowWidthPadding ;

	gRenderWindowSize = ImGui::GetContentRegionAvail();
	ImGuiWindow* window = ImGui::GetCurrentWindow() ;
	gRenderWindowPos = window->Pos ;
	if ( lastContentSize.x != gRenderWindowSize.x && lastContentSize.y != gRenderWindowSize.y ) {
		H3Delegate::getInstance()->resize ( (iU32)gRenderWindowSize.x , (iU32)gRenderWindowSize.y ) ;
	}
	ImGui::Image ( (void*)H3Delegate::getInstance()->getRenderTexture() , gRenderWindowSize , ImVec2(0,1) , ImVec2(1,0) ) ;
	lastContentSize = gRenderWindowSize ;
}

void ImRenderWindow::OnMenu ()
{
}
