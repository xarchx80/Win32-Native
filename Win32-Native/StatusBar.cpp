#include <PCH.h>
#include "StatusBar.h"


StatusBar::StatusBar(Wnd * parent)
	: Control(parent)
{
	lpClass = STATUSCLASSNAME;
	style = WS_VISIBLE;//| WS_BORDER | CCS_BOTTOM;
	hbrBkgorund = CreateSolidBrush(RGB(44, 137, 204));
	Create();
	
	std::vector<StatusBarPart> parts = {
	{"One", 150, NULL},
	{"two", 200, NULL },
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

LRESULT StatusBar::NotifyReflectEvent(Event & e)
{
	LOG("%s notify reflect", GetText());
	return LRESULT();
}

LRESULT StatusBar::DrawItemEvent(DRAWITEMSTRUCT* dip)
{
	
	if (dip->itemID == 2) {
		HDC dc = dip->hDC;
		RECT rc = dip->rcItem;
		//auto br = CreateSolidBrush(RGB(65,100,100));
		SetTextColor(dc, RGB(230, 230, 230));
		SetBkColor(dc, RGB(50, 50, 50));
		SetBkMode(dc, TRANSPARENT);
		//SelectObject(dc, hbrBkgorund);
		FillRect(dc, &rc, hbrBkgorund);
		int index = dip->itemID;

		const char* code = GetPartText(dip->itemID);
		int code_len = std::strlen(code);
		//TextOut(dc, 0, 0, code, code_len);

		DrawText(dc, code, -1, &dip->rcItem, DT_LEFT);
		//SAFE_DELETE_GDIOBJ(br);
	}
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
