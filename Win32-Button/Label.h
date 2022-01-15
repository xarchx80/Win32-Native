#pragma once

#include <Control.h>

class Label : public SubClassControl
{
public:
	Label(Wnd* parent, int x, int y, int w, int h, const char* text);

	virtual LRESULT OnDrawItem(DRAWITEMSTRUCT* dis) override;
	virtual LRESULT LocalWndProc(HWND, UINT, WPARAM, LPARAM) override;
};

