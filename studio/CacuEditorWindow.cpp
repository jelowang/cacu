#ifdef WIN32
#include <windows.h>
#include <commdlg.h>
#include "H3MeshRender.h"
#include "H3Object.h"
#include "CacuEditorWindow.h"
#include "CacuParticleEditor.h"
#include "CacuEmitterEditor.h"
#include "CacuMainScene.h"
#include "H3Image.h"
#include "H3Delegate.h"
#include "H3DrawPrimitives.h"
#include "HpTexture2D.h"
#include "HpParticleSystem.h"

#define CACU_VERSION "0.1.0"

static void* m_HpParticleSystemExampleSeek ;
static HpParticleSystem* gHpParticleSystem ;	

ImVec2 mousePosition ;

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

CacuEditorWindow::CacuEditorWindow(const char* title) 
{
	SetTitle(title);
	//CacuEmitterEditor::init () ;
	//gHpParticleSystem = CacuEmitterEditor::getCurrentSystem () ;
}

CacuEditorWindow::~CacuEditorWindow () 
{
}

/*
 brief :
*/

void CacuEditorWindow::OnGui() 
{

	//CacuParticleEditor::showFpsOverlay () ;
	//CacuParticleEditor::drawParticleEditor ( CacuEmitterEditor::getCurrentEmitter() , &CacuEmitterEditor::getCurrentEmitter()->m_orignal_data ) ;
	
	//CacuEmitterEditor::drawEmitterEditor ( this  ) ;	
	//gHpParticleSystem->draw ( 0.016f ) ;
	
	static ImVec2 lastContentSize ;

	extern ImVec2 gRenderWindowSize ;
	extern iU32 gRenderWindowHeightPadding ;
	extern iU32 gRenderWindowWidthPadding ;

	gRenderWindowSize = ImGui::GetContentRegionAvail();
	if ( lastContentSize.x != gRenderWindowSize.x && lastContentSize.y != gRenderWindowSize.y ) {
		H3Delegate::getInstance()->resize ( (iU32)gRenderWindowSize.x , (iU32)gRenderWindowSize.y ) ;
	}
	ImGui::Image ( (void*)H3Delegate::getInstance()->getRenderTexture() , gRenderWindowSize , ImVec2(0,1) , ImVec2(1,0) ) ;
	lastContentSize = gRenderWindowSize ;
	
}

/*
 brief :
*/
void CacuEditorWindow::OnMenu() 
{

    static bool show_app_about = false;

    {
        if (ImGui::BeginMenu(H3UtilGbk2Utf8("File").data()))
        {

			iS8 filepath[MAX_PATH] = {0} ;
			iS32 length = 0 ;

			TCHAR szBuffer[MAX_PATH] = {0} ;
			TCHAR defaultPath[MAX_PATH] = {0} ;
			GetCurrentDirectory ( sizeof(defaultPath) , defaultPath ) ;
	
			OPENFILENAME ofn = {0} ;
			memset ( &ofn , 0 , sizeof(ofn) ) ;
			ofn.lStructSize = sizeof(ofn) ;
			ofn.hwndOwner = NULL ;
			ofn.lpstrFilter = L"*.cacu" ;
			ofn.lpstrInitialDir = (LPCWSTR)H3UtilGbk2Utf8 ( std::string("d:\\") ).data() ;
			ofn.lpstrFile = szBuffer ;
			ofn.nMaxFile = sizeof(szBuffer) / sizeof( *szBuffer ) ;
			ofn.nFilterIndex = 0 ;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST ;

			if (ImGui::MenuItem(H3UtilGbk2Utf8("New").data())) {
				//SCClListClear ( &gEmitterList ) ;
			}
			if (ImGui::MenuItem(H3UtilGbk2Utf8("Import").data(), "Ctrl+O")) {
				//bool bSEL = GetOpenFileName ( &ofn ) ;
				//SetCurrentDirectory ( defaultPath ) ;
				//length = WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , NULL , 0 , NULL , NULL ) ;
				//WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , filepath , length , NULL , NULL ) ;
				//SCHalSetAbsoluteMode ( true ) ;
				//HpCacuFileLoad ( filepath , &gEmitterList ) ;
				//SCHalSetAbsoluteMode ( false ) ;

			}
			if (ImGui::MenuItem(H3UtilGbk2Utf8("Save As").data())) {

				//bool bSEL = GetSaveFileName ( &ofn ) ;
				//SetCurrentDirectory ( defaultPath ) ;
				//length = WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , NULL , 0 , NULL , NULL ) ;
				//WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , filepath , length , NULL , NULL ) ;
				//SCHalSetAbsoluteMode ( true ) ;
				//HpCacuFileSave ( filepath , &gEmitterList ) ;
				//SCHalSetAbsoluteMode ( false ) ;

			}
			ImGui::Separator();

			if (ImGui::MenuItem(H3UtilGbk2Utf8("Exit").data(), "Alt+F4")) {}

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(H3UtilGbk2Utf8("Windows").data()))
        {+
            ImGui::MenuItem(H3UtilGbk2Utf8("Object").data(), NULL, &show_app_about);
			ImGui::MenuItem(H3UtilGbk2Utf8("Details").data(), NULL, &show_app_about);
			ImGui::EndMenu();
        }

        if (ImGui::BeginMenu(H3UtilGbk2Utf8("Help").data()))
        {
            ImGui::MenuItem(H3UtilGbk2Utf8("About").data(), NULL, &show_app_about);
			ImGui::EndMenu();
        }



    }

	if (show_app_about)
    {

		iS8 buffer[512] = {0} ;
		

        ImGui::Begin("About Cacu Particle Studio", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
		sprintf ( buffer , "版本 %s", CACU_VERSION ) ;
		ImGui::Text(H3UtilGbk2Utf8(buffer).data());

		memset ( buffer , 0 , sizeof(buffer) ) ;
		sprintf ( buffer , "H3 %s", H3Delegate::getInstance()->getVersion() ) ;
		ImGui::Text(H3UtilGbk2Utf8(buffer).data());

        ImGui::Separator();

		ImGui::Text(H3UtilGbk2Utf8("由\"量子动力实验室\"研发").data());
		ImGui::Text(H3UtilGbk2Utf8("深圳市乐悠世纪科技有限公司（C）保留所有权利").data());		

        ImGui::End();
	
    }
}

#endif	//#ifdef WIN32