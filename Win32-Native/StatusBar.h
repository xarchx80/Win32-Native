#pragma once

#include <Control.h>

struct StatusBarPart
{
	LPCSTR code;
	UINT width;
	DWORD flag;
};

class StatusBar : public SubClassControl
{
public:
	StatusBar(Wnd* parent);
	
	//bool SetParts(StatusBarPart* data, int len);
	bool SetParts(std::vector<StatusBarPart> data);

	void SetPartText(UINT index, LPCSTR code);
	const char* GetPartText(UINT index);
	RECT GetPartRect(UINT index);
	RECT GetPartNoThemeRect(UINT index);
	int GetPartsNum() const;
	bool GetBorder(int* data);

	virtual LRESULT NotifyReflectEvent(Event &e) override;
	virtual LRESULT PaintEvent(Painter& p) override;
	virtual LRESULT DrawItemEvent(DRAWITEMSTRUCT* dip) override;
	virtual LRESULT EraseBkgndEvent(Event &e);

	std::string dummy_str;
	std::vector<const char*> m_parts_text;
};

//CtlType	Undefined; do not use.
//CtlID	Child window identifier of the status bar.
//itemID	Zero - based index of the part to be drawn.
//itemAction	Undefined; do not use.
//itemState	Undefined; do not use.
//hwndItem	Handle to the status bar.
//hDC	Handle to the device context of the status bar.
//rcItem	Coordinates of the window part to be drawn.The coordinates are relative to the upper left corner of the status bar.
//itemData	Application - defined 32 - bit value specified in the lParam parameter of the SB_SETTEXT message.
