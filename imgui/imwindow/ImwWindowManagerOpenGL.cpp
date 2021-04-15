#include "ImwWindowManagerOpenGL.h"
#include "ImwPlatformWindowOpenGL.h"

#include <imgui_impl_glfw_gl3.h>

using namespace ImWindow;

ImwWindowManagerOpenGL::ImwWindowManagerOpenGL()
{
	ImwPlatformWindowOpenGL::InitDX11();
}

ImwWindowManagerOpenGL::~ImwWindowManagerOpenGL()
{
	ImwPlatformWindowOpenGL::ShutdownDX11();
	ImGui_ImplGlfwGL3_ShutdownEx();
}

void ImwWindowManagerOpenGL::InternalRun()
{
	PreUpdate();
	//	jelo comitted start
	/*MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	Sleep(16);
	*/
	//	jelo comitted end
}

ImwPlatformWindow* ImwWindowManagerOpenGL::CreatePlatformWindow(bool bMain, ImwPlatformWindow* pParent, bool bDragWindow)
{
	IM_ASSERT(m_pCurrentPlatformWindow == NULL);
	ImwPlatformWindowOpenGL* pWindow = new ImwPlatformWindowOpenGL(bMain, bDragWindow, CanCreateMultipleWindow());
	if (pWindow->Init(pParent))
	{
		return (ImwPlatformWindow*)pWindow;
	}  
	else
	{
		delete pWindow;
		return NULL;
	}
}

ImVec2 ImwWindowManagerOpenGL::GetCursorPos()
{
	POINT oPoint;
	::GetCursorPos(&oPoint);
	return ImVec2(oPoint.x, oPoint.y);
}

bool ImwWindowManagerOpenGL::IsLeftClickDown()
{
	return GetAsyncKeyState(VK_LBUTTON);
}
