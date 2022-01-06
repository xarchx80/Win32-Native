#include <PCH.h>
#include "StatusBar.h"


StatusBar::StatusBar(Wnd * parent)
	: SubClassControl(parent)
{
	lpClass = STATUSCLASSNAME;
	style = WS_VISIBLE | WS_CLIPSIBLINGS;//| WS_BORDER | CCS_BOTTOM;
	styleEx = WS_EX_CLIENTEDGE;
	hbrBkgorund = CreateSolidBrush(RGB(44, 137, 204));
	Create();
	
	std::vector<StatusBarPart> parts = {
	{"One", 150, SBT_OWNERDRAW},
	{"two", 200, SBT_OWNERDRAW | SBT_NOBORDERS},
	{"Three", 100, SBT_OWNERDRAW | SBT_NOBORDERS }
	};

	SetParts(parts);
	SendMessage(m_hwnd, SB_SIMPLE, FALSE, 0);
	//LOG(GetPartText(2));
	SendMessage(m_hwnd, SB_SETBKCOLOR, NULL, (LPARAM)RGB(50, 50, 50));
}


bool StatusBar::SetParts(std::vector<StatusBarPart> data)
{
	assert(IsWindow(m_hwnd));
	if (data.size() <= 0 && data.size() > 250)
		return false;
	int len = data.size();
	std::vector<int> offsets;
	m_parts_text.clear();

	int offset = 0;
	for (int i = 0; i < len; i++) {
		offset += data[i].width;
		offsets.push_back(offset);
	}
		
	SendMessage(m_hwnd, SB_SETPARTS, (WPARAM)len, (LPARAM)offsets.data());

	for (int i = 0; i < len; ++i) {
		int flag = data[i].flag;
		auto code = data[i].code;
		m_parts_text.push_back(data[i].code);
		bool b = SendMessage(m_hwnd, SB_SETTEXT, i | flag, (LPARAM)code);
		if (!b)
			LOG("warning");
		
	}
	return true;
}



void StatusBar::SetPartText(UINT index, LPCSTR code)
{
	assert(IsWindow(m_hwnd));
	assert(index >= m_parts_text.size());
	m_parts_text[index] = code;
	SendMessage(m_hwnd, SB_SETTEXT, index, (LPARAM)code);
}

const char* StatusBar::GetPartText(UINT index)
{
	int len = LOWORD(SendMessage(m_hwnd, SB_GETTEXTLENGTH, (WPARAM)index, 0));
	dummy_str.clear();
	dummy_str.reserve(len);
	SendMessage(m_hwnd, SB_GETTEXT, index, (LPARAM)dummy_str.data());
	if (!dummy_str.empty())
		return dummy_str.c_str();
	assert(index <= m_parts_text.size());
	return m_parts_text[index];
}

RECT StatusBar::GetPartRect(UINT index)
{
	assert(index <= m_parts_text.size());
	RECT rc;
	SendMessage(m_hwnd, SB_GETRECT, index, (LPARAM)&rc);
	return rc;
}

RECT StatusBar::GetPartNoThemeRect(UINT index)
{
	return RECT();
}

int StatusBar::GetPartsNum() const
{
	return SendMessage(m_hwnd, SB_GETPARTS, 0, 0);
}

bool StatusBar::GetBorder(int* data)
{
	int size[3];
	bool res = SendMessage(m_hwnd, SB_GETBORDERS, 0, (LPARAM)&size);
	if (res) {
		data[0] = size[0];
		data[1] = size[1];
		data[2] = size[2];
	}

	return res;
}

LRESULT StatusBar::NotifyReflectEvent(Event & e)
{
	LOG("%s notify reflect", GetText());
	return LRESULT();
}

LRESULT StatusBar::PaintEvent(Painter &p)
{
	RECT rc = GetPartRect(0);
	FillRect(p.dc, &rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	return TRUE;
}

LRESULT StatusBar::DrawItemEvent(DRAWITEMSTRUCT* dip)
{
	

	HDC dc = dip->hDC;
	RECT rc = dip->rcItem;
	int index = dip->itemID;
	//auto br = CreateSolidBrush(RGB(65,100,100));
	SetTextColor(dc, RGB(230, 230, 230));
	SetBkColor(dc, RGB(50, 50, 50));
	SetBkMode(dc, TRANSPARENT);
	//SelectObject(dc, hbrBkgorund);
	if (index == 1) {
		auto rrc = GetPartRect(index);
		rrc.left -= 20;
		rrc.top -= 2;
		FillRect(dc, &rrc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	}
	else
		FillRect(dc, &rc, hbrBkgorund);

	const char* code = GetPartText(dip->itemID);
	int code_len = std::strlen(code);
	//TextOut(dc, 0, 0, code, code_len);

	DrawText(dc, code, -1, &dip->rcItem, DT_LEFT);
		//SAFE_DELETE_GDIOBJ(br);
	
	return true;
}

LRESULT StatusBar::EraseBkgndEvent(Event &e)
{
	HDC dc = (HDC)e.wp;
	RECT rc;
	GetClientRect(m_hwnd, &rc);
	
	static auto pen = CreatePen(PS_SOLID, 1, RGB(50, 50, 50));
	SelectObject(dc, pen);
	FillRect(dc, &rc, hbrBkgorund);
	
	return TRUE;
}
