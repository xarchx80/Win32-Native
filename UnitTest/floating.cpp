#include <PCH.h>
#include <Application.h>

struct Window
{
	Window(Window* parent) : parent(parent){}
	HWND hwnd;
	Window* parent;
};

std::string GetText(HWND hwnd) {
	int len = GetWindowTextLength(hwnd) + 1;
	std::string str;
	str.reserve(len);
	GetWindowText(hwnd, (LPSTR)str.c_str(), len);
	return str;
}

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT WINAPI SubWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id,
	DWORD_PTR data);

int main(int args, char* argv[])
{
	Application a;
	HINSTANCE inst = GetModuleHandle(NULL);
	WNDCLASSEX wc{
		sizeof(WNDCLASSEX), CS_VREDRAW | CS_HREDRAW, WndProc,0,0, inst, 0,
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)GetStockObject(GRAY_BRUSH),
		0,"window",0
	};
	RegisterClassEx(&wc);

	HWND window = CreateWindowEx(NULL, "window", "main window", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		400, 200, 700, 400, NULL, NULL,GetModuleHandle(NULL), NULL);

	HWND button = CreateWindowEx(NULL, WC_BUTTON, "button", WS_POPUP  | WS_VISIBLE
		| BS_PUSHBUTTON,
		200, 50, 200, 100, NULL, NULL, GetModuleHandle(NULL), NULL);

	//SetWindowLongPtr(button)
	SetWindowSubclass(button, SubWndProc, (UINT_PTR)button, (DWORD_PTR)button);
	HWND parent = GetParent(button);
	auto s = GetText(parent);
	LOG("%s is parent", s.c_str());
	SetWindowLong(button, GWL_STYLE, WS_VISIBLE | WS_CHILD);
	SetParent(button, window);
	

	ShowWindow(window, TRUE);

	return a.Exec();
}



LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_SIZE:
		LOG("%s Sizong", GetText(hwnd).c_str());
		break;
		//case WM_COMMAND:
		//{
		//	HWND child = nullptr;
		//	child = (HWND)lp;
		//	if (child) {
		//		LOG("%s command", GetText(child).c_str());
		//		if (HIWORD(wp) == BN_CLICKED) {
		//			LOG("cliked %d", LOWORD(wp));
		//			ShowWindow(child, FALSE);
		//		}
		//		//return TRUE;
		//	}
		//		
		//	LOG("%s command", GetText(hwnd).c_str());
		//	break;
		//}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hwnd, &ps);
		RECT r;
		HBRUSH br;
		GetClientRect(hwnd, &r);
		br = (HBRUSH)GetStockObject(BLACK_BRUSH);
		FillRect(dc, &r, br);
		static HBRUSH hbr = CreateSolidBrush(RGB(0, 100, 100));
		r.bottom = 110;
		FillRect(dc, &r, hbr);
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_COMMAND:
	{
		HWND child = nullptr;
		child = (HWND)lp;
		if (child) {
			LOG("%s command", GetText(child).c_str());
			if (HIWORD(wp) == BN_CLICKED) {
				LOG("cliked %d", LOWORD(wp));
				ShowWindow(child, FALSE);
			}
			//return TRUE;
		}

		LOG("%s command", GetText(hwnd).c_str());
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wp, lp);

}

LRESULT WINAPI SubWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp, UINT_PTR id,
	DWORD_PTR data)
{
	
	switch (msg)
	{
	case WM_SIZE:
		LOG("%s sub wm_size", GetText(hwnd).c_str());
		break;
	case WM_COMMAND:
	{
		LOG("%s sub command", GetText(hwnd).c_str());
		ShowWindow(hwnd, SW_HIDE);
		break;
	}
	}
	return DefSubclassProc(hwnd, msg, wp, lp);
}