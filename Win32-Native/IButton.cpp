#include <PCH.h>
#include "IButton.h"

IButton::IButton(Wnd* parent, LPCSTR text)
	: SubClassControl(parent)
{
	SetClassName(WC_BUTTON);
	SetStyle(BS_PUSHBUTTON);
	SetWindowName(text);
	Create();
}

IButton::IButton(Wnd * parent, int x, int y, int w, int h, LPCSTR text)
	: SubClassControl(parent, x, y, w, h, text)
{
	SetClassName(WC_BUTTON);
	SetStyle(WS_VISIBLE | BS_PUSHBUTTON);
	SetWindowName(text);
	Create();
}

LRESULT IButton::OnCtrlColor(Event& e)
{
	LOG("color");
	return (LRESULT)GetBkBrush();
}

LRESULT IButton::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Event e{ hwnd, msg, wp, lp };
	switch (msg)
	{
	case WM_CTLCOLOR:
		return OnCtrlColor(e);
		break;
	case WM_CTLCOLORBTN:
		return OnCtrlColor(e);
		break;
	case WM_COMMAND:
	{
		LOG("butttonn %s command", GetText());
		break;
	}
	case WM_PAINT:
		return 0;
		break;
	}
	return LocalDefWndProc(hwnd, msg, wp, lp);
}
