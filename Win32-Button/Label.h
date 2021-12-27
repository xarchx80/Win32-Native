#pragma once

#include <Control.h>

class Label : public Control
{
public:
	Label(Wnd* parent, int x, int y, int w, int h, const char* text);

	virtual LRESULT DrawItemEvent(DRAWITEMSTRUCT* dis) override;
	//virtual LRESULT LocalWndProc(HWND, UINT, WPARAM, LPARAM);
};

