#pragma once

#include <Control.h>

struct TabItem
{
	Wnd* window;
	const char* text;

};

#define UWM_HIDEWINDOWS		(WM_USER) + 400

typedef std::vector<Wnd*> WndList;

class ITabControl : public SuperClassControl
{
public:
	ITabControl(Wnd* parent);
	virtual~ITabControl();

	void	SetItems(std::vector<TabItem> &items);
	void	InsertItem(const TabItem& i);

	LRESULT OnPaint(Painter& p) override;
	LRESULT OnNotifyReflect(Event &e) override;
	void	OnResize(int w, int h) override;

	int		GetTabCount() const;
	int		GetTabSelectedIndex() const;
	int		SetTabItemSize(int w, int h);
	RECT	GetAdjustWndRect() const;
	RECT	GetAdjustItemRect(int index) const;
	int		DeleteAllItems();
	int		DeleteItem(int index);
	void	UpdateTabList();

	std::vector<Wnd*>	mTabList;
	Wnd*				mCurTabWnd;
	LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
};

//typedef struct tagTCITEMA
//{
//	UINT mask;
//	DWORD dwState;
//	DWORD dwStateMask;
//	LPSTR pszText;
//	int cchTextMax;
//	int iImage;
//
//	LPARAM lParam;
//} TCITEMA, *LPTCITEMA;