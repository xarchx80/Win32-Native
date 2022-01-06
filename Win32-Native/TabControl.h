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

	void SetItems(std::vector<TabItem> &items);
	std::vector<TabItem> mTabItems;

	LRESULT PaintEvent(Painter& p) override;

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