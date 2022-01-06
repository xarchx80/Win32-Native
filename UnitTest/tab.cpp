#include <PCH.h>
#include <Application.h>


LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT WINAPI TabWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

HBRUSH hbrBackground = CreateSolidBrush(RGB(50,60,88));

//Window User Class Name
#define WC_USER_WINDOW	"WindowClass"

//Window Supper Class
#define WC_SUPPER_TAB	"TAB_SUPER_CLASE"

static WNDPROC PreTabWndProc = nullptr;

struct
{
	HBRUSH hbrBrush = CreateSolidBrush(RGB(80,82,88));
	HBRUSH hbrBrush1 = CreateSolidBrush(RGB(180, 90, 20));
	HBRUSH hbrBrush2 = CreateSolidBrush(RGB(80, 120, 110));
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
}pallete;

//#include <OleCtl.h>
int main(int args, char* argv[]) 
{
	Application app;
	HINSTANCE inst = GetModuleHandle(NULL);

	

	HWND dummy = CreateWindow(WC_TABCONTROL, "", 0, 0, 0, 200, 20,
		HWND_DESKTOP,
		NULL, inst, NULL);
	PreTabWndProc = (WNDPROC)GetClassLongPtr(dummy, GCLP_WNDPROC);
	SetClassLongPtr(dummy, GCLP_WNDPROC, (LONG_PTR)TabWndProc);
	DestroyWindow(dummy);

	WNDCLASSEX wc{
		sizeof(WNDCLASSEX),
		CS_VREDRAW | CS_VREDRAW,
		WndProc,
		NULL, NULL,
		inst,NULL,LoadCursor(NULL,IDC_ARROW),
		hbrBackground, NULL, WC_USER_WINDOW, NULL};

	if (!RegisterClassEx(&wc))
		assert(0);

	HWND hwnd = CreateWindowEx(NULL, WC_USER_WINDOW, "New Window", WS_VISIBLE | WS_POPUP,
		600, 160, 800, 400, NULL, (HMENU)NULL, inst, NULL);

	ShowWindow(hwnd, TRUE);



	HWND tab = CreateWindowEx(NULL, WC_TABCONTROL, "tab", WS_VISIBLE | WS_CHILD,
		30, 30, 600, 300, hwnd, (HMENU)NULL, inst, NULL);
	TCITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = (char*)"Hello";
	TabCtrl_InsertItem(tab, 0, &item);
	item.mask = TCIF_TEXT;
	item.pszText = (char*)"Two";
	TabCtrl_InsertItem(tab, 1, &item);
	item.pszText = (char*)"Tree";
	TabCtrl_InsertItem(tab, 1, &item);

	TabCtrl_SetItemSize(tab, 200, 22);
	
	return app.Exec();
}

LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	return DefWindowProc(hwnd, msg, wp, lp);
}


LRESULT WINAPI TabWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC dc;
	RECT rc;
	RECT rcitem;
	int index;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		dc = BeginPaint(hwnd, &ps);

		GetClientRect(hwnd, &rc);

		TabCtrl_AdjustRect(hwnd, TRUE, &rc);

		FillRect(dc, &rc, pallete.hbrBrush);
		SelectObject(dc, pallete.hPen);
		SelectObject(dc, pallete.hbrBrush1);

		int num_tabs = TabCtrl_GetItemCount(hwnd);
		int cur_tab = TabCtrl_GetCurSel(hwnd);

		for (auto i = 0; i < num_tabs; i++) {
			TabCtrl_GetItemRect(hwnd, i, &rcitem);
			HBRUSH oldbr = nullptr;
			SelectObject(dc, pallete.hbrBrush);
			if (cur_tab == i)
				oldbr = (HBRUSH)SelectObject(dc, pallete.hbrBrush1);
			else
				if (oldbr)
					SelectObject(dc, oldbr);
			DrawRectangle(dc, rcitem);
			
			SetBkMode(dc, TRANSPARENT);
			SetTextColor(dc, RGB(220, 200, 200));
			std::unique_ptr<CHAR> pstr(new char[256]);
			TCITEM item;
			item.mask = TCIF_TEXT;
			item.cchTextMax = 256;
			item.pszText = pstr.get();
			if (!TabCtrl_GetItem(hwnd, i, &item))
				continue;

			LPSTR str = item.pszText;
			DrawText(dc, str, strlen(str), &rcitem, DT_CENTER |DT_VCENTER| DT_SINGLELINE);

			RECT r{0,0,600,300};
			SendMessage(hwnd, TCM_ADJUSTRECT, false, (LPARAM)&r);

			DrawRectangle(dc, r);

		}

		EndPaint(hwnd, &ps);
		return 0;
	}
		break;
	case WM_SIZE:
	{

	}
	break;
	}
	

	if (PreTabWndProc)
		return CallWindowProc(PreTabWndProc, hwnd, msg, wp, lp);
	else
		return DefWindowProc(hwnd, msg, wp, lp);
	
}