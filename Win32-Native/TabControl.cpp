#include <PCH.h>
#include <TabControl.h>

TabControl::TabControl(Wnd* parent)
	: SuperClassControl(parent)
{
	lpText = "TabControl";
	lpClass = WC_TABCONTROL;
	lpSuperClass = "TABCONTROL_EX";
	style = WS_VISIBLE;
	Create();
}

void TabControl::SetItems(std::vector<TabItem>& items)
{
	Wnd* window = nullptr;
	for (int i = 0; i < items.size(); i++) {
		window = items[i].window;
		TCITEM item;
		item.mask = TCIF_TEXT;
		item.iImage = -1;
		item.pszText = (LPSTR)items[i].text;
		item.cchTextMax = strlen(item.pszText) + 1;
		SendMessage(m_hwnd, TCM_INSERTITEM, i, (LPARAM)&item);
		HWND parent = nullptr;
		if (window) {
			HWND parent = GetParent(window->m_hwnd);
			if (!parent && window->HasStyle(WS_POPUP)) {
				window->RemoveStyle(WS_POPUP);
			}
			window->AddStyle(WS_CHILD);
			window->SetParent(m_hwnd);
		}
		item.lParam = (LONG)window;
	}
}

LRESULT TabControl::PaintEvent(Painter& p)
{
	static HBRUSH br1 = CreateSolidBrush(RGB(40, 60, 80));
	static HBRUSH br2 = CreateSolidBrush(RGB(60, 120, 190));
	static HBRUSH br3 = CreateSolidBrush(RGB(160, 120, 60));
	static HPEN pen1 = CreatePen(PS_SOLID,1,RGB(50, 150, 200));

	RECT rc;
	GetClientRect(m_hwnd, &rc);
	p.SetObject(br1);
	p.SetObject(pen1);
	//p.DrawRectangle(rc);
	p.DrawRect(rc,br1);

	int itemNum = SendMessage(m_hwnd, TCM_GETITEMCOUNT, 0, 0);
	int curindex = SendMessage(m_hwnd, TCM_GETCURSEL, 0, 0);
	TCITEM item;

	for (int i = 0; i < itemNum; i++) {
		TabCtrl_GetItem(m_hwnd, i, &item);
		TabCtrl_GetItemRect(m_hwnd, i, &rc);
		p.SetObject(br2);
		
		if (i == curindex) {
			p.DrawRect(rc, br3);
		}else p.DrawRect(rc, br2);


	}

	return TRUE;
}

LRESULT TabControl::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		LOG("Created %s", GetText());
		break;
	case WM_COMMAND:
		LOG("command tab hooked %s", GetText());
		break;
	case WM_PAINT:
		Painter p(hwnd);
		PaintEvent(p);
		break;
	}
	
	return CallWindowProc(m_preWndProc, hwnd, msg, wp, lp);
}
