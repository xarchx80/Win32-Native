#include <PCH.h>
#include "Wnd.h"

Wnd::Wnd(Wnd* parent)
	: m_parent(parent), geo{0},lpClass("Wnd32"), lpText("Wnd32"), style(WS_VISIBLE)
{
	
}

Wnd::Wnd(Wnd* parent, int x, int y, int w, int h, const TCHAR* text)
	: m_parent(parent), geo{ x, y, w, h}, lpClass("Wnd32"), lpText(text), style(WS_VISIBLE)
{

}

Wnd::~Wnd()
{
	SAFE_DELETE_WND(m_hwnd);
}



bool Wnd::Create()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = lpClass.c_str();
	wc.lpfnWndProc = Wnd::GlobalWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;
	if (!GetClassInfoEx(wc.hInstance, wc.lpszClassName, &wc)) {
		if (!RegisterClassEx(&wc)) {
			assert(0);
		}
	}

	HWND parent_hwnd = nullptr;
	if (m_parent) {
		if (m_parent->m_hwnd)
			parent_hwnd = m_parent->m_hwnd;
	}

	if (parent_hwnd) {
		style &= ~WS_POPUP; style |= WS_CHILD;
	}else {
		style &= ~WS_CHILD; style |= WS_POPUP;
	}

	m_hwnd = CreateWindowEx(
		NULL,
		lpClass.c_str(),
		lpText.c_str(),
		style,
		geo.x, geo.y, geo.cx, geo.cy,
		parent_hwnd, (HMENU)id, wc.hInstance, this);

	if (!m_hwnd) {
		return false;
	}
	return true;
}

void Wnd::Show()
{
	ShowWindow(m_hwnd, TRUE);
}

const char* Wnd::GetText() const
{
	return lpText.c_str();
}

LRESULT Wnd::DrawItemEvent(DRAWITEMSTRUCT* dis)
{
	return FALSE;
}

LRESULT Wnd::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{

	switch (msg)
	{
	case WM_SIZE:
		LOG("%s sizing", GetText());
		break;

	case WM_RBUTTONUP:
		LOG("%s button", GetText());
		break;

	case WM_COMMAND:
	{
		LOG("%s command", GetText());
		Wnd* child = nullptr;
		child = (Wnd*)GetWindowLongPtr((HWND)lp, GWL_USERDATA);
		if (child) {
			LOG("%s child command", child->GetText());
		}
		break;
	}
	case WM_NOTIFY:
	{
		//LOG("%s notify", GetText());
		HWND child_hwnd = ((LPNMHDR)lp)->hwndFrom;
		Wnd* child = (Wnd*)GetWindowLongPtr(child_hwnd, GWL_USERDATA);
		//if (child) {
		//	LOG("%s child notify", child->GetText());
		//}
		break;
	}
	case WM_PRINTCLIENT:
	{
		LOG("%s is printclient", GetText());
		break;
	}
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lp;
		Wnd* child = (Wnd*)GetWindowLongPtr(dis->hwndItem, GWL_USERDATA);
		if (child) {
			if (child->DrawItemEvent(dis))
				return TRUE;
		}
		break;
	}
	case WM_NCCALCSIZE: 
	{
		NCCALCSIZE_PARAMS* np = (NCCALCSIZE_PARAMS*)lp;
		LOG("%s nccsize", GetText());
	}
	break;
	}
	if (is_control) {
		return DefSubclassProc(hwnd, msg, wp, lp);
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT Wnd::GlobalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Wnd* wnd = (Wnd*)GetWindowLongPtr(hwnd, GWL_USERDATA);

	switch (msg)
	{
	case WM_NCCREATE: {
		LPCREATESTRUCT clp = (LPCREATESTRUCT)lp;
		wnd = (Wnd*)clp->lpCreateParams;
		SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG)wnd);
		return true;
		break;
	}
	}
	if (wnd)
		return wnd->LocalWndProc(hwnd, msg, wp, lp);

	return DefWindowProc(hwnd, msg, wp, lp);
}