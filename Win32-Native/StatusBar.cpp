#include <PCH.h>
#include "StatusBar.h"


StatusBar::StatusBar(Wnd * parent)
	: Control(parent)
{
	lpClass = STATUSCLASSNAME;
	style = WS_VISIBLE | WS_BORDER | CCS_BOTTOM;
	Create();
	int w[2] = { 300,300 };
	SendMessage(m_hwnd,SB_SETPARTS, (WPARAM)1, (LPARAM)w);
}
