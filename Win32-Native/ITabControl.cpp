#include <PCH.h>
#include <ITabControl.h>

ITabControl::ITabControl(Wnd* parent)
	: SuperClassControl(parent), mCurTabWnd(nullptr)
{
	lpText = "ITabControl";
	lpClass = WC_TABCONTROL;
	lpSuperClass = WC_TABCONTROL_SUPER;
	style = WS_VISIBLE | WS_CLIPSIBLINGS;//| WS_CLIPCHILDREN;
	Create();
	TabCtrl_SetItemSize(mHwnd, 80, 22);
	//TabCtrl_SetPadding(mHwnd, 2,2);
	mFont = CreateFont(18, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE,
		ANSI_CHARSET, OUT_TT_PRECIS, CLIP_TT_ALWAYS,
		DEFAULT_QUALITY, FF_DONTCARE, "Segoe UI");
	SendMessage(mHwnd, WM_SETFONT, (WPARAM)mFont, TRUE);
}

ITabControl::~ITabControl()
{
	
}

void ITabControl::SetItems(std::vector<TabItem>& items)
{
	DeleteAllItems();
	mTabList.clear();
	Wnd* window = nullptr;
	for (size_t i = 0; i < items.size(); i++) 
	{
		window = items[i].window;
		TCITEM item;
		item.mask = TCIF_TEXT | TCIF_PARAM;
		item.iImage = -1;
		item.pszText = (LPSTR)items[i].text;
		item.cchTextMax = strlen(item.pszText) + 1;
		HWND parent = nullptr;
		if (window) {
			HWND parent = GetParent(window->mHwnd);
			if (!parent && window->HasStyle(WS_POPUP)) {
				window->RemoveStyle(WS_POPUP);
			}
			window->AddStyle(WS_CHILD);
			window->SetParent(mHwnd);
		}
		
		item.lParam = (LONG_PTR)window;
		mTabList.push_back((Wnd*)item.lParam);
		SendMessage(mHwnd, TCM_INSERTITEM, i, (LPARAM)&item);
	}
	RECT rc;
	GetClientRect(mHwnd, &rc);
	TabCtrl_AdjustRect(mHwnd, FALSE, &rc);

	for (auto w : mTabList) {
		if (!w) continue;
		w->SetGeometry(rc);
		w->Hide();
	}
	int selected = TabCtrl_GetCurSel(mHwnd);
	if (mTabList.size()) {
		mCurTabWnd = mTabList[selected];
		if (mCurTabWnd) mCurTabWnd->Show();
	}
}

void ITabControl::InsertItem(const TabItem& item)
{
	mTabList.push_back(item.window);
	Wnd* wnd = item.window;
	TCITEM tcitem;
	tcitem.mask = TCIF_TEXT | TCIF_PARAM;
	tcitem.iImage = -1;
	tcitem.pszText = (LPSTR)item.text;
	tcitem.cchTextMax = strlen(tcitem.pszText) + 1;
	int nLastIndex = TabCtrl_GetItemCount(mHwnd);

	HWND parent = nullptr;
	if (item.window) {
		HWND parent = GetParent(wnd->mHwnd);
		if (!parent && wnd->HasStyle(WS_POPUP)) {
			wnd->RemoveStyle(WS_POPUP);
		}
		wnd->AddStyle(WS_CHILD);
		wnd->SetParent(mHwnd);
	}
	TabCtrl_InsertItem(mHwnd, nLastIndex, &tcitem);
	UpdateTabList();
}

LRESULT ITabControl::OnNotifyReflect(Event& e)
{
	NMHDR* hdr = (LPNMHDR)e.lp;
	switch (hdr->code)
	{
	case TCN_SELCHANGE:
	{
		for (auto w : mTabList) {
			if (!w) continue;
			w->Hide();
		}
		LPARAM lp = (LONG_PTR)mTabList.data();
		WPARAM wp = mTabList.size();
		PostMessage(mHwnd, UWM_HIDEWINDOWS, wp, lp);
		int select = TabCtrl_GetCurSel(mHwnd);
		if (mTabList.size()) {
			mCurTabWnd = mTabList[select];
		}
		break;
	}
	}
	if (mCurTabWnd) mCurTabWnd->Show();
	return 0;
}

LRESULT ITabControl::OnPaint(Painter& p)
{
	static HBRUSH br1 = CreateSolidBrush(RGB(40, 60, 80));
	static HBRUSH br2 = CreateSolidBrush(RGB(60, 120, 190));
	static HBRUSH br3 = CreateSolidBrush(RGB(160, 120, 60));
	static HPEN pen1 = CreatePen(PS_SOLID, 1, RGB(50, 150, 200));

	int nItems = SendMessage(mHwnd, TCM_GETITEMCOUNT, 0, 0);
	int select = SendMessage(mHwnd, TCM_GETCURSEL, 0, 0);

	RECT rc, trc;
	GetClientRect(mHwnd, &rc);
	p.SetObject(br1);
	p.SetObject(pen1);
	p.SetObject(mFont);
	p.DrawRect(rc, br1);
	SetBkMode(p.dc, TRANSPARENT);

	for (int i = 0; i < nItems; i++) {
		std::unique_ptr<CHAR> pstr(new char[256]);
		TCITEM item{};
		item.mask = TCIF_TEXT | TCIF_PARAM;
		item.cchTextMax = 256;
		item.pszText = pstr.get();
		TabCtrl_GetItem(mHwnd, i, &item);
		TabCtrl_GetItemRect(mHwnd, i, &trc);
		p.DrawRect(trc, br2);
		SetTextColor(p.dc, RGB(180, 180, 180));
		if (select == i)
			SetTextColor(p.dc, RGB(230, 230, 230));
		DrawText(p.dc, item.pszText, strlen(item.pszText), &trc, DT_CENTER |
			DT_VCENTER | DT_SINGLELINE);
	}

	TabCtrl_AdjustRect(mHwnd, FALSE, &rc);

	if (mCurTabWnd == nullptr) {
		p.DrawRect(rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	}
	return TRUE;
}
void ITabControl::OnResize(int w, int h)
{
	RECT rc;
	GetClientRect(mHwnd, &rc);
	TabCtrl_AdjustRect(mHwnd, FALSE, &rc);

	for (auto w : mTabList) {
		if (!w) continue;
		w->SetGeometry(rc);
	}
	InvalidateRect(mHwnd, NULL, TRUE);
}


LRESULT ITabControl::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	Event evt{ hwnd, msg, wp, lp };
	switch (msg)
	{
	case WM_CREATE:
		LOG("Created %s", GetText());
		break;
	case WM_COMMAND:
		LOG("command tab hooked %s", GetText());
		break;
	case WM_MOUSEHOVER:
		LOG("hoover");
		break;
	case WM_NOTIFY:
	{
		HWND child = (HWND)((LPNMHDR)lp)->hwndFrom;
		if (child) {
			LOG("notify");
		}
		BOOL res = OnNotify(evt);
		if (res) return res;
		break;
	}
	case WM_WINDOWPOSCHANGED:
	{
		int w = LOWORD(lp);
		int h = HIWORD(lp);
		OnResize(w,h);
		LOG("%s sized %d %d", GetText(), w, h);
		break;
	}
	case WM_PAINT:
	{
		Painter p(hwnd);
		OnPaint(p);
		break;
	}
	case UWM_HIDEWINDOWS:
		int nItems = wp;
		if (!nItems) return FALSE;
		Wnd** wndArray = (Wnd**)lp;
		Wnd* wnd = nullptr;
		for (int i = 0; i < nItems; ++i) {
			wnd = wndArray[i];
			if (wnd == nullptr) continue;
			wnd->Hide();
		}	
		return TRUE;
	}
	
	return LocalDefWndProc(hwnd, msg, wp, lp);
}

int ITabControl::GetTabCount() const
{
	return SendMessage(mHwnd, TCM_GETITEMCOUNT, 0, 0);
}

int ITabControl::GetTabSelectedIndex() const
{
	return SendMessage(mHwnd, TCM_GETCURSEL, 0, 0);
}

int ITabControl::SetTabItemSize(int w, int h)
{
	return TabCtrl_SetItemSize(mHwnd, w, h);
}

RECT ITabControl::GetAdjustWndRect() const
{
	RECT rc;
	GetClientRect(mHwnd, &rc);
	TabCtrl_AdjustRect(mHwnd, FALSE, &rc);
	return rc;
}

RECT ITabControl::GetAdjustItemRect(int index) const
{
	assert(index >= GetTabCount());
	RECT rc;
	TabCtrl_GetItemRect(mHwnd, index, &rc);
	return rc;
}

int ITabControl::DeleteAllItems()
{
	return TabCtrl_DeleteAllItems(mHwnd);
}

int ITabControl::DeleteItem(int index)
{
	return TabCtrl_DeleteItem(mHwnd, index);
}

void ITabControl::UpdateTabList()
{
	RECT rc;
	GetClientRect(mHwnd, &rc);
	TabCtrl_AdjustRect(mHwnd, FALSE, &rc);

	for (auto w : mTabList) {
		if (!w) continue;
		w->SetGeometry(rc);
		w->Hide();
	}
	int selected = TabCtrl_GetCurSel(mHwnd);
	if (mTabList.size()) {
		mCurTabWnd = mTabList[selected];
		if (mCurTabWnd) mCurTabWnd->Show();
	}
}
