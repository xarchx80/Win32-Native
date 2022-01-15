#include <PCH.h>
#include "Label.h"

Label::Label(Wnd * parent, int x, int y, int w, int h, const char * text)
	: SubClassControl(parent, x, y, w, h, text)
{
	lpClass = WC_STATIC;
	style = WS_VISIBLE | SS_OWNERDRAW;
	id = 2;
	Create();
}

LRESULT Label::OnDrawItem(DRAWITEMSTRUCT * dis)
{
	LOG("draw item : %d ", dis->CtlID);
	RECT rc = dis->rcItem;
	HPEN pen = CreatePen(PS_SOLID, 1, RGB(150, 60, 60));
	SelectObject(dis->hDC, pen);
	HBRUSH br = CreateSolidBrush(RGB(30, 30, 30));
	SelectObject(dis->hDC, br);
	Rectangle(dis->hDC, rc.left, rc.top, rc.right, rc.bottom);
	//DrawEdge(dis->hDC, &rc, EDGE_ETCHED, BF_RECT | BF_ADJUST);

	SAFE_DELETE_GDIOBJ(pen);
	SAFE_DELETE_GDIOBJ(br);
	return LRESULT();
}

LRESULT Label::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		LOG("create");
		break;
	case WM_DRAWITEM:
	{
		//if(wp == 2)
		//	LOG("static draw");
		break;
	}
	}
	return LocalDefWndProc(hwnd, msg, wp, lp);
}
