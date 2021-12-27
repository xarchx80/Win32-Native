#include <PCH.h>
#include "Painter.h"

IPainter::IPainter(HWND hwnd)
	: hwnd(hwnd)
{
	dc = BeginPaint(hwnd, &ps);
}

IPainter::~IPainter()
{
	EndPaint(hwnd, &ps);
}

bool IPainter::Release()
{
	
	return true;
}
