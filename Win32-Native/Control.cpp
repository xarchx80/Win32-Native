#include <PCH.h>
#include "Control.h"

Control::Control(Wnd* parent)
	: Wnd(parent)
{
}

Control::Control(Wnd* parent, int x, int y, int w, int h, const char* text)
	: Wnd(parent, x, y, w, h, text)
{
}

bool Control::Create()
{
	if (!Wnd::Create())
		return false;
	is_control = true;
	SetWindowLongPtr(m_hwnd, GWL_USERDATA, (LONG)this);
	SetWindowSubclass(m_hwnd, Control::GlobalSubWndProc, (UINT_PTR)this, (DWORD_PTR)this);
	return true;
}

//LRESULT Control::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
//{
//	return DefSubclassProc(hwnd, msg, wp, lp);
//}

LRESULT Control::GlobalSubWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id, DWORD_PTR data)
{
	Control* control = (Control*)data;

	if (control) {
		return Wnd::GlobalWndProc(hwnd, msg, wp, lp);
	}

	return DefSubclassProc(hwnd, msg, wp, lp);
}
