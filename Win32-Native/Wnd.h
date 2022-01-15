#pragma once

struct WndCreateStruct
{
	LPCSTR lpText;
	LPCSTR lpClass;
	LPCSTR lpSuperClass;
	DWORD style;
	DWORD styleEx;
	int x, y, cx, cy;
	HBRUSH hbrBkgrnd;
	HBRUSH hFont;
};

class Wnd
{
public:
	Wnd(Wnd* parent);
	Wnd(Wnd* parent, int x, int y, int w, int h, const TCHAR* text);
	virtual ~Wnd();
	
	virtual bool Create();
	void Show();
	const char* GetText() const;
	virtual LRESULT OnNotify(Event &e);
	virtual LRESULT OnNotifyReflect(Event &e);
	virtual LRESULT OnPaint(Painter &p);
	virtual void	OnResize(int w, int h);
	virtual LRESULT OnDrawItem(DRAWITEMSTRUCT* dis);
	virtual LRESULT OnEraseBkgnd(Event &e);
	virtual LRESULT OnCtrlColor(Event& e);
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	virtual LRESULT LocalDefWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	static LRESULT WINAPI GlobalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
	


	virtual bool SetSuperClass(LPCSTR src, LPCSTR dest);
	virtual bool SetSubClass();

	bool HasStyle(DWORD s);
	void AddStyle(DWORD s);
	void RemoveStyle(DWORD s);
	void SetStyle(DWORD s);
	void SetStyleEx(DWORD s);

	void SetClassName(LPCSTR name);
	void SetSuperClassName(LPCSTR name);
	void SetWindowName(LPCSTR name);

	void Hide();
	void SetGeometry(const RECT& rc);
	void SetGeometry(const RECT& rc, HWND zorder, UINT flag);

	void SetFont(HFONT font);
	HFONT GetFont() const;
	void SetBkBrush(HBRUSH br);
	HBRUSH GetBkBrush() const;

	bool SetParent(HWND parent);
	Wnd* mParent;
	HWND mHwnd;
	std::string lpClass;
	std::string lpText;
	std::string lpSuperClass;
	DWORD style;
	DWORD styleEx;
	UINT id = 0;
	HFONT mFont;
	//static WNDPROC mPreWndProc;
	struct {
		int x, y, cx, cy;
	}geo;
	bool mIsControl = false;
	bool mIsSupperClass = false;
	bool mIsSubClass = false;
	bool mIsPaintEventEnable = false;
	bool bPaintEnable;
	HBRUSH mBkBrush = nullptr;
	
};

