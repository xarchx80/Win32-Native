#include <PCH.h>
#include <Wnd.h>
#include <Control.h>
#include <Application.h>
#include <StatusBar.h>
#include <TabControl.h>
#include <IButton.h>


int main(int args, char* argv[])
{
	Application app;

	Wnd window(nullptr, 700, 200, 700, 400,"main window");
	window.hbrBkgorund = CreateSolidBrush(RGB(35, 40, 44));
	window.Create();
	window.Show();

	//SuperClassControl button(&window, 50, 50, 200, 80, "Button1");
	//button.lpClass = "Button";
	//button.lpSuperClass = "Button_Ex";
	//button.style = WS_VISIBLE | BS_PUSHBUTTON;
	//button.Create();

	SubClassControl button1(&window, 250, 50, 200, 80, "Button1");
	button1.lpClass = "Button";
	button1.lpSuperClass = "Button_Ex";
	button1.style = WS_VISIBLE ;
	button1.Create();

	IButton b(nullptr,"long button");
	
	RECT rc;
	TabControl tab(&window);
	Wnd* parent = tab.m_parent;
	GetClientRect(parent->m_hwnd, &rc);
	rc.bottom -= 28;
	SetWindowPos(tab.m_hwnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL);

	auto a = SuperClassControl::m_registeredSuperClasses;

	
	std::vector<TabItem> tItems = {
		{nullptr, "Zero"},
		{ nullptr, "One"},
		{ &b, b.GetText()}
	};
	tab.SetItems(tItems);
	//Control status(&window, 100, -200, 100, 10, "status");
	//status.lpClass = STATUSCLASSNAME;
	//status.style = WS_VISIBLE | WS_BORDER |CCS_BOTTOM;
	//status.Create();
	StatusBar s(&window);

	
	//Log::log_message("hello");
	//throw std::runtime_error("vaild");
	
	return app.Exec();
}