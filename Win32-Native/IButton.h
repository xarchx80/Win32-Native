#pragma once

#include <Control.h>

class IButton : public SubClassControl
{
public:
	IButton(Wnd* parent, LPCSTR text);
	IButton(Wnd* parent, int x, int y, int w, int h, LPCSTR text);

	virtual LRESULT OnCtrlColor(Event &e) override;
	//LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};

