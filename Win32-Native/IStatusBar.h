#pragma once

#include <Control.h>

struct StatusItem
{
	LPCSTR code;
	int width;
	DWORD flag;
};

class IStatusBar : public SubClassControl
{
public:
	IStatusBar(Wnd* parent);
	
	bool SetItems(const std::vector<StatusItem>& data);
	void SetItemText(UINT index, LPCSTR code);
	LPCSTR	GetItemText(UINT index);
	RECT	GetItemRect(UINT index);
	int		GetItemCount() const;
	bool	GetBorder(int* pSize);

	virtual LRESULT OnNotifyReflect(Event &e) override;
	virtual LRESULT OnPaint(Painter& p) override;
	virtual LRESULT OnDrawItem(DRAWITEMSTRUCT* dip) override;
	virtual LRESULT OnEraseBkgnd(Event &e);

	std::string dummy_str;
	std::vector<LPCSTR> mItemTexts;
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
