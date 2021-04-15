
#include "ImDetailWindow.h"
#include "StudioWindow.h"
#include "Editor.h"

extern StudioWindow* gStudioWindow ; 

void ImDetailWindow::OnGui ()
{
	Editor::drawDetailsWindow( gStudioWindow->gDetailsType , gStudioWindow->gDetailsRef ) ;
	Editor::drawObjectBlendShapeEditor ( (H3Object* )gStudioWindow->gDetailsRef ) ;
}

void ImDetailWindow::OnMenu ()
{
}
