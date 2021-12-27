#pragma once


class IPainter
{
public:
	IPainter(HWND hwnd);
	~IPainter();
	bool Release();

	
	HDC dc;
	HWND hwnd;
	PAINTSTRUCT ps;
};

