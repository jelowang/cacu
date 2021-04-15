// ImGui - standalone example application for Glfw + OpenGL 3, using programmable pipeline
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include "imgui_internal.h"
#include <stdio.h>
#include "ShadowDemo.h"
#include "StudioWindow.h"
#include "H3DrawPrimitives.h"
#include "CacuEditorWindow.h"
#include "CacuEmitterEditor.h"
#include "CacuMainScene.h"
#include "BillboardTest.h"
#include "H3Delegate.h"
#include "ImwWindowManagerOpenGL.h"
#include "ImDetailWindow.h"
#include "ImHierarchyWindow.h"
#include "ImRenderWindow.h"
#include "TextureAutoMapping.h"

StudioWindow* gStudioWindow = 0 ;

using namespace ImWindow;

ImVec2 gRenderWindowPos ;
ImVec2 gRenderWindowSize ;
iU32 gRenderWindowHeightPadding = 0 ;
iU32 gRenderWindowWidthPadding = 0 ;

int cacuMainWindowWidth = 1024 ;
int cacuMainWindowHeight = 1024;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error %d: %s\n", error, description);
}

static void initGuiSytle ()
{
	//	jelo add start
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font0 = io.Fonts->AddFontDefault();
	ImFont* font1 = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\arial.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesChinese());
	extern ImGuiState* GImGui;

#if 1
	//	jelo add
	GImGui->Style.Colors[ImGuiCol_WindowBg] = ImVec4(80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f, 255.0f / 255.0f);

	GImGui->Style.Colors[ImGuiCol_TitleBg] = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 115.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 115.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 115.0f / 255.0f);
	//	headers
	GImGui->Style.Colors[ImGuiCol_Header] = ImVec4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_HeaderActive] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);

	GImGui->Style.Colors[ImGuiCol_CloseButton] = ImVec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 128.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_MenuBarBg] = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f);
	//	buttons
	//GImGui->Style.Colors[ImGuiCol_Button] = ImVec4(0.0f / 255.0f, 0.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	//GImGui->Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f / 255.0f, 80.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	//GImGui->Style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_Button] = ImVec4(50.0f / 255.0f, 50.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ButtonActive] = ImVec4(60.0f / 255.0f, 60.0f / 255.0f, 60.0f / 255.0f, 255.0f / 255.0f);
	//	srollbars
	GImGui->Style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 150.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 80.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 100.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f);
#else
	//	jelo add
	GImGui->Style.Colors[ImGuiCol_Text] = ImVec4(40.0f / 255.0f, 40.0f / 255.0f, 40.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_WindowBg] = ImVec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_FrameBg] = ImVec4(150.0f / 255.0f, 150.0f / 255.0f, 150.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_TitleBg] = ImVec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(180.0f / 255.0f, 180.0f / 255.0f, 180.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(200.0f / 255.0f, 200.0f / 255.0f, 200.0f / 255.0f, 255.0f / 255.0f);
	//	headers
	GImGui->Style.Colors[ImGuiCol_Header] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_HeaderHovered] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_HeaderActive] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);

	GImGui->Style.Colors[ImGuiCol_CloseButton] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_MenuBarBg] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 255.0f / 255.0f);
	//	buttons
	GImGui->Style.Colors[ImGuiCol_Button] = ImVec4(200.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(255.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ButtonActive] = ImVec4(255.0f / 255.0f, 50.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f);
	//	srollbars
	GImGui->Style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(100.0f / 255.0f, 100.0f / 255.0f, 100.0f / 255.0f, 150.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f, 80.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f, 100.0f / 255.0f);
	GImGui->Style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(255.0f / 255.0f, 255.0f / 255.0f, 0.0f / 255.0f, 200.0f / 255.0f);
#endif

	GImGui->Style.FramePadding = ImVec2(4, 4);
	GImGui->Style.FrameRounding = 2.0f;
	GImGui->Style.WindowPadding = ImVec2(3, 3);
	GImGui->Style.WindowFillAlphaDefault = 1.0f;
	GImGui->Style.WindowRounding = 0.0f;
	GImGui->Style.IndentSpacing = 0.0f;
	GImGui->Style.ItemSpacing = ImVec2(10, 3);
	//	jelo add end
}

class MyImwWindow : public ImwWindow, ImwMenu
{
public:
	MyImwWindow(const char* pTitle = "MyImwWindow")
	{
		SetTitle(pTitle);
	}
	virtual void OnGui()
	{
		ImGuiStyle* ref = NULL;
		
		ImGuiStyle& style = ImGui::GetStyle();

		const ImGuiStyle def; // Default style
		if (ImGui::Button("Revert Style"))
			style =  def;
		if (ref)
		{
			ImGui::SameLine();
			if (ImGui::Button("Save Style"))
				*ref = style;
		}

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);

		if (ImGui::TreeNode("Rendering"))
		{
			ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
			ImGui::Checkbox("Anti-aliased shapes", &style.AntiAliasedShapes);
			ImGui::PushItemWidth(100);
			ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, NULL, 2.0f);
			if (style.CurveTessellationTol < 0.0f) style.CurveTessellationTol = 0.10f;
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sizes"))
		{
			ImGui::SliderFloat("Alpha", &style.Alpha, 0.20f, 1.0f, "%.2f");                 // Not exposing zero here so user doesn't "lose" the UI. But application code could have a toggle to switch between zero and non-zero.
			ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat("ChildWindowRounding", &style.ChildWindowRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
			ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarWidth", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 16.0f, "%.0f");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Colors"))
		{
			static int output_dest = 0;
			static bool output_only_modified = false;
			if (ImGui::Button("Output Colors"))
			{
				if (output_dest == 0)
					ImGui::LogToClipboard();
				else
					ImGui::LogToTTY();
				ImGui::LogText("ImGuiStyle& style = ImGui::GetStyle();" "\n");
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const ImVec4& col = style.Colors[i];
					const char* name = ImGui::GetStyleColName(i);
					if (!output_only_modified || memcmp(&col, (ref ? &ref->Colors[i] : &def.Colors[i]), sizeof(ImVec4)) != 0)
						ImGui::LogText("style.Colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" "\n", name, 22 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
				}
				ImGui::LogFinish();
			}
			ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY"); ImGui::PopItemWidth();
			ImGui::SameLine(); ImGui::Checkbox("Only Modified Fields", &output_only_modified);

			static ImGuiColorEditMode edit_mode = ImGuiColorEditMode_RGB;
			ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditMode_RGB);
			ImGui::SameLine();
			ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditMode_HSV);
			ImGui::SameLine();
			ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditMode_HEX);
			//ImGui::Text("Tip: Click on colored square to change edit mode.");

			static ImGuiTextFilter filter;
			filter.Draw("Filter colors", 200);

			ImGui::BeginChild("#colors", ImVec2(0, 300), true);
			ImGui::PushItemWidth(-160);
			ImGui::ColorEditMode(edit_mode);
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColName(i);
				if (!filter.PassFilter(name))
					continue;
				ImGui::PushID(i);
				ImGui::ColorEdit4(name, (float*)&style.Colors[i], true);
				if (memcmp(&style.Colors[i], (ref ? &ref->Colors[i] : &def.Colors[i]), sizeof(ImVec4)) != 0)
				{
					ImGui::SameLine(); if (ImGui::Button("Revert")) style.Colors[i] = ref ? ref->Colors[i] : def.Colors[i];
					if (ref) { ImGui::SameLine(); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i]; }
				}
				ImGui::PopID();
			}
			ImGui::PopItemWidth();
			ImGui::EndChild();

			ImGui::TreePop();
		}

		ImGui::PopItemWidth();
		if (ImGui::Button("Close"))
		{
			Destroy();
		}
	}

	virtual void OnMenu()
	{
		if (ImGui::BeginMenu("MyImwWindow menu"))
		{
			if (ImGui::MenuItem("Close it"))
			{
				Destroy();
			}
			ImGui::EndMenu();
		}
	}
};

class MyImwWindow3 : public ImwWindow
{
public:
	MyImwWindow3(const char* pTitle = "MyImwWindow3")
	{
		SetTitle(pTitle);
	}
	virtual void OnGui()
	{
		ImGui::Text("Hello, world!");

		ImGui::Checkbox("Use ImGui colors", &(ImwWindowManager::GetInstance()->GetConfig().m_bTabUseImGuiColors));
		if (ImGui::Button("Create"))
		{
			//MMC_NEW CacuEditorWindow();
			new MyImwWindow () ;
		}
	}
};


class StyleEditorWindow : public ImwWindow , ImwMenu
{
public:
	StyleEditorWindow()
	{
		SetTitle("Style Editor");
	}
	virtual void OnGui()
	{
		ImGuiStyle* ref = NULL;
		
		ImGuiStyle& style = ImGui::GetStyle();

		const ImGuiStyle def; // Default style
		if (ImGui::Button("Revert Style"))
			style =  def;
		if (ref)
		{
			ImGui::SameLine();
			if (ImGui::Button("Save Style"))
				*ref = style;
		}

		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);

		if (ImGui::TreeNode("Rendering"))
		{
			ImGui::Checkbox("Anti-aliased lines", &style.AntiAliasedLines);
			ImGui::Checkbox("Anti-aliased shapes", &style.AntiAliasedShapes);
			ImGui::PushItemWidth(100);
			ImGui::DragFloat("Curve Tessellation Tolerance", &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, NULL, 2.0f);
			if (style.CurveTessellationTol < 0.0f) style.CurveTessellationTol = 0.10f;
			ImGui::PopItemWidth();
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Sizes"))
		{
			ImGui::SliderFloat("Alpha", &style.Alpha, 0.20f, 1.0f, "%.2f");                 // Not exposing zero here so user doesn't "lose" the UI. But application code could have a toggle to switch between zero and non-zero.
			ImGui::SliderFloat2("WindowPadding", (float*)&style.WindowPadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("WindowRounding", &style.WindowRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat("ChildWindowRounding", &style.ChildWindowRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat2("FramePadding", (float*)&style.FramePadding, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("FrameRounding", &style.FrameRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat2("ItemSpacing", (float*)&style.ItemSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("ItemInnerSpacing", (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, "%.0f");
			ImGui::SliderFloat2("TouchExtraPadding", (float*)&style.TouchExtraPadding, 0.0f, 10.0f, "%.0f");
			ImGui::SliderFloat("IndentSpacing", &style.IndentSpacing, 0.0f, 30.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarWidth", &style.ScrollbarSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("ScrollbarRounding", &style.ScrollbarRounding, 0.0f, 16.0f, "%.0f");
			ImGui::SliderFloat("GrabMinSize", &style.GrabMinSize, 1.0f, 20.0f, "%.0f");
			ImGui::SliderFloat("GrabRounding", &style.GrabRounding, 0.0f, 16.0f, "%.0f");
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Colors"))
		{
			static int output_dest = 0;
			static bool output_only_modified = false;
			if (ImGui::Button("Output Colors"))
			{
				if (output_dest == 0)
					ImGui::LogToClipboard();
				else
					ImGui::LogToTTY();
				ImGui::LogText("ImGuiStyle& style = ImGui::GetStyle();" "\n");
				for (int i = 0; i < ImGuiCol_COUNT; i++)
				{
					const ImVec4& col = style.Colors[i];
					const char* name = ImGui::GetStyleColName(i);
					if (!output_only_modified || memcmp(&col, (ref ? &ref->Colors[i] : &def.Colors[i]), sizeof(ImVec4)) != 0)
						ImGui::LogText("style.Colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);" "\n", name, 22 - (int)strlen(name), "", col.x, col.y, col.z, col.w);
				}
				ImGui::LogFinish();
			}
			ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo("##output_type", &output_dest, "To Clipboard\0To TTY"); ImGui::PopItemWidth();
			ImGui::SameLine(); ImGui::Checkbox("Only Modified Fields", &output_only_modified);

			static ImGuiColorEditMode edit_mode = ImGuiColorEditMode_RGB;
			ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditMode_RGB);
			ImGui::SameLine();
			ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditMode_HSV);
			ImGui::SameLine();
			ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditMode_HEX);
			//ImGui::Text("Tip: Click on colored square to change edit mode.");

			static ImGuiTextFilter filter;
			filter.Draw("Filter colors", 200);

			ImGui::BeginChild("#colors", ImVec2(0, 300), true);
			ImGui::PushItemWidth(-160);
			ImGui::ColorEditMode(edit_mode);
			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				const char* name = ImGui::GetStyleColName(i);
				if (!filter.PassFilter(name))
					continue;
				ImGui::PushID(i);
				ImGui::ColorEdit4(name, (float*)&style.Colors[i], true);
				if (memcmp(&style.Colors[i], (ref ? &ref->Colors[i] : &def.Colors[i]), sizeof(ImVec4)) != 0)
				{
					ImGui::SameLine(); if (ImGui::Button("Revert")) style.Colors[i] = ref ? ref->Colors[i] : def.Colors[i];
					if (ref) { ImGui::SameLine(); if (ImGui::Button("Save")) ref->Colors[i] = style.Colors[i]; }
				}
				ImGui::PopID();
			}
			ImGui::PopItemWidth();
			ImGui::EndChild();

			ImGui::TreePop();
		}

		ImGui::PopItemWidth();
	}

	virtual void OnMenu ()
	{
		static bool show_app_about = false ;
        if (ImGui::BeginMenu(H3UtilGbk2Utf8("File").data()))
        {
            //ShowMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(H3UtilGbk2Utf8("Help").data()))
        {
            ImGui::MenuItem(H3UtilGbk2Utf8("About").data(), NULL, &show_app_about);
			ImGui::EndMenu();
        }
	}
};


int main(int, char**)
{
	AllocConsole();

    // Setup window
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return 1; 
	  
	gRenderWindowPos.x = 0 ;
	gRenderWindowPos.y = 0 ;
	gRenderWindowSize.x = 0 ;
	gRenderWindowSize.y = 0 ;
	gRenderWindowHeightPadding = 22 ;
	gRenderWindowWidthPadding = 2 ;

	initGuiSytle() ;

	ImwWindowManagerOpenGL oMgr;
	oMgr.Init();
	oMgr.SetMainTitle(H3UtilGbk2Utf8("Cacu Facial Studio").data());
	ImwWindowManager::GetInstance()->GetConfig().m_bTabUseImGuiColors = false ;
	
	gStudioWindow = MMC_NEW StudioWindow () ;
	//TextureAutoMapping* textureAutoMapping = MMC_NEW TextureAutoMapping();
	H3Delegate::getInstance()->init ( 60 ) ;
	H3Delegate::getInstance ( )->start ( gStudioWindow );
	H3Delegate::getInstance()->resize ( cacuMainWindowWidth, cacuMainWindowHeight );
	H3Delegate::getInstance()->renderToTexture () ;

	ImDetailWindow* detailWindow = MMC_NEW ImDetailWindow () ;
	ImHierarchyWindow* hierarcyWindow = MMC_NEW ImHierarchyWindow () ;
	oMgr.Dock ( hierarcyWindow ) ;
	oMgr.DockWith ( detailWindow , hierarcyWindow , E_DOCK_ORIENTATION_RIGHT);

	ImRenderWindow* renderWindow = MMC_NEW ImRenderWindow () ;
	oMgr.Dock ( renderWindow , E_DOCK_ORIENTATION_RIGHT);

	extern bool gWindowShouldClose ;
	while ( false == gWindowShouldClose ) {
		oMgr.Run();
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

#ifdef WIN32 
	iS8 symbol = 0 ;
	while ( '1' != symbol ) {
		scanf("%c",&symbol);
	}
#endif 

    return 0;
}
