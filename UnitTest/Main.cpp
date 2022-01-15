#include <PCH.h>
#include <Application.h>

static WNDPROC pStatusWndProc = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK StatusWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define WindowClass "window"
#define EditClass "EDIT"

struct 
{
	COLORREF clr = RGB(200, 220, 220);
	COLORREF clrBk = RGB(60, 60, 60);
	HBRUSH br = CreateSolidBrush(RGB(30, 150, 200));
	HBRUSH sbBr = CreateSolidBrush(RGB(200, 150, 30));
}custom;

void InitButtonClass(LPCSTR lpClass)
{
	HWND dummy_edit = CreateWindow(lpClass, "", 0, 0, 0, 200, 20, NULL, NULL, GetModuleHandle(NULL), NULL);
	pStatusWndProc = (WNDPROC)GetClassLongPtr(dummy_edit, GCLP_WNDPROC);
	SetClassLongPtr(dummy_edit, GCLP_WNDPROC, (LONG_PTR)StatusWndProc);
	DestroyWindow(dummy_edit);
}


WNDPROC PreWnd = nullptr;
//LRESULT WINAPI SubClassProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
//	UINT_PTR id, DWORD_PTR data);

int main(int args, char* argv[])
{
	Application app;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	InitButtonClass(STATUSCLASSNAME);
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX),
		CS_HREDRAW | CS_VREDRAW,
		WndProc,0,0,
		hInstance,NULL,LoadCursor(NULL,IDC_ARROW),
		CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW)),
		NULL,
		WindowClass,
		NULL };
	RegisterClassEx(&wc);

	

	HWND hwnd = CreateWindow(WindowClass, "UnitTest",
		WS_POPUP | WS_VISIBLE,
		600,200, 800, 500, NULL, NULL, hInstance, NULL);

	HWND status = CreateWindowEx(NULL, STATUSCLASSNAME, "status",
		WS_CHILD | WS_VISIBLE  | SBARS_TOOLTIPS,
		0, 0, 400, 100, hwnd, NULL, hInstance, NULL);

	int parts_width[3] = { 100,100 ,-1}; //"-1" end of array is fill remaining width
	int parts_num = sizeof(parts_width) / sizeof(int);

	const char* tip = { "hello" };
	SendMessage(status, SB_SETPARTS, parts_num, (LPARAM)parts_width);
	SendMessage(status, SB_SETTIPTEXT, 0, (LPARAM)tip);

	ShowWindow(hwnd, TRUE);

	return app.Exec();
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		char str[256];
		int len = GetWindowTextLength(hwnd) + 1;
		GetWindowText(hwnd, str, len);
		LOG("%s create", str);
	}
	break;
	case WM_PAINT:
	{
		char str[256];
		int len = GetWindowTextLength(hwnd) + 1;
		GetWindowText(hwnd, str, len);
		
		LOG("%s painting", str);
		PAINTSTRUCT ps;
		HDC dc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		return 0;
		break;
	}
	case WM_DESTROY: PostQuitMessage(0); return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


LRESULT CALLBACK StatusWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC dc;
	RECT rc;
	PAINTSTRUCT ps;
	int len, nParts = 0;
	int iPartsCoords[128];
	switch (msg)
	{
	case WM_NCCREATE:
	{
		char str[256];
		int len = GetWindowTextLength(hwnd) + 1;
		GetWindowText(hwnd, str, len);
		LOG("%s nc create", str);
	}
	case WM_NCCALCSIZE:
	{
		break;
	}
	case WM_ERASEBKGND:
	{
		break;
		dc = (HDC)wp;
		GetClientRect(hwnd, &rc);
		FillRect(dc, &rc, custom.br);
		return 1;
	}
	case WM_PAINT:
	{
		dc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		
		FillRect(dc, &rc, custom.br);

		nParts = SendMessage(hwnd, SB_GETPARTS, 
			sizeof(iPartsCoords) / sizeof(int), (WPARAM)iPartsCoords);
		struct {
			int x,y,spacing;
		}border;
		SendMessage(hwnd, SB_GETBORDERS, 0, (LPARAM)&border);

		for (int i = 0; i < nParts; i++)
		{
			SendMessage(hwnd, SB_GETRECT, i, (LPARAM)&rc);
			//remove defualt border & scpacing
			rc.top -= border.y;
			rc.left -= border.spacing;
			//FillRect(dc, &rc, custom.sbBr);
			Rectangle(dc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
			//retore rect
			rc.top += border.y;
			rc.left += border.spacing;
			
			SetBkMode(dc, TRANSPARENT);
			SetTextColor(dc, custom.clr);
			LRESULT res = SendMessage(hwnd, SB_GETTEXTLENGTH, i, 0);
			len = LOWORD(res);
			LPSTR str[256];
			SendMessage(hwnd, SB_GETTEXT, i, (LPARAM)str);
			DrawText(dc, (char*)str, len, &rc, DT_CENTER | DT_SINGLELINE);
		}
		EndPaint(hwnd, &ps);
		return 0;
	}

	}
	return CallWindowProc(pStatusWndProc, hwnd, msg, wp, lp);
}
