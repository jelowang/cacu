
#include "H3Delegate.h"
#include "ImwPlatformWindowOpenGL.h"
#include "ImwWindowManager.h"
#include <vector>
#include <cmath>
#include <cstdio>
#include <limits>
#include <chrono>
#include <thread>
#include <mutex>

bool gWindowShouldClose = false ;

using namespace ImWindow;

ImwPlatformWindowOpenGL::InstanceMap		ImwPlatformWindowOpenGL::s_mInstances;
bool									ImwPlatformWindowOpenGL::s_bClassInitialized = false;
WNDCLASSEX								ImwPlatformWindowOpenGL::s_oWndClassEx;

ImwPlatformWindow*						ImwPlatformWindowOpenGL::s_pLastHoveredWindow = NULL;

INT64									ImwPlatformWindowOpenGL::g_Time = 0;
INT64									ImwPlatformWindowOpenGL::g_TicksPerSecond = 0;

static GLFWwindow* gTheMainWindow = NULL ;
static GLEWContext* gCurrentGLEWContext = 0 ;

ImwPlatformWindowOpenGL::ImwPlatformWindowOpenGL( bool bMain, bool bIsDragWindow, bool bCreateState )
	: ImwPlatformWindow( bMain, bIsDragWindow, bCreateState )
{
	m_bDrag = false; 
	m_oSize = ImVec2(0,0);
	m_oPosition = ImVec2(-1,-1);
	m_hWnd = NULL ;
}

ImwPlatformWindowOpenGL::~ImwPlatformWindowOpenGL()
{
	s_mInstances.erase(m_hWnd);
	glfwDestroyWindow(m_hWnd);
	delete m_pGLEWContext ;

}

GLEWContext* glewGetContext()
{
	H3ASSERT(gCurrentGLEWContext) ;
    return gCurrentGLEWContext;
} 

bool ImwPlatformWindowOpenGL::Init(ImwPlatformWindow* pMain)
{

	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

	if ( pMain == NULL ) {
		m_hWnd = glfwCreateWindow(1280, 720, "Particle Studio", NULL, NULL);
		gWindowShouldClose = false ;
	} else {
		m_hWnd = glfwCreateWindow(1280, 720, "Particle Studio", NULL, gTheMainWindow);
	}
	glfwMakeContextCurrent(m_hWnd);

	m_pGLEWContext = new GLEWContext();
	gCurrentGLEWContext = m_pGLEWContext ;
 
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		printf("GLEW Error occured, Description: %s\n", glewGetErrorString(err));
		return false ;
	}

	if ( pMain == NULL ) {

		gTheMainWindow = m_hWnd ;
		
 		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	}

	s_mInstances.insert(std::pair<GLFWwindow*, ImwPlatformWindowOpenGL*>(m_hWnd, this));

	if (m_bIsDragWindow){
		HWND handle = glfwGetWin32Window(m_hWnd);
		SetWindowLong(handle, GWL_EXSTYLE, GetWindowLong(handle, GWL_EXSTYLE) | WS_EX_LAYERED | WS_POPUP);
		SetWindowLong(handle, GWL_STYLE, WS_POPUP | WS_BORDER);
		SetLayeredWindowAttributes(handle, RGB(0, 0, 0), 128, LWA_ALPHA);
	}

	SetState();
	ImGui_ImplGlfwGL3_Init(m_hWnd, true);

	if (!QueryPerformanceFrequency((LARGE_INTEGER *)&g_TicksPerSecond)) 
		return false;
	if (!QueryPerformanceCounter((LARGE_INTEGER *)&g_Time))
		return false;

	ImGuiIO& io = ImGui::GetIO();
	io.KeyMap[ImGuiKey_Tab] = VK_TAB;                       // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array that we will update during the application lifetime.
	io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
	io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
	io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
	io.KeyMap[ImGuiKey_Home] = VK_HOME;
	io.KeyMap[ImGuiKey_End] = VK_END;
	io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
	io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
	io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
	io.KeyMap[ImGuiKey_A] = 'A';
	io.KeyMap[ImGuiKey_C] = 'C';
	io.KeyMap[ImGuiKey_V] = 'V';
	io.KeyMap[ImGuiKey_X] = 'X';
	io.KeyMap[ImGuiKey_Y] = 'Y';
	io.KeyMap[ImGuiKey_Z] = 'Z';

	io.RenderDrawListsFn = ImGui_ImplGlfwGL3_RenderDrawListsEx;  // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
	io.ImeWindowHandle = glfwGetWin32Window(m_hWnd);

	RestoreState();

	m_hCursorArrow = LoadCursor( NULL, IDC_ARROW );
	m_hCursorResizeNS = LoadCursor( NULL, IDC_SIZENS );
	m_hCursorResizeWE = LoadCursor( NULL, IDC_SIZEWE );

	if( pMain == NULL )
	{
		ImGui_ImplGlfwGL3_NewFrame();
	}
	
	ImGui_ImplGlfwGL3_CreateWindow ( m_hWnd , 1280, 720 ) ;

	//static std::thread gl_thread( std::bind(&ImWindow::ImwPlatformWindowOpenGL::Paint, this) ); 

	return true;
}

const ImVec2& ImwPlatformWindowOpenGL::GetPosition() const
{
	return m_oPosition;
}

const ImVec2& ImwPlatformWindowOpenGL::GetSize() const
{
	return m_oSize;
}

void ImwPlatformWindowOpenGL::Show()
{
	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = m_pGLEWContext ;
	glfwShowWindow(m_hWnd);
}

void ImwPlatformWindowOpenGL::Hide()
{
	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = m_pGLEWContext ;
	glfwHideWindow(m_hWnd);
}

void ImwPlatformWindowOpenGL::SetSize(int iWidth, int iHeight)
{
	//RECT oRect;
	//oRect.left = 0;
	//oRect.top = 0;
	//oRect.right = iWidth;
	//oRect.bottom = iHeight;
	//AdjustWindowRect(&oRect, GetWindowLong(m_hWnd, GWL_STYLE), false);
	//SetWindowPos(m_hWnd, 0, 0, 0, oRect.right - oRect.left, oRect.bottom - oRect.top, SWP_NOMOVE);

	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = m_pGLEWContext ;
	glfwSetWindowSize(m_hWnd, iWidth, iHeight);

}

void ImwPlatformWindowOpenGL::SetPosition(int iX, int iY)
{
	//RECT oRect;
	//oRect.left = iX;
	//oRect.top = iY;
	//oRect.right = iX + m_oSize.x;
	//oRect.bottom = iY + m_oSize.y;
	//AdjustWindowRect(&oRect, GetWindowLong(m_hWnd, GWL_STYLE), false);
	//SetWindowPos(m_hWnd, 0, oRect.left, oRect.top, 0, 0, SWP_NOSIZE);

	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = m_pGLEWContext ;
	glfwSetWindowPos(m_hWnd, iX, iY);

}

void ImwPlatformWindowOpenGL::SetTitle(const char* pTtile)
{
	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = m_pGLEWContext ;
	glfwSetWindowTitle(m_hWnd, pTtile);
}

void ImwPlatformWindowOpenGL::PreUpdate()
{

	gWindowShouldClose = (bool )glfwWindowShouldClose ( gTheMainWindow ) ;

	//	jelo add start
	SetState();
	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = (GLEWContext* )m_pGLEWContext ;
	ImGuiIO& io = ImGui::GetIO();

	if (glfwGetWindowAttrib(m_hWnd, GLFW_FOCUSED))
	{
		double mouse_x, mouse_y;
		glfwGetCursorPos(m_hWnd, &mouse_x, &mouse_y);
		io.MousePos = ImVec2((float)mouse_x, (float)mouse_y);   // Mouse position in screen coordinates (set to -1,-1 if no mouse / on another screen, etc.)
		s_pLastHoveredWindow = this;

		extern ImVec2 gRenderWindowPos ;
		extern ImVec2 gRenderWindowSize ;
		extern iU32 gRenderWindowHeightPadding ;
		extern iU32 gRenderWindowWidthPadding ;
		H3MouseEvent m ;
		m.e |= H3MouseEvent_Moving ;
		m.point.id[0] = 0 ;
		m.point.totall = 1 ;
		m.point.x[0] = (int )mouse_x - gRenderWindowPos.x - gRenderWindowWidthPadding ;
		m.point.y[0] = (int )mouse_y - gRenderWindowPos.y - gRenderWindowHeightPadding ;
		if ( 0 <= m.point.x[0] && m.point.y[0] >= 0 ) {
			if ( m.point.x[0] < gRenderWindowSize.x-gRenderWindowWidthPadding && m.point.y[0] < gRenderWindowSize.y-gRenderWindowWidthPadding ) {
				H3Delegate::getInstance()->mouseEvent ( m ) ;
			}
		}

	}
	else
    {
        io.MousePos = ImVec2(-1,-1);
    }

	ImGui_ImplGlfwGL3_SetCurrentWindow ( m_hWnd ) ;
	ImGui_ImplGlfwGL3_UpdateMouse () ;
	RestoreState();
	//	jelo add end

}

void ImwPlatformWindowOpenGL::Paint()
{

	glfwMakeContextCurrent(m_hWnd);
	gCurrentGLEWContext = (GLEWContext* )m_pGLEWContext ;
	glfwPollEvents();

	int windowWidth;
	int windowHeight;

	double cursorX;
	double cursorY;
	
	glfwGetWindowSize(m_hWnd, &windowWidth, &windowHeight);
	m_oSize.x = (float)windowWidth;
	m_oSize.y = (float)windowHeight;

	if (m_bDrag)
	{
	
		glfwGetWindowSize(m_hWnd, &windowWidth, &windowHeight);
		glfwGetCursorPos(m_hWnd, &cursorX, &cursorY);

		int iX = m_iWindowPosStartDrag.x + cursorX - m_iCursorPosStartDrag.x;
		int iY = m_iWindowPosStartDrag.y + cursorY - m_iCursorPosStartDrag.y;

		glfwSetWindowPos(m_hWnd, iX, iY );

	}

	glfwGetWindowPos(m_hWnd, &windowWidth, &windowHeight);
	m_oPosition = ImVec2((float)windowWidth, (float)windowHeight);

	//	rendering
	SetState();
		
	ImGui_ImplGlfwGL3_SetCurrentWindow ( m_hWnd ) ;
	
	if ( m_hWnd == gTheMainWindow ) {
		H3Delegate::getInstance()->update(0.016) ;
	}

	int frameBufferW ;
	int frameBufferH ;

	glfwGetWindowSize(m_hWnd, &windowWidth, &windowHeight);
	m_oSize.x = (float)windowWidth;
	m_oSize.y = (float)windowHeight;

	glfwGetFramebufferSize(m_hWnd, &frameBufferW, &frameBufferH);
	ImGui::GetIO().DisplaySize = ImVec2(m_oSize.x, m_oSize.y);
	ImGui::GetIO().DisplayFramebufferScale = ImVec2((float)frameBufferW / windowWidth, (float)frameBufferH / windowHeight);

	// Setup time step
	//double current_time =  glfwGetTime();
	//ImGui::GetIO().DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f/60.0f);
	//g_Time = current_time;

	ImGui::NewFrame();
	ImwPlatformWindow::Paint();
	glfwSwapBuffers(m_hWnd);

	if (this == s_pLastHoveredWindow)
	{
		switch (ImGui::GetMouseCursor())
		{
		case ImGuiMouseCursor_Arrow:
			SetCursor(m_hCursorArrow);
			break;
		case ImGuiMouseCursor_TextInput:         // When hovering over InputText, etc.
			SetCursor(m_hCursorArrow);
			break;
		case ImGuiMouseCursor_Move:              // Unused
			SetCursor(m_hCursorArrow);
			break;
		case ImGuiMouseCursor_ResizeNS:          // Unused
			SetCursor(m_hCursorResizeNS);
			break;
		case ImGuiMouseCursor_ResizeEW:          // When hovering over a column
			SetCursor(m_hCursorResizeWE);
			break;
		case ImGuiMouseCursor_ResizeNESW:        // Unused
			SetCursor(m_hCursorArrow);
			break;
		case ImGuiMouseCursor_ResizeNWSE:        // When hovering over the bottom-right corner of a window
			SetCursor(m_hCursorArrow);
			break;
		}
	}
		
	RestoreState();
	
}

void ImwPlatformWindowOpenGL::Destroy()
{

}

void ImwPlatformWindowOpenGL::StartDrag()
{
	m_bDrag = true;

	int windowX;
	int windowY;

	glfwGetWindowPos(m_hWnd, &windowX, &windowY);

	double cursorX;
	double cursorY;

	glfwGetCursorPos(m_hWnd, &cursorX, &cursorY);

	m_iWindowPosStartDrag.x = windowX;
	m_iWindowPosStartDrag.y = windowY;

	m_iCursorPosStartDrag.x = cursorX;
	m_iCursorPosStartDrag.y = cursorY;
}

void ImwPlatformWindowOpenGL::StopDrag()
{
	m_bDrag = false;
}

bool ImwPlatformWindowOpenGL::IsDraging()
{
	return m_bDrag;
}


GLFWwindow* ImwPlatformWindowOpenGL::GetHWnd()
{
	return m_hWnd;
}

LRESULT ImwPlatformWindowOpenGL::OnMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0 ;
}

// Static

int ImwPlatformWindowOpenGL::GetInstanceCount()
{
	return s_mInstances.size();
}

LRESULT ImwPlatformWindowOpenGL::ImwPlatformWindowOpenGLProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0 ;
}

bool ImwPlatformWindowOpenGL::InitDX11()
{

	//	jelo add
	return 0;
	//	jelo

}

void ImwPlatformWindowOpenGL::ShutdownDX11()
{
}