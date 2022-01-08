#pragma once

#include <Wnd.h>

class SubClassControl : public Wnd
{
public:
	SubClassControl(Wnd* parent);
	SubClassControl(Wnd* parent, int x, int y, int w, int h, const char* text);
	virtual~SubClassControl();

	virtual bool Create() override;

	//virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI SubClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp,
		UINT_PTR id, DWORD_PTR data);
};

class SuperClassControl : public Wnd
{
public:
	SuperClassControl(Wnd* parent);
	SuperClassControl(Wnd* parent, int x, int y, int w, int h, const char* text);

	
	virtual bool SetSuperClass(LPCSTR src, LPCSTR dest) override;
	WNDPROC GetPreWndProc() const;
	int m_PreCBWndExtra = 0;
	WNDPROC m_preWndProc = nullptr;
	static std::vector<std::string> m_registeredSuperClasses;
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI SuperClassWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};
