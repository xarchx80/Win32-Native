#include <PCH.h>
#include "IButton.h"

IButton::IButton(Wnd* parent, LPCSTR text)
	: SuperClassControl(parent)
{
	SetClassName(WC_BUTTON);
	SetSuperClassName("BUTTON_EX");
	SetWindowName(text);
	Create();
}

IButton::IButton(Wnd * parent, int x, int y, int w, int h, LPCSTR text)
	: SuperClassControl(parent, x, y, w, h, text)
{
	SetClassName(WC_BUTTON);
	SetSuperClassName("BUTTON_EX");
	SetWindowName(text);
	Create();
}

LRESULT IButton::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		LOG("butttonn %s command", GetText());
		break;
	}
	}
	return CallWindowProc(GetPreWndProc(), hwnd, msg, wp, lp);
}
