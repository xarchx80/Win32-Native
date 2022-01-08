#include <PCH.h>
#include <TabControl.h>

TabControl::TabControl(Wnd* parent)
	: SuperClassControl(parent)
{
	lpText = "TabControl";
	lpClass = WC_TABCONTROL;
	lpSuperClass = "TABCONTROL_EX";
	style = WS_VISIBLE | WS_CLIPSIBLINGS;//| WS_CLIPCHILDREN;
	Create();	
}

TabControl::~TabControl()
{
}

void TabControl::SetItems(std::vector<TabItem>& items)
{
	Wnd* window = nullptr;
	for (int i = 0; i < items.size(); i++) 
	{
		window = items[i].window;
		TCITEM item;
		item.mask = TCIF_TEXT | TCIF_PARAM;
		item.iImage = -1;
		item.pszText = (LPSTR)items[i].text;
		item.cchTextMax = strlen(item.pszText) + 1;
		/*SendMessage(m_hwnd, TCM_INSERTITEM, i, (LPARAM)&item);*/
		HWND parent = nullptr;
		if (window) {
			HWND parent = GetParent(window->m_hwnd);
			if (!parent && window->HasStyle(WS_POPUP)) {
				window->RemoveStyle(WS_POPUP);
			}
			//window->RemoveStyle(WS_VISIBLE);
			window->AddStyle(WS_CHILD);
			window->SetParent(m_hwnd);
			//childs.emplace_back(window);
		}
		
		item.lParam = (LONG_PTR)window;
		m_TabWnds.push_back((Wnd*)item.lParam);
		SendMessage(m_hwnd, TCM_INSERTITEM, i, (LPARAM)&item);
	}
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	TabCtrl_AdjustRect(m_hwnd, FALSE, &rc);

	for (auto w : m_TabWnds) {
		if (!w) continue;
		w->SetGeometry(rc);
		w->Hide();
	}
	int selected = TabCtrl_GetCurSel(m_hwnd);
	if (m_TabWnds.size()) {
		m_CurTabWnd = m_TabWnds[selected];
		if (m_CurTabWnd) m_CurTabWnd->Show();
	}
}

LRESULT TabControl::NotifyReflectEvent(Event & e)
{
	NMHDR* hdr = (LPNMHDR)e.lp;
	switch (hdr->code)
	{
	case TCN_SELCHANGE:
	{
		for (auto w : m_TabWnds) {
			if (!w) continue;
			w->Hide();
		}
		int select = TabCtrl_GetCurSel(m_hwnd);
		if (m_TabWnds.size()) {
			m_CurTabWnd = m_TabWnds[select];
		}
		break;
	}
	}
	if (m_CurTabWnd) m_CurTabWnd->Show();
	return 0;
}

LRESULT TabControl::PaintEvent(Painter& p)
{
	static HBRUSH br1 = CreateSolidBrush(RGB(40, 60, 80));
	static HBRUSH br2 = CreateSolidBrush(RGB(60, 120, 190));
	static HBRUSH br3 = CreateSolidBrush(RGB(160, 120, 60));
	static HPEN pen1 = CreatePen(PS_SOLID, 1, RGB(50, 150, 200));

	int nItems = SendMessage(m_hwnd, TCM_GETITEMCOUNT, 0, 0);
	int select = SendMessage(m_hwnd, TCM_GETCURSEL, 0, 0);

	RECT rc, trc;
	GetClientRect(m_hwnd, &rc);
	p.SetObject(br1);
	p.SetObject(pen1);
	p.DrawRect(rc, br1);
	SetBkMode(p.dc, TRANSPARENT);

	for (int i = 0; i < nItems; i++) {
		std::unique_ptr<CHAR> pstr(new char[256]);
		TCITEM item{};
		item.mask = TCIF_TEXT | TCIF_PARAM;
		item.cchTextMax = 256;
		item.pszText = pstr.get();
		TabCtrl_GetItem(m_hwnd, i, &item);
		TabCtrl_GetItemRect(m_hwnd, i, &trc);
		p.DrawRect(trc, br2);
		SetTextColor(p.dc, RGB(210, 210, 210));
		if (select == i)
			SetTextColor(p.dc, RGB(230, 230, 230));
		DrawText(p.dc, item.pszText, strlen(item.pszText), &trc, DT_CENTER |
			DT_VCENTER | DT_SINGLELINE);
	}

	TabCtrl_AdjustRect(m_hwnd, FALSE, &rc);

	if (m_CurTabWnd == nullptr) {
		p.DrawRect(rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	}
	return TRUE;
}
void TabControl::OnResize(SizeEvent& e)
{

}


LRESULT TabControl::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Event evt{ msg, wp, lp };
	switch (msg)
	{
	case WM_CREATE:
		LOG("Created %s", GetText());
		break;
	case WM_COMMAND:
		LOG("command tab hooked %s", GetText());
		break;
	case WM_NOTIFY:
	{
		
		HWND child = (HWND)((LPNMHDR)lp)->hwndFrom;
		if (child) {
			LOG("notify");
		}
		BOOL res = NotifyEvent(evt);
		if (res) return res;
		LOG("noti");
		break;
	}
	case WM_SIZE:
	{
		SizeEvent se(wp, lp);
		OnResize(se);
		LOG("%s sized", GetText());
		break;
	}
	case WM_PAINT:
	{
		Painter p(hwnd);
		PaintEvent(p);
		break;
	}
	case WM_DISPLAYCHANGE:
	//{
	//	Painter p(hwnd);
	//	DoPaint(p);
	//	LOG("paint %s", GetText());
	//}
	//return 0;
	break;


	}
	
	return CallWindowProc(m_preWndProc, hwnd, msg, wp, lp);
}

int TabControl::DeleteAllItems()
{
	return TabCtrl_DeleteAllItems(m_hwnd);
}

int TabControl::DeleteItem(int index)
{
	return TabCtrl_DeleteItem(m_hwnd, index);
}
