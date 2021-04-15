#ifndef ImDetailWindow_H
#define ImDetailWindow_H

#include "H3Std.h"
#include "ImwWindow.h"
#include "ImwMenu.h"

using namespace ImWindow;

class ImDetailWindow : public ImwWindow , ImwMenu , virtual public H3Memory
{
public: 

	ImDetailWindow(const char* pTitle = H3UtilGbk2Utf8("Details").data())
	{
		SetTitle(pTitle);
	}
	~ImDetailWindow(){}

	virtual void OnGui() ;
	virtual void OnMenu() ;

} ;

#endif