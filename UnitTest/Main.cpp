#include <PCH.h>
#include <Application.h>

static WNDPROC old_wndproc = nullptr;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK EditProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#define WindowClass "window"
#define EditClass "EDIT"

struct InitClass
{
	InitClass()
	{
		HWND dummy_edit = CreateWindow(EditClass, "", 0, 0, 0, 200, 20, NULL, NULL, GetModuleHandle(NULL), NULL);
		old_wndproc = (WNDPROC)GetClassLongPtr(dummy_edit, GCLP_WNDPROC);
		SetClassLongPtr(dummy_edit, GCLP_WNDPROC, (LONG_PTR)EditProc);
		DestroyWindow(dummy_edit);
	}
};

InitClass g_initclass;


int main(int args, char* argv[])
{
	Application app;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW,WndProc,0,0,hInstance,NULL,NULL,CreateSolidBrush(GetSysColor(COLOR_BTNSHADOW)),
		NULL,WindowClass,NULL };
	RegisterClassEx(&wcex);

	

	HWND hWnd = CreateWindow(WindowClass, "UnitTest", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);



	HWND edit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Ex_ClientEdge", WS_CHILD | WS_VISIBLE | WS_BORDER, 
		10, 90, 200, 24, hWnd, NULL, GetModuleHandle(NULL), NULL);

	CreateWindowEx(0, "EDIT", "no border", WS_CHILD | WS_VISIBLE, 10, 10, 200, 24, hWnd, 
		NULL, GetModuleHandle(NULL), NULL);
	CreateWindowEx(0, "EDIT", "no ex style", WS_CHILD | WS_VISIBLE | WS_BORDER, 
		10, 50, 200, 24, hWnd, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(hWnd, TRUE);

	LOG("hm?");

	return app.Exec();
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: PostQuitMessage(0); return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

static LRESULT WINAPI EditProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_NCCREATE:
	{
		DWORD style = GetWindowLong(hwnd, GWL_STYLE);
		if (style & WS_BORDER)
		{
			// WS_EX_CLIENTEDGE style will make the border 2 pixels thick...
			style = GetWindowLong(hwnd, GWL_EXSTYLE);
			if (!(style & WS_EX_CLIENTEDGE))
			{
				style |= WS_EX_CLIENTEDGE;
				SetWindowLong(hwnd, GWL_EXSTYLE, style);
			}
		}
		// to draw on the parent DC, CLIPCHILDREN must be off
		HWND hParent = GetParent(hwnd);
		style = GetWindowLong(hParent, GWL_STYLE);
		if (style & WS_CLIPCHILDREN)
		{
			style &= ~WS_CLIPCHILDREN;
			SetWindowLong(hParent, GWL_STYLE, style);
		}
	}
	break;
	case WM_NCPAINT:
	{
		if(GetWindowLong(hwnd, GWL_EXSTYLE) & WS_EX_CLIENTEDGE)
			LOG("ex styled client");
		return 0;
	}
	}

	return CallWindowProc(old_wndproc, hwnd, msg, wp, lp);
}
