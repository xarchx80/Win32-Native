#pragma once

#include <unordered_map>

class Wnd;
class WndCache
{
public:
	bool Insert(HWND hwnd, Wnd* wnd);
	std::unordered_map<HWND, Wnd*> mWndCache;
};

