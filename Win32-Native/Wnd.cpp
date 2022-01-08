#include <PCH.h>
#include "Wnd.h"

//WNDPROC Wnd::mPreWndProc = nullptr;

Wnd::Wnd(Wnd* parent)
	: m_parent(parent), geo{ 0 }, lpClass("Wnd32"), lpText("Wnd32"), style(WS_VISIBLE), styleEx(NULL)
{
	
}

Wnd::Wnd(Wnd* parent, int x, int y, int w, int h, const TCHAR* text)
	: m_parent(parent), geo{ x, y, w, h }, lpClass("Wnd32"), lpText(text), style(WS_VISIBLE), styleEx(NULL)
{

}

Wnd::~Wnd()
{
	SAFE_DELETE_GDIOBJ(hbrBkgorund);
	SAFE_DELETE_WND(m_hwnd);
}



bool Wnd::Create()
{
	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = hbrBkgorund ?  hbrBkgorund :(HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = lpClass.c_str();
	wc.lpfnWndProc = Wnd::GlobalWndProc;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	bool exist = GetClassInfoEx(wc.hInstance, wc.lpszClassName, &wc);
	if (exist) {
		if (m_IsSupperClass) {
			assert(!lpSuperClass.empty());
			SetSuperClass(wc.lpszClassName, lpSuperClass.c_str());
			//lpClass = lpSuperClass;
		}
	}
	else {
		if (RegisterClassEx(&wc)) LOG("%s __registered class", wc.lpszClassName);
		else assert(0);
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
		styleEx,
		m_IsSupperClass ? lpSuperClass.c_str() : lpClass.c_str(),
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

LRESULT Wnd::NotifyEvent(Event & e)
{
	return FALSE;
}

LRESULT Wnd::NotifyReflectEvent(Event & e)
{
	return FALSE;
}

LRESULT Wnd::PaintEvent(Painter &p)
{
	return FALSE;
}

void Wnd::OnResize(SizeEvent& e)
{
	_CRT_UNUSED(e);
}

LRESULT Wnd::DrawItemEvent(DRAWITEMSTRUCT* dis)
{
	return FALSE;
}

LRESULT Wnd::EraseBkgndEvent(Event &e)
{
	return FALSE;
}

LRESULT Wnd::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Event evt{ msg, wp, lp };
	switch (msg)
	{
	case WM_SIZE:
		//LOG("%s sizing", GetText());
		break;

	case WM_RBUTTONUP:
		//LOG("%s button", GetText());
		break;

	case WM_COMMAND:
	{
		//LOG("%s command", GetText());
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
		BOOL res = FALSE;
		if (child) {
			//LOG("%s notify", child->GetText());
			res = child->NotifyReflectEvent(evt);
			if (res) return res;
		}
		res = NotifyEvent(evt);
		if (res) return res;
		break;
	}
	case WM_PRINTCLIENT:
	{
		//LOG("%s is printclient", GetText());
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
		//LOG("%s nccsize", GetText());
	}
	break;
	case WM_PAINT:
	{
		if (is_control && !is_control_paint_event_enable)
			break;
		LOG("%s paint", GetText());
		Painter p(hwnd);
		if (PaintEvent(p))
			return 0;
		break;
	}
	
	case WM_ERASEBKGND:
	{
		//LOG("%s erase", GetText());
		if (EraseBkgndEvent(evt))
			return TRUE;
		return FALSE;
	}
	break;
	case WM_NCPAINT:
		//LOG("%s NCPAINT", GetText());
	break;
	}
	//return 0;

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
		//return true;
	}
	break;
	}
	if (wnd)
		return wnd->LocalWndProc(hwnd, msg, wp, lp);

	return DefWindowProc(hwnd, msg, wp, lp);
}

bool Wnd::SetSuperClass(LPCSTR src, LPCSTR dest)
{
	_CRT_UNUSED(src);
	_CRT_UNUSED(dest);
	return false;
}

bool Wnd::SetSubClass()
{
	return false;
}

bool Wnd::HasStyle(DWORD s)
{
	return (style & s);
}

void Wnd::AddStyle(DWORD s)
{
	style |= s;
}

void Wnd::RemoveStyle(DWORD s)
{
	style &= ~s;
}

void Wnd::SetStyle(DWORD s)
{
	style = s;
}

void Wnd::SetStyleEx(DWORD s)
{
	styleEx = s;
}

void Wnd::SetClassName(LPCSTR name)
{
	lpClass = name;
}

void Wnd::SetSuperClassName(LPCSTR name)
{
	lpSuperClass = name;
}

void Wnd::SetWindowName(LPCSTR name)
{
	lpText = name;
}

void Wnd::Hide()
{
	assert(m_hwnd);
	ShowWindow(m_hwnd, SW_HIDE);
}

void Wnd::SetGeometry(const RECT & rc)
{
	::SetWindowPos(m_hwnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOZORDER);
}

void Wnd::SetGeometry(const RECT & rc, HWND zorder, UINT flag)
{
	::SetWindowPos(m_hwnd, zorder, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		flag);
}

bool Wnd::SetParent(HWND parent)
{
	return ::SetParent(m_hwnd, parent);
}

