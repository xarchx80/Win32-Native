#include <PCH.h>
#include "CBTHook.h"

HHOOK CBTHook::gHook = nullptr;

CBTHook::CBTHook(UINT thraedId)
	: mHookWndProc(nullptr), mThreadId(thraedId)
{
	//mHookWndProc = CBTHook::HookWndProc;
	//gHook = SetWindowsHookEx(WH_CBT, mHookWndProc, nullptr, mThreadId);
}

CBTHook::~CBTHook()
{

}

void CBTHook::OnCreate(HookEvent& e)
{
}

void CBTHook::OnActive(HookEvent& e)
{
}

void CBTHook::HookMessageBox(LPCSTR msg, UINT flag)
{
	
	gHook = SetWindowsHookEx(WH_CALLWNDPROC, CBTHook::HookCallWndProc, nullptr, GetCurrentThreadId());
	MessageBox(NULL, msg, "error", flag);
	::UnhookWindowsHookEx(gHook);
	gHook = nullptr;
}

LRESULT CBTHook::HookCBTWndProc(INT code, WPARAM wp, LPARAM lp)
{
	if (code < 0)
		CallNextHookEx(gHook, code, wp, lp);

	HWND hwnd = reinterpret_cast<HWND>(wp);
	HookEvent e{ hwnd, code, wp, lp };

	switch (code)
	{
	case HCBT_ACTIVATE:
	{
		//HDC dc = GetDC(hwnd);
		//static HBRUSH hbr = CreateSolidBrush(RGB(50, 50, 50));
		//SelectObject(dc, hbr);
		//RECT rc;
		//GetClientRect(hwnd, &rc);
		//FillRect(dc, &rc, hbr);
		return 0;
	}
		break;
	case HCBT_CREATEWND:
	{
		std::string buff;
		int len = GetWindowTextLength(hwnd) + 1;
		buff.reserve(128);
		GetClassName(hwnd, (LPSTR)buff.c_str(), 128);
		LOG("%s has been created", buff.c_str());
		buff.clear();
		buff.reserve(128);
		GetWindowText(hwnd, (LPSTR)buff.c_str(), 128);

		//LOG("%s hooked", buff.c_str());
	}
		break;
	default:
		break;
	}

	return CallNextHookEx(gHook, code, wp, lp);
}

LRESULT CBTHook::HookCallWndProc(INT code, WPARAM wp, LPARAM lp)
{
	if(code < 0) 
		return CallNextHookEx(gHook, code, wp, lp);
	
	if(code == HC_ACTION)
	{
		CWPSTRUCT* cs = (CWPSTRUCT*)lp;
		bool res = HookWndProc(cs->hwnd, cs->message, cs->wParam, cs->lParam);
		if (res) return res;
	}
	return CallNextHookEx(gHook, code, wp, lp);
}

BOOL CBTHook::HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	char lpClass[MAX_PATH];
	char lpText[MAX_PATH];
	switch (msg)
	{
	case WM_NCCREATE:
	{
		GetClassName(hwnd, lpClass, MAX_PATH);
		if (std::strcmp(lpClass, "#32770") == 0 ) {
			DWORD style = GetClassLongPtr(hwnd, GCL_STYLE);
			SetWindowLongPtr(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP | WS_CLIPCHILDREN);
			LOG("3270");

		}
		return TRUE;
		
	}
	break;
	case WM_COMMAND:
	{
		GetClassName(hwnd, lpClass, MAX_PATH);
		LOG("%s command", lpClass);
	}
	break;
	case WM_PAINT:
	{
		Painter p(hwnd);
		static HBRUSH br = CreateSolidBrush(RGB(40, 50, 60));
		static HBRUSH brr = CreateSolidBrush(RGB(140, 50, 60));
		RECT rc;
		GetClientRect(hwnd, &rc);
		p.DrawRect(rc, br);
		char* cn[128];
		RealGetWindowClass(hwnd, (LPSTR)cn, 256);
		LOG("%s", cn);
		if (std::strcmp((const char*)cn, WC_BUTTON)==0) {
			LOG("%s", WC_BUTTON);
			p.DrawRect(rc, brr);
		}
		return true;
	}
	break;
	}
	return TRUE;
}
