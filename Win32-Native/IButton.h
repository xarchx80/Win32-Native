#pragma once

#include <Control.h>

class IButton : public SuperClassControl
{
public:
	IButton(Wnd* parent, LPCSTR text);
	IButton(Wnd* parent, int x, int y, int w, int h, LPCSTR text);

	LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) override;
};

