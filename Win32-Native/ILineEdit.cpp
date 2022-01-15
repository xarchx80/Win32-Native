#include <PCH.h>
#include <ILineEdit.h>

ILineEdit::ILineEdit(Wnd* parent, int x, int y, int w, int h, LPCSTR text)
	: SubClassControl(parent, x, y, w, h, text)
{
	lpClass = WC_EDIT;
	lpSuperClass = "WC_EDIT_SUPER";
	SetStyle(WS_VISIBLE | ES_MULTILINE);
	Create();
	mBrush = CreateSolidBrush(RGB(24, 24, 24));
	Edit_SetText(mHwnd, "");
	
}

void ILineEdit::SetText(LPCSTR text)
{
	Edit_SetText(mHwnd, text);
}

void ILineEdit::SetReadOnly(bool b)
{
	Edit_SetReadOnly(mHwnd, b);
}

void ILineEdit::SetLimitText(int cMax)
{
	Edit_LimitText(mHwnd, cMax);
}

LRESULT ILineEdit::LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CTLCOLOREDIT:
	{
		HDC dc = (HDC)wp;
		SetBkColor(dc, RGB(100, 0, 0));
		SetTextColor(dc, RGB(220, 220, 220));
		return (LRESULT)mBrush;
	}
		break;

	}
	
	return LocalDefWndProc(hwnd, msg, wp, lp);
}
