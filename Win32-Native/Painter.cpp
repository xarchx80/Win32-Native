#include <PCH.h>
#include "Painter.h"

Painter::Painter(HWND hwnd)
	: hwnd(hwnd)
{
	dc = BeginPaint(hwnd, &ps);
}

Painter::Painter(HWND hwnd, HDC hdc)
	: dc(hdc), hwnd(hwnd)
{
}

Painter::~Painter()
{
	EndPaint(hwnd, &ps);
}

bool Painter::Release()
{
	
	return true;
}

void Painter::DrawRect(const RECT &rc, HBRUSH br)
{
	FillRect(dc, &rc, br);
}

void Painter::DrawRectangle(const RECT & rc)
{
	Rectangle(dc, rc.left, rc.top, rc.right, rc.bottom);
}

HGDIOBJ Painter::SetObject(HGDIOBJ obj)
{
	assert(dc);
	return SelectObject(dc, obj);
}
