#ifndef ImRenderWindow_H
#define ImRenderWindow_H

#include "H3Std.h"
#include "ImwWindow.h"
#include "ImwMenu.h"

using namespace ImWindow;

class ImRenderWindow : public ImwWindow , ImwMenu , virtual public H3Memory
{
public: 

	ImRenderWindow(const char* pTitle = H3UtilGbk2Utf8("Rendering").data())
	{
		SetTitle(pTitle);
	}
	~ImRenderWindow(){}

	virtual void OnGui() ;
	virtual void OnMenu() ;

} ;

#endif