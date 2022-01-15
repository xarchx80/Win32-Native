#pragma once

struct HookEvent
{
	HWND hwnd;
	int code;
	WPARAM wp;
	LPARAM lp;
};

class CBTHook
{
public:
	CBTHook(UINT thraedId);
	virtual~CBTHook();

	virtual void OnCreate(HookEvent& e);
	virtual void OnActive(HookEvent& e);
	static void HookMessageBox(LPCSTR msg, UINT flag);
	static HHOOK gHook;
	UINT mThreadId;
	HOOKPROC mHookWndProc;
	static LRESULT CALLBACK HookCBTWndProc(INT code, WPARAM wp, LPARAM lp);
	static LRESULT CALLBACK HookCallWndProc(INT code, WPARAM wp, LPARAM lp);
	static BOOL CALLBACK HookWndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
};

