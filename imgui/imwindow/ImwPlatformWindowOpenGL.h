
#ifndef __IM_PLATFORM_WINDOW_DX11_H__
#define __IM_PLATFORM_WINDOW_DX11_H__

#include "ImwConfig.h"
#include "ImwPlatformWindow.h"

#include <map>

#include "H3Std.h"
#include <imgui_impl_glfw_gl3.h>

#include <windows.h>
#include <windowsx.h>


namespace ImWindow
{
	class ImwPlatformWindowOpenGL : ImwPlatformWindow
	{
		friend class ImwWindowManagerOpenGL;
	public:
		typedef ImwMap<GLFWwindow*, ImwPlatformWindowOpenGL*> InstanceMap;
	public:
											ImwPlatformWindowOpenGL(bool bMain, bool bIsDragWindow, bool bCreateState);
		virtual								~ImwPlatformWindowOpenGL();

		virtual bool						Init(ImwPlatformWindow* pMain);

		virtual const ImVec2&				GetPosition() const;
		virtual const ImVec2&				GetSize() const;

		virtual void						Show();
		virtual void						Hide();
		virtual void						SetSize(int iWidth, int iHeight);
		virtual void						SetPosition(int iX, int iY);
		virtual void						SetTitle(const char* pTtile);

		GLFWwindow*							GetHWnd();
	protected:
		virtual void						PreUpdate();
		virtual void						Paint();
		virtual void						Destroy();

		virtual void						StartDrag();
		virtual void						StopDrag();
		virtual bool						IsDraging();

		LRESULT								OnMessage(UINT message, WPARAM wParam, LPARAM lParam);

		GLFWwindow*							m_hWnd;
		void*								m_glewContext ;

		//	jelo add start
		GLEWContext*						m_pGLEWContext;
		//	jelo add end

		// Static
	public:
		static int							GetInstanceCount();

	protected:
		static LRESULT CALLBACK				ImwPlatformWindowOpenGLProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		static bool							InitDX11();
		static void							ShutdownDX11();


		static InstanceMap					s_mInstances;
		static bool							s_bClassInitialized;
		static WNDCLASSEX					s_oWndClassEx;
		static ImwPlatformWindow*			s_pLastHoveredWindow;
		static INT64						g_Time;
		static INT64						g_TicksPerSecond;

		ImVec2								m_oPosition;
		ImVec2								m_oSize;

		bool								m_bDrag;
		ImVec2								m_iCursorPosStartDrag;
		ImVec2								m_iWindowPosStartDrag;

		HCURSOR								m_hCursorArrow;
		HCURSOR								m_hCursorResizeNS;
		HCURSOR								m_hCursorResizeWE;

	};
}

#endif // __IM_PLATFORM_WINDOW_DX11_H__