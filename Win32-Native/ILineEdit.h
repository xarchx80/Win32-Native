#pragma once

#include <Control.h>

class ILineEdit : public SubClassControl
{
public:
	ILineEdit(Wnd* parent, int x, int y, int w, int h, LPCSTR text);
	HBRUSH mBrush;
	//Self Func
	void SetText(LPCSTR text);
	void SetReadOnly(bool b);
	void SetLimitText(int cMax);
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

	COLORREF mBkColor;
	COLORREF mTextColor;
};

//#define EM_GETSEL               0x00B0
//#define EM_SETSEL               0x00B1
//#define EM_GETRECT              0x00B2
//#define EM_SETRECT              0x00B3
//#define EM_SETRECTNP            0x00B4
//#define EM_SCROLL               0x00B5
//#define EM_LINESCROLL           0x00B6
//#define EM_SCROLLCARET          0x00B7
//#define EM_GETMODIFY            0x00B8
//#define EM_SETMODIFY            0x00B9
//#define EM_GETLINECOUNT         0x00BA
//#define EM_LINEINDEX            0x00BB
//#define EM_SETHANDLE            0x00BC
//#define EM_GETHANDLE            0x00BD
//#define EM_GETTHUMB             0x00BE
//#define EM_LINELENGTH           0x00C1
//#define EM_REPLACESEL           0x00C2
//#define EM_GETLINE              0x00C4
//#define EM_LIMITTEXT            0x00C5
//#define EM_CANUNDO              0x00C6
//#define EM_UNDO                 0x00C7
//#define EM_FMTLINES             0x00C8
//#define EM_LINEFROMCHAR         0x00C9
//#define EM_SETTABSTOPS          0x00CB
//#define EM_SETPASSWORDCHAR      0x00CC
//#define EM_EMPTYUNDOBUFFER      0x00CD
//#define EM_GETFIRSTVISIBLELINE  0x00CE
//#define EM_SETREADONLY          0x00CF
//#define EM_SETWORDBREAKPROC     0x00D0
//#define EM_GETWORDBREAKPROC     0x00D1
//#define EM_GETPASSWORDCHAR      0x00D2
//#if(WINVER >= 0x0400)
//#define EM_SETMARGINS           0x00D3
//#define EM_GETMARGINS           0x00D4
//#define EM_SETLIMITTEXT         EM_LIMITTEXT   /* ;win40 Name change */
//#define EM_GETLIMITTEXT         0x00D5
//#define EM_POSFROMCHAR          0x00D6
//#define EM_CHARFROMPOS          0x00D7
//#endif /* WINVER >= 0x0400 */
//
//#if(WINVER >= 0x0500)
//#define EM_SETIMESTATUS         0x00D8
//#define EM_GETIMESTATUS         0x00D9
//#endif /* WINVER >= 0x0500 */
//
//#if(WINVER >= 0x0604)
//#define EM_ENABLEFEATURE        0x00DA
//#endif /* WINVER >= 0x0604 */
//
//
//#endif /* !NOWINMESSAGES */
//
//#if(WINVER >= 0x0604)
///*
// * EM_ENABLEFEATURE options
// */
//typedef enum {
//	EDIT_CONTROL_FEATURE_ENTERPRISE_DATA_PROTECTION_PASTE_SUPPORT = 0,
//	EDIT_CONTROL_FEATURE_PASTE_NOTIFICATIONS = 1,
//} EDIT_CONTROL_FEATURE;
//#endif /* WINVER >= 0x0604 */