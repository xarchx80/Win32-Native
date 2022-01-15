#pragma once


class Painter
{
public:
	Painter(HWND hwnd);
	Painter(HWND hwnd, HDC hdc);
	~Painter();
	bool Release();

	void DrawRect(const RECT &rc, HBRUSH br);
	void DrawRectangle(const RECT &rc);
	void DrawCode(LPCSTR code, LPRECT rc, UINT flag);
	HGDIOBJ SetObject(HGDIOBJ obj);
	
	HDC dc = nullptr;
	HWND hwnd = nullptr;
	PAINTSTRUCT ps;
};

