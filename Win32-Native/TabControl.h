#pragma once

#include <Control.h>

struct TabItem
{
	Wnd* window;
	//TCITEM tcItem;
	const char* text;

};

class TabControl : public SuperClassControl
{
public:
	TabControl(Wnd* parent);
	virtual~TabControl();

	void SetItems(std::vector<TabItem> &items);

	LRESULT PaintEvent(Painter& p) override;
	LRESULT NotifyReflectEvent(Event &e) override;
	void OnResize(SizeEvent& e) override;

	int DeleteAllItems();
	int DeleteItem(int index);

	
	std::vector<Wnd*> m_TabWnds;
	Wnd* m_CurTabWnd = nullptr;
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