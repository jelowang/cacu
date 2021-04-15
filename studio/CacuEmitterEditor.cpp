
#ifdef WIN32
#include <windows.h>
#include <commdlg.h>
#include "imgui.h"
#include "h3Std.h"
#include "CacuEmitterEditor.h"
#include "CacuParticleEditor.h"
#include "H3Util.h"
#include "HpParticleSystem.h"
#include "HpTexture2D.h"
#include "HpCacu.h"
#include "H3Delegate.h"
#include "HpTextureCache.h"
#include "HpUtil.h"

extern int cacuMainWindowWidth ;
extern int cacuMainWindowHeight ;

static HpParticleSystem* gHpParticleSystem = 0 ;	
static SCClList gEmitterList = {0} ;
static HpEmitter gEt = {0} ;
static HpEmitter* gCurrentEmitter = &gEt ; 
static HpEmitter* gLastNewEmitter = 0 ;
static bool use3D = false ;
static bool textureLoop = false ;
static HpTexture2D* gQdlLogo = 0 ;

//namespace CacuEmitterEditor {

static void ShowHelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip(desc);
}

void init () 
{
	if ( 0 == gHpParticleSystem ) {
		gHpParticleSystem = new HpParticleSystem () ;
	}

	if ( 0 == gQdlLogo ) {
		gQdlLogo = MMC_NEW HpTexture2D() ;
		gQdlLogo->initWithPNGFile ( "textures/qdl.png" ) ;
		gQdlLogo->addRef () ;
	}

}

void clear () 
{
	if ( gHpParticleSystem )
		delete gHpParticleSystem ;

	if ( 0 == gQdlLogo ) {
		gQdlLogo->release () ;
	}

}

static void ShowMenuFile()
{

#ifdef WIN32
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

    if (ImGui::MenuItem(H3UtilGbk2Utf8("新建").data())) {
		SCClListClear ( &gEmitterList ) ;
	}
    if (ImGui::MenuItem(H3UtilGbk2Utf8("导入").data(), "Ctrl+O")) {
		bool bSEL = GetOpenFileName ( &ofn ) ;
		SetCurrentDirectory ( defaultPath ) ;
		length = WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , NULL , 0 , NULL , NULL ) ;
		WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , filepath , length , NULL , NULL ) ;
		SCHalSetAbsoluteMode ( true ) ;
		HpCacuFileLoad ( filepath , &gEmitterList ) ;
		SCHalSetAbsoluteMode ( false ) ;

	}
    if (ImGui::MenuItem(H3UtilGbk2Utf8("另存为").data())) {

		bool bSEL = GetSaveFileName ( &ofn ) ;
		SetCurrentDirectory ( defaultPath ) ;
		length = WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , NULL , 0 , NULL , NULL ) ;
		WideCharToMultiByte ( CP_ACP , 0 , szBuffer , -1 , filepath , length , NULL , NULL ) ;
		SCHalSetAbsoluteMode ( true ) ;
		HpCacuFileSave ( filepath , &gEmitterList ) ;
		SCHalSetAbsoluteMode ( false ) ;

	}
    ImGui::Separator();

    if (ImGui::MenuItem(H3UtilGbk2Utf8("退出").data(), "Alt+F4")) {}
#endif

}

void drawMainMenu ()
{

    static bool show_app_about = false;

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu(H3UtilGbk2Utf8("文件").data()))
        {
            ShowMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(H3UtilGbk2Utf8("帮助").data()))
        {
            ImGui::MenuItem(H3UtilGbk2Utf8("关于").data(), NULL, &show_app_about);
			ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

	if (show_app_about)
    {


		iS8 buffer[512] = {0} ;
		sprintf ( buffer , "渲染内核 %s", H3Delegate::getInstance()->getVersion() ) ;

        ImGui::Begin("About Cacu Studio", &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
		//ImGui::SetWindowPos ( ImVec2(cacuMainWindowWidth/2,cacuMainWindowHeight/2)) ;
		ImGui::Text(H3UtilGbk2Utf8("实时演算特效引擎").data(), H3Delegate::getInstance()->getVersion() );
        ImGui::Separator();

		//ImVec2 ClipSize(200, 200);
		//ImVec2 pos = ImGui::GetCursorScreenPos();
		//ImVec4 clip_rect(pos.x, pos.y, pos.x+ClipSize.x, pos.y+ClipSize.y);
		//ImGui::InvisibleButton("##dummy", ClipSize);
		//ImGui::GetWindowDrawList()->PushTextureID((ImTextureID* )gQdlLogo->getId() );
		//ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x+ClipSize.x,pos.y+ClipSize.y), ImColor(90,90,120,255));
		//ImGui::GetWindowDrawList()->AddImage ( (ImTextureID* )gQdlLogo->getId() , pos , ImVec2(pos.x+ClipSize.x,pos.y+ClipSize.y) ) ;
		//ImGui::GetWindowDrawList()->PopTextureID();

		ImGui::Text(H3UtilGbk2Utf8("由\"量子动力实验室\"研发").data());
		ImGui::Text(H3UtilGbk2Utf8("深圳市乐悠世纪科技有限公司（C）保留所有权利").data());		

        ImGui::End();
	
    }

}

HpParticleSystem* getCurrentSystem () 
{
	return gHpParticleSystem ;
}

HpEmitter* getCurrentEmitter () 
{
	return gCurrentEmitter ;
}

void* getEmitterList () 
{
	return &gEmitterList ;
}

void drawEmitterEditor ( CacuEditorWindow* window ) 
{
	static int totallTextureInputed = 0 ;
	bool stop = false ;
	gCurrentEmitter =&gEt ;
	static bool opened ;
    if (!ImGui::Begin(H3UtilGbk2Utf8("System Editor").data(), &opened))
    {
        ImGui::End();
        return;
    }

	ImGui::SameLine () ;
	if ( ImGui::Button(H3UtilGbk2Utf8("开始模拟").data()) ) {

		for ( SCClList* walker = gEmitterList.next ; walker ; walker = walker->next ) { 

			HpEmitter* tree = (HpEmitter* )walker->element ;
			void* seek = gHpParticleSystem->loadEmitter ( tree ) ;
			void* handle = gHpParticleSystem->createEmitter (seek) ; 
			gHpParticleSystem->playEmitter (handle) ;
			HpPoint point ;
			//point.x = 500.0 ;
			//point.y = -500.0 ;
			gHpParticleSystem->setEmitterPosition ( handle , point ) ;

		}
		
	}
	ImGui::SameLine () ;
	if ( ImGui::Button(H3UtilGbk2Utf8("停止模拟").data()) ) {
		gHpParticleSystem->stopEmitterAll () ;
	}
	ImGui::SameLine () ;

	if ( ImGui::Button(H3UtilGbk2Utf8("添加喷头").data()) ) {
		ImGui::OpenPopup(H3UtilGbk2Utf8("添加新喷头？").data());
		totallTextureInputed = 1 ;
	}

	int e = (int )use3D ;
	ImGui::SameLine () ;
	if ( ImGui::RadioButton(H3UtilGbk2Utf8("2D").data(), &e, 0) ) {
		//window->change2d () ;
	}
	ImGui::SameLine () ;
	if ( ImGui::RadioButton(H3UtilGbk2Utf8("3D").data(), &e, 1) ) {
		//window->change3d () ;
	}	
	use3D = (bool ) e ;

	ImGui::PushItemWidth(100);
	if (ImGui::BeginPopupModal(H3UtilGbk2Utf8("添加新喷头？").data(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		ImGui::PushItemWidth(100);
		ImGui::InputText("Name", gCurrentEmitter->m_name, sizeof(gCurrentEmitter->m_name));
		ImGui::PopItemWidth();
		for ( int count = 0 ; count < totallTextureInputed ; count ++ ) {

			ImGui::PushItemWidth(100);
			iS8 buffer[512] = {0} ;
			sprintf( buffer , "Texture , Frame : %d" , count ) ;
		    ImGui::InputText(buffer, gCurrentEmitter->m_hpeTexture.m_framelist[count].m_name, 
				sizeof(gCurrentEmitter->m_hpeTexture.m_framelist[count].m_name));
			 
			ImGui::SameLine () ; 
			sprintf( buffer , "duration : %d" , count ) ;
			ImGui::DragFloat(buffer, &gCurrentEmitter->m_hpeTexture.m_framelist[count].m_duration, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
			ImGui::PopItemWidth();
		}

	    if (ImGui::Button("OK", ImVec2(120,0))) { 

			HpEmitter* tree = (HpEmitter* ) MMC_MALLOC ( sizeof(HpEmitter) ) ;
			assert(tree) ;
			strcpy ( tree->m_name , gCurrentEmitter->m_name ) ;

			//	save frame's name
			for ( int count = 0 ; count < totallTextureInputed ; count ++ ) {
				strcpy ( tree->m_hpeTexture.m_framelist[count].m_name , gCurrentEmitter->m_hpeTexture.m_framelist[count].m_name ) ;
				tree->m_hpeTexture.m_framelist[count].m_duration = gCurrentEmitter->m_hpeTexture.m_framelist[count].m_duration ;	
			}

			SCClListInsert ( &gEmitterList , tree ) ;
			gLastNewEmitter = tree ;
			gCurrentEmitter = tree ;

			//	create frame's texture
			for ( int count = 0 ; count < totallTextureInputed ; count ++ ) {
				HpTexture2D* texture = 0 ;
				texture = HpTextureCache::getInstance()->getTexture ( 
					H3UtilString2Number(gLastNewEmitter->m_hpeTexture.m_framelist[count].m_name).getValue() ) ;

				if ( 0 == texture ) {
					texture = MMC_NEW HpTexture2D () ;
					bool backup = SCHalGetAbsoluteMode () ;
					SCHalSetAbsoluteMode ( false ) ;
					texture->initWithPNGFile ( gLastNewEmitter->m_hpeTexture.m_framelist[count].m_name ) ;
					SCHalSetAbsoluteMode ( backup ) ;
					HpTextureCache::getInstance()->addTexture (
						H3UtilString2Number(gLastNewEmitter->m_hpeTexture.m_framelist[count].m_name).getValue() ,
						texture 
					) ;

				}
				texture->addRef () ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_widthInPixels = texture->getContentSize().width ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_heightInPixels = texture->getContentSize().height ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_rect.x1 = 0 ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_rect.y1 = 0 ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_rect.x2 = gLastNewEmitter->m_hpeTexture.m_framelist[count].m_widthInPixels ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_rect.y2 = gLastNewEmitter->m_hpeTexture.m_framelist[count].m_heightInPixels ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_handle = (long long )texture ;
				gLastNewEmitter->m_hpeTexture.m_framelist[count].m_id = (long long )texture->getId () ;
				gLastNewEmitter->m_hpeTexture.m_totallFrame ++ ;
				 
			}

			ImGui::CloseCurrentPopup(); 
		}
	    ImGui::SameLine();
	    if (ImGui::Button("No", ImVec2(120,0))) { 
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SameLine();
	    if (ImGui::Button("ADD Texture", ImVec2(120,0))) {
			totallTextureInputed ++ ;
		}

	    ImGui::EndPopup();
	}

	CacuParticleEditor::enableParticleEditor ( false ) ;
	for ( SCClList* walker = gEmitterList.next ; walker && stop == false ; walker = walker->next ) { 

		HpEmitter* tree = (HpEmitter* )walker->element ;
		iS8 buffer[512] = {0} ;
		sprintf( buffer , "Tree %d" , walker->index ) ;

		if (ImGui::TreeNode(buffer , "%s" , tree->m_name)) {
			
			iS32 srcItem = 0 ;
			iS32 dstItem = 0 ;

			iU32 src[] = { GL_ZERO , GL_ONE , GL_SRC_COLOR , GL_ONE_MINUS_SRC_COLOR , GL_SRC_ALPHA ,
			GL_ONE_MINUS_SRC_ALPHA , GL_DST_ALPHA , GL_ONE_MINUS_DST_ALPHA } ;
			iU32 des[] = { GL_ZERO , GL_ONE , GL_SRC_COLOR , GL_ONE_MINUS_SRC_COLOR , GL_SRC_ALPHA ,
			GL_ONE_MINUS_SRC_ALPHA , GL_DST_ALPHA , GL_ONE_MINUS_DST_ALPHA } ;
			
			for ( ; srcItem < sizeof(src) / sizeof(iU32) ; srcItem++ ) 
				if ( src[srcItem] == tree->m_BlendFuncSrc ) 
					break ;

			for ( ; dstItem < sizeof(des) / sizeof(iU32) ; dstItem++ ) 
				if ( des[dstItem] == tree->m_BlendFuncDst ) 
					break ;
	
			ImGui::InputText(H3UtilGbk2Utf8("名称").data(), tree->m_name, sizeof(tree->m_name));
			ImGui::DragFloat(H3UtilGbk2Utf8(string("喷射开始时间")).data(), &tree->m_starttime, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
			ImGui::DragFloat(H3UtilGbk2Utf8(string("喷射时长")).data(), &tree->m_emittDuration, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
			ImGui::InputInt(H3UtilGbk2Utf8(string("喷射总粒子量")).data(), &tree->m_emittTotall);
			ImGui::DragFloat(H3UtilGbk2Utf8(string("角度")).data(), &tree->m_orignal_data.m_emitter.startAngle, 0.005f, 0.0f, 360.0f, "%1.3f") ;
			ImGui::DragFloat(H3UtilGbk2Utf8(string("角度变化范围")).data(), &tree->m_orignal_data.m_emitter.angleVariance, 0.005f, 0.0f, 360.0f, "%1.3f") ;
			ImGui::Separator();

	        ImGui::Combo(H3UtilGbk2Utf8("源混合函数").data(), &srcItem, "GL_ZERO\0GL_ONE\0GL_SRC_COLOR\0GL_ONE_MINUS_SRC_COLOR\0GL_SRC_ALPHA\0GL_ONE_MINUS_SRC_ALPHA\0GL_DST_ALPHA\0GL_ONE_MINUS_DST_ALPHA\0\0");
	        ImGui::Combo(H3UtilGbk2Utf8("目标混合函数").data(), &dstItem, "GL_ZERO\0GL_ONE\0GL_SRC_COLOR\0GL_ONE_MINUS_SRC_COLOR\0GL_SRC_ALPHA\0GL_ONE_MINUS_SRC_ALPHA\0GL_DST_ALPHA\0GL_ONE_MINUS_DST_ALPHA\0\0");

			tree->m_BlendFuncSrc = src[srcItem] ;
			tree->m_BlendFuncDst = src[dstItem] ;

			ImGui::Separator();
			ImGui::SameLine () ;
			if ( ImGui::RadioButton(H3UtilGbk2Utf8("Once").data(), &tree->m_hpeTexture.frameLoop, 0) ) {
				tree->m_hpeTexture.frameLoop = 0 ;
			}
			ImGui::SameLine () ;
			if ( ImGui::RadioButton(H3UtilGbk2Utf8("Loop").data(), &tree->m_hpeTexture.frameLoop, 1) ) {
				tree->m_hpeTexture.frameLoop = 1 ;
			}	

			for ( int count = 0 ; count < tree->m_hpeTexture.m_totallFrame ; count ++ ) {
				ImGui::Separator();

				iS8 buffer[512] = {0} ;
				sprintf( buffer , "Texture , Frame : %d" , count ) ;

				ImGui::InputText(buffer, tree->m_hpeTexture.m_framelist[count].m_name, sizeof(tree->m_hpeTexture.m_framelist[count].m_name));
				sprintf( buffer , "duration : %d" , count ) ;
				ImGui::DragFloat(buffer, &tree->m_hpeTexture.m_framelist[count].m_duration, 0.005f, 0.0f, 1000.0f, "%1.3f") ;
			}
			if ( tree->m_hpeTexture.m_totallFrame > 1 ) {
				if ( ImGui::Button(H3UtilGbk2Utf8("删除帧").data()) ) {
					//	have memory leaking here 
					tree->m_hpeTexture.m_totallFrame -- ;
				}
			}

			ImGui::Separator();
			bool boolen = tree->m_visible ;
			ImGui::Checkbox(H3UtilGbk2Utf8("选取").data(),&boolen);
			tree->m_visible = boolen ;
			if ( tree->m_visible ) {
				gCurrentEmitter = tree ;
				CacuParticleEditor::enableParticleEditor ( true ) ;
			} 
			ImGui::SameLine () ;
			
			int e = (int ) tree->m_visible ;
			ImGui::RadioButton(H3UtilGbk2Utf8("激活").data(), &e, 1); ImGui::SameLine () ;
			ImGui::RadioButton(H3UtilGbk2Utf8("挂起").data(), &e, 0); 
			
			if ( 1 == e ) {
				tree->m_visible = true ;
			} else {
				tree->m_visible = false ;
			}
			ImGui::SameLine();
			if ( ImGui::Button(H3UtilGbk2Utf8("删除喷头").data()) ) {
				ImGui::OpenPopup(H3UtilGbk2Utf8("删除喷头？").data());
			}

			if (ImGui::BeginPopupModal(H3UtilGbk2Utf8("删除喷头？").data(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{

				if (ImGui::Button("OK", ImVec2(120,0))) { 
					gCurrentEmitter = tree ;
					HpTexture2D* texture = (HpTexture2D* )gCurrentEmitter->m_hpeTexture.m_framelist[0].m_handle ;
					texture->release () ;
					SCClListDelete ( &gEmitterList , gCurrentEmitter ) ;
					if ( gCurrentEmitter->m_data ) 
						MMC_FREE(gCurrentEmitter->m_data) ;
					if ( gCurrentEmitter->m_HpWorld )
						delete gCurrentEmitter->m_HpWorld ;
					MMC_FREE(gCurrentEmitter) ;
					gCurrentEmitter = &gEt ;
					stop = true ;
					ImGui::CloseCurrentPopup(); 
				}
				ImGui::SameLine();
				if (ImGui::Button("No", ImVec2(120,0))) { 
					ImGui::CloseCurrentPopup(); 
				}
				ImGui::EndPopup();
			}
			
			ImGui::TreePop();
		}

	}
	ImGui::PopItemWidth();
	ImGui::End() ;

}	
	

//} ;


/*
 brief :
*/
CacuEmitterEditor::CacuEmitterEditor ( const char* title ) 
{
	SetTitle ( title ) ;
}

/*
 brief :
*/
CacuEmitterEditor::~CacuEmitterEditor () 
{
}

/*
 brief :
*/
void CacuEmitterEditor::OnGui() 
{
}

/*
 brief :
*/
void CacuEmitterEditor::OnMenu() 
{
}


#endif // ifdef WIN32