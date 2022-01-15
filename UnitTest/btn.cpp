#include <PCH.h>
#include <Application.h>


class Wnd
{
public:
	Wnd(HWND parent)
		: mParentHwnd(parent)
	{
		style = WS_VISIBLE;
	}
	virtual HWND Create(int x, int y, int w, int h, LPCSTR lpName, LPCSTR lpClass)
	{
		HINSTANCE inst = GetModuleHandle(NULL);
		WNDCLASSEX wc{};
		wc.cbSize = sizeof(WNDCLASSEX);
		if (!GetClassInfoEx(inst, lpClass, &wc)) {
			wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hInstance = inst;
			wc.lpfnWndProc = Wnd::WndProc;
			wc.lpszClassName = lpClass;
			wc.style = CS_VREDRAW | CS_VREDRAW;
			LOG("custom");
			
		}
		RegisterClassEx(&wc);
		
		if (mParentHwnd) {
			style |= WS_CHILD;
		}
		else style |= WS_POPUP;

		mHwnd = CreateWindowEx(NULL, wc.lpszClassName, lpName, style,
			x, y, w, h, mParentHwnd, (HMENU)NULL, inst, this);
		mControl[mHwnd] = this;
		return mHwnd;

	}
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	void Show() { ShowWindow(mHwnd, TRUE); }
	HWND mHwnd = nullptr;
	HWND mParentHwnd = nullptr;
	DWORD style;
	std::string mString;
	LPCSTR GetText() {
		int l = GetWindowTextLength(mHwnd) + 1;
		mString.clear();
		mString.reserve(l);
		GetWindowText(mHwnd, (LPSTR)mString.c_str(), l);
		return mString.c_str();
	}
	virtual LRESULT OnCtrlColor(WPARAM wp) {
		return FALSE;
	}
	static std::unordered_map<HWND, Wnd*> mControl;
	inline static Wnd* FindControl(HWND hwnd) {
		if (mControl.find(hwnd) != mControl.end()) {
			return mControl[hwnd];
		}
		return nullptr;
	}
};
std::unordered_map<HWND, Wnd*> Wnd::mControl;
class SubClass : public Wnd
{
public:
	SubClass(HWND parent) : Wnd(parent) {

	}
	virtual HWND Create(int x, int y, int w, int h, LPCSTR lpName, LPCSTR lpClass)
	{
		Wnd::Create(x, y, w, h, lpName, lpClass);
		SetWindowLongPtr(mHwnd, GWLP_USERDATA, (LONG)this);
		SetWindowSubclass(mHwnd, SubClassWndProc, (UINT_PTR)this, (DWORD_PTR)this);
		return mHwnd;
	}
	virtual LRESULT OnCtrlColor(WPARAM wp) {
		HDC dc = (HDC)wp;
		SetBkColor(dc, RGB(0, 0, 0));
		SetTextColor(dc, RGB(50, 150, 220));
		return (LRESULT)(HBRUSH)GetStockObject(BLACK_BRUSH);
	}
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI SubClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
		UINT_PTR id, DWORD_PTR data);
};

LRESULT SubClass::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	int aa = 10;

	switch (msg)
	{
	case WM_CTLCOLORBTN:
		LOG("btn");
		break;
	default:
		break;
	}
	return DefSubclassProc(hwnd, msg, wp, lp);
}

LRESULT SubClass::SubClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
	UINT_PTR id, DWORD_PTR data)
{
	Wnd* wnd = (Wnd*)data;
	if (wnd) 
		return wnd->LocalWndProc(hwnd, msg, wp, lp);
	return DefSubclassProc(hwnd, msg, wp, lp);
}

int main(int args, char* argv[])
{
	Application app;

	Wnd w(nullptr);
	w.style |= WS_POPUPWINDOW;
	w.Create(600, 180, 700, 300, "main window", "win32");
	//w.Show();
	SetWindowLong(w.mHwnd, GWL_STYLE, WS_VISIBLE | WS_OVERLAPPEDWINDOW);
	SubClass b(w.mHwnd);
	b.Create(100, 30, 300, 100, "button", WC_BUTTON);
	//b.Show();
	w.Show();

	return app.Exec();
}

LRESULT Wnd::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Wnd* ctrl = nullptr;
	switch (msg)
	{
	case WM_COMMAND:
		LOG("%s command", GetText());
		break;
	case WM_CTLCOLORBTN:
	{	
		LOG("ctrl");
		HWND h = (HWND)lp;
		ctrl = FindControl(h);
		if (ctrl) {
			LOG("%s success", ctrl->GetText());
			return ctrl->OnCtrlColor(wp);
		}
		//ctrl = (Wnd*)GetWindowLongPtr((HWND)lp, GWLP_WNDPROC);
		//if (ctrl) {
		//	LOG("%s controls", ctrl->GetText());
		//}
		//return (LRESULT)(HBRUSH)GetStockObject(BLACK_BRUSH);
	}	break;
	default:
		break;
	}
	return DefWindowProc(hwnd, msg, wp, lp);
}

LRESULT Wnd::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Wnd* wnd = (Wnd*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (msg == WM_NCCREATE) {
		wnd = (Wnd*)((LPCREATESTRUCT)lp)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG)wnd);
	}
	if (wnd) return wnd->LocalWndProc(hwnd, msg, wp, lp);

	return DefWindowProc(hwnd, msg, wp, lp);
}
