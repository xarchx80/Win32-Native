#include <PCH.h>
#include "Control.h"

SubClassControl::SubClassControl(Wnd* parent)
	: Wnd(parent)
{
	m_IsSubClass = true;
}

SubClassControl::SubClassControl(Wnd* parent, int x, int y, int w, int h, const char* text)
	: Wnd(parent, x, y, w, h, text)
{
	m_IsSubClass = true;
}

bool SubClassControl::Create()
{
	if (!Wnd::Create())
		return false;
	is_control = true;
	SetWindowLongPtr(m_hwnd, GWL_USERDATA, (LONG)this);
	SetWindowSubclass(m_hwnd, SubClassControl::SubClassWndProc, (UINT_PTR)this, (DWORD_PTR)this);
	return true;
}



LRESULT SubClassControl::SubClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id, DWORD_PTR data)
{
	SubClassControl* control = (SubClassControl*)data;

	if (control) {
		return Wnd::GlobalWndProc(hwnd, msg, wp, lp);
	}

	return DefSubclassProc(hwnd, msg, wp, lp);
}

std::vector<std::string> SuperClassControl::m_registeredSuperClasses{};

SuperClassControl::SuperClassControl(Wnd* parent)
	: Wnd(parent)
{
	m_IsSupperClass = true;
}

SuperClassControl::SuperClassControl(Wnd* parent, int x, int y, int w, int h, const char* text)
	: Wnd(parent, x, y, w, h, text)
{
	m_IsSupperClass = true;
}
#include <algorithm>
bool SuperClassControl::SetSuperClass(LPCSTR src, LPCSTR dest)
{
	//if (std::find(m_registeredSuperClasses.begin(), m_registeredSuperClasses.end(),
	//	dest) != m_registeredSuperClasses.end())
	//{
	//	LOG("has already");
	//	
	//}
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	if (!GetClassInfoEx(NULL, src, &wc)) {
		LOG("warnning there is no %s builtin class", src);
	}
	
	m_preWndProc = wc.lpfnWndProc;
	m_PreCBWndExtra = wc.cbWndExtra;

	wc.lpszClassName = dest;
	wc.style &= ~CS_GLOBALCLASS;

	wc.cbWndExtra += sizeof(SuperClassControl*);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = SuperClassControl::SuperClassWndProc;
	//wc.lpfnWndProc = Wnd::GlobalWndProc;

	bool bRegisted = RegisterClassEx(&wc);
	if (bRegisted) {
		m_registeredSuperClasses.emplace_back(wc.lpszClassName);
	}
	
	return true;
}




LRESULT SuperClassControl::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	assert(m_preWndProc);
	switch (msg)
	{
	case WM_COMMAND:
	{
		LOG("%s from supper class command", GetText());
	}
	break;
	}
	return CallWindowProc(m_preWndProc, hwnd, msg, wp, lp);
}

LRESULT SuperClassControl::SuperClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Wnd* wnd = (Wnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (msg == WM_NCCREATE) {
		wnd = (Wnd*)LPCREATESTRUCT(lp)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)wnd);
	}
	if (wnd) return wnd->LocalWndProc(hwnd, msg, wp, lp);

	return DefSubclassProc(hwnd, msg, wp, lp);
}