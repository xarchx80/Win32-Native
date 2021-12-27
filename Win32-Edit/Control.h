#pragma once

#include <Wnd.h>

class Control : public Wnd
{
public:
	Control(Wnd* parent);
	Control(Wnd* parent, int x, int y, int w, int h, const char* text);

	virtual bool Create() override;

	//virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI GlobalSubWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
		UINT_PTR id, DWORD_PTR data);
};

