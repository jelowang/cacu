
#ifndef __IM_WINDOW_MANAGER_DX11_H__
#define __IM_WINDOW_MANAGER_DX11_H__

#include "ImwConfig.h"

#include "ImwWindowManager.h"

namespace ImWindow
{
	class ImwWindowManagerOpenGL : public ImwWindowManager
	{
	public:
		ImwWindowManagerOpenGL();
		virtual							~ImwWindowManagerOpenGL();
	protected:
		virtual bool					CanCreateMultipleWindow() { return true; }
		virtual ImwPlatformWindow*		CreatePlatformWindow(bool bMain, ImwPlatformWindow* pParent, bool bDragWindow);

		virtual void					InternalRun();
		virtual ImVec2					GetCursorPos();
		virtual bool					IsLeftClickDown();
	};
}

#endif //__IM_WINDOW_MANAGER_DX11_H__