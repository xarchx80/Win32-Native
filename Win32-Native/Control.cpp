#include <PCH.h>
#include "Control.h"

SubClassControl::SubClassControl(Wnd* parent)
	: Wnd(parent)
{
	mIsSubClass = true;
}

SubClassControl::SubClassControl(Wnd* parent, int x, int y, int w, int h, const char* text)
	: Wnd(parent, x, y, w, h, text)
{
	mIsSubClass = true;
}

SubClassControl::~SubClassControl()
{
	RemoveWindowSubclass(mHwnd, SubClassControl::SubClassWndProc, NULL);
}

bool SubClassControl::Create()
{
	if (!Wnd::Create())
		return false;
	mIsControl = true;
	//SetWindowLongPtr(mHwnd, GWL_USERDATA, (LONG)this);
	SetWindowSubclass(mHwnd, SubClassControl::SubClassWndProc, (UINT_PTR)this, (DWORD_PTR)this);
	SetWindowLongPtr(mHwnd, GWL_USERDATA, (LONG)this);
	return true;
}

LRESULT SubClassControl::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return Wnd::LocalWndProc(hwnd, msg, wp, lp);
}

LRESULT SubClassControl::LocalDefWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return DefSubclassProc(hwnd, msg, wp, lp);
}

LRESULT SubClassControl::SubClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id, DWORD_PTR data)
{
	SubClassControl* wnd = (SubClassControl*)data;

	if (wnd) {
		//return Wnd::GlobalWndProc(hwnd, msg, wp, lp);
		//LOG("sub class");
		return wnd->LocalWndProc(hwnd, msg, wp, lp);
	}

	return DefSubclassProc(hwnd, msg, wp, lp);
}

std::vector<std::string> SuperClassControl::mRegistedSuperClasses{};

SuperClassControl::SuperClassControl(Wnd* parent)
	: Wnd(parent)
{
	mIsSupperClass = true;
}

SuperClassControl::SuperClassControl(Wnd* parent, int x, int y, int w, int h, const char* text)
	: Wnd(parent, x, y, w, h, text)
{
	mIsSupperClass = true;
}
#include <algorithm>
bool SuperClassControl::SetSuperClass(LPCSTR src, LPCSTR dest)
{
	//if (std::find(mRegistedSuperClasses.begin(), mRegistedSuperClasses.end(),
	//	dest) != mRegistedSuperClasses.end())
	//{
	//	LOG("has already");
	//	
	//}
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	if (!GetClassInfoEx(NULL, src, &wc)) {
		LOG("warnning there is no %s builtin class", src);
	}
	
	mPreWndProc = wc.lpfnWndProc;
	mPreCBWndExtra = wc.cbWndExtra;

	wc.lpszClassName = dest;
	wc.style &= ~CS_GLOBALCLASS;

	wc.cbWndExtra += sizeof(SuperClassControl*);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = SuperClassControl::SuperClassWndProc;
	//wc.lpfnWndProc = Wnd::GlobalWndProc;

	bool bRegisted = RegisterClassEx(&wc);
	if (bRegisted) {
		mRegistedSuperClasses.emplace_back(wc.lpszClassName);
	}
	
	return true;
}

WNDPROC SuperClassControl::GetPreWndProc() const
{
	return mPreWndProc;
}

LRESULT SuperClassControl::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	assert(mPreWndProc);
	switch (msg)
	{
	case WM_COMMAND:
	{
		LOG("%s from supper class command", GetText());
	}
	break;
	}
	return LocalDefWndProc(hwnd, msg, wp, lp);
}

LRESULT SuperClassControl::LocalDefWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return CallWindowProc(mPreWndProc, hwnd, msg, wp, lp);
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