#ifndef ImHierarchyWindow_H
#define ImHierarchyWindow_H

#include "H3Std.h"
#include "ImwWindow.h"
#include "ImwMenu.h"

using namespace ImWindow;

class ImHierarchyWindow : public ImwWindow , ImwMenu , virtual public H3Memory
{
public: 

	ImHierarchyWindow(const char* pTitle = H3UtilGbk2Utf8("Hierarchy").data())
	{
		SetTitle(pTitle);
	}
	~ImHierarchyWindow(){}   

	virtual void OnGui() ;
	virtual void OnMenu() ;

} ;

#endif