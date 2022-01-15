#include <PCH.h>
#include "IStatusBar.h"


IStatusBar::IStatusBar(Wnd * parent)
	: SubClassControl(parent)
{
	lpClass = STATUSCLASSNAME;
	style = WS_VISIBLE | WS_CLIPSIBLINGS;//| WS_BORDER | CCS_BOTTOM;
	styleEx = NULL;
	mBkBrush = CreateSolidBrush(RGB(44, 137, 204));
	mIsPaintEventEnable = true;
	Create();
	
	std::vector<StatusItem> parts = {
	{"One", 150, 0},
	{"two", 150, 0},
	{"Three", -1, 0 }
	};

	SendMessage(mHwnd, SB_SIMPLE, FALSE, 0);
	SetItems(parts);
}


bool IStatusBar::SetItems(const std::vector<StatusItem>& data)
{
	assert(IsWindow(mHwnd));
	if (data.size() <= 0 && data.size() > 256)
		return false;
	int len = data.size();
	std::vector<int> offsets;
	mItemTexts.clear();

	int offset = 0;
	for (int i = 0; i < len; i++) {
		if (data[i].width < 0) {
			offsets.push_back(-1);
			continue;
		}
		offset += data[i].width;
		offsets.push_back(offset);
	}
	
	SendMessage(mHwnd, SB_SETPARTS, (WPARAM)len, (LPARAM)offsets.data());

	for (int i = 0; i < data.size(); i++) {
		int flag = data[i].flag;
		auto code = data[i].code;
		mItemTexts.push_back(data[i].code);
		SendMessage(mHwnd, SB_SETTEXT, MAKELONG(i,flag), (LPARAM)code);		
	}

	ValidateRect(mHwnd, NULL);
	return true;
}



void IStatusBar::SetItemText(UINT index, LPCSTR code)
{
	assert(IsWindow(mHwnd));
	assert(index >= mItemTexts.size());
	mItemTexts[index] = code;
	SendMessage(mHwnd, SB_SETTEXT, index, (LPARAM)code);
}

LPCSTR IStatusBar::GetItemText(UINT index)
{
	int len = LOWORD(SendMessage(mHwnd, SB_GETTEXTLENGTH, (WPARAM)index, 0));
	dummy_str.clear();
	dummy_str.reserve(len);
	SendMessage(mHwnd, SB_GETTEXT, index, (LPARAM)dummy_str.data());
	if (!dummy_str.empty())
		return dummy_str.c_str();
	assert(index <= mItemTexts.size());
	return mItemTexts[index];
}

RECT IStatusBar::GetItemRect(UINT index)
{
	//assert(index <= mItemTexts.size());
	RECT rc;
	SendMessage(mHwnd, SB_GETRECT, index, (LPARAM)&rc);
	return rc;
}

int IStatusBar::GetItemCount() const
{
	return SendMessage(mHwnd, SB_GETPARTS, 0, 0);
}

bool IStatusBar::GetBorder(int* pSize)
{
	return SendMessage(mHwnd, SB_GETBORDERS, 0, (LPARAM)*pSize);
}

LRESULT IStatusBar::OnNotifyReflect(Event& e)
{
	LOG("%s notify reflect", GetText());
	return LRESULT();
}

LRESULT IStatusBar::OnPaint(Painter &p)
{
	RECT rc;
	int border[3];
	SendMessage(mHwnd, SB_GETBORDERS, 0, (LPARAM)&border);
	for (int i = 0; i < GetItemCount(); i++)
	{
		p.SetObject(mBkBrush);
		rc = GetItemRect(i);
		rc.top -= border[1];
		rc.left -= border[2];
		p.DrawRect(rc, mBkBrush);
		rc.top += border[1];
		rc.left += border[2];
		const char* code[256];
		SendMessage(mHwnd, SB_GETTEXT, LOWORD(i), (LPARAM)code);
		SetBkMode(p.dc, TRANSPARENT);
		SetTextColor(p.dc, RGB(200, 200, 200));
		p.DrawCode((LPCSTR)code, &rc, DT_CENTER);
	}

	return TRUE;
}

LRESULT IStatusBar::OnDrawItem(DRAWITEMSTRUCT* dip)
{
	HDC dc = dip->hDC;
	RECT rc = dip->rcItem;
	int index = dip->itemID;
	return true;
}

LRESULT IStatusBar::OnEraseBkgnd(Event &e)
{
	HDC dc = (HDC)e.wp;
	RECT rc;
	GetClientRect(mHwnd, &rc);
	FillRect(dc, &rc, mBkBrush);
	return TRUE;
}
