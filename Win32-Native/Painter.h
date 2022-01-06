#pragma once


class Painter
{
public:
	Painter(HWND hwnd);
	~Painter();
	bool Release();

	void DrawRect(const RECT &rc, HBRUSH br);
	void DrawRectangle(const RECT &rc);
	HGDIOBJ SetObject(HGDIOBJ obj);
	
	HDC dc = nullptr;
	HWND hwnd = nullptr;
	PAINTSTRUCT ps;
};

