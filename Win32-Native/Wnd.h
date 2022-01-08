#pragma once


class Wnd
{
public:
	Wnd(Wnd* parent);
	Wnd(Wnd* parent, int x, int y, int w, int h, const TCHAR* text);
	virtual ~Wnd();
	
	virtual bool Create();
	void Show();
	const char* GetText() const;
	virtual LRESULT NotifyEvent(Event &e);
	virtual LRESULT NotifyReflectEvent(Event &e);
	virtual LRESULT PaintEvent(Painter &p);
	virtual void OnResize(SizeEvent &e);
	virtual LRESULT DrawItemEvent(DRAWITEMSTRUCT* dis);
	virtual LRESULT EraseBkgndEvent(Event &e);
	virtual LRESULT LocalWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
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

	bool SetParent(HWND parent);
	Wnd* m_parent;
	HWND m_hwnd;
	std::string lpClass;
	std::string lpText;
	std::string lpSuperClass;
	DWORD style;
	DWORD styleEx;
	UINT id = 0;
	//static WNDPROC mPreWndProc;
	struct {
		int x, y, cx, cy;
	}geo;
	bool is_control = false;
	bool is_control_paint_event_enable = false;
	bool m_IsSupperClass = false;
	bool m_IsSubClass = false;
	bool bPaintEnable;
	HBRUSH hbrBkgorund = nullptr;
};

