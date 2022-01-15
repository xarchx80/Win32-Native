#include <PCH.h>
#include <Wnd.h>
#include <Control.h>
#include <Application.h>
#include <IStatusBar.h>
#include <ITabControl.h>
#include <IButton.h>
#include <ILineEdit.h>
#include <CBTHook.h>

int main(int args, char* argv[])
{
	Application app;

	Wnd window(nullptr, 700, 200, 700, 400,"main window");
	window.mBkBrush = CreateSolidBrush(RGB(35, 40, 44));
	window.Create();
	window.Show();

	

	SubClassControl button1(nullptr, 250, 50, 200, 80, "Button1");
	button1.lpClass = "Button";
	button1.lpSuperClass = "Button_Ex";
	button1.style = WS_VISIBLE ;
	button1.Create();

	ILineEdit edit(nullptr, 0, 0, 100, 200, "edit");

	IButton b(nullptr,"long button");
	
	RECT rc;
	ITabControl tab(&window);
	Wnd* parent = tab.mParent;
	GetClientRect(parent->mHwnd, &rc);
	rc.bottom -= 28;
	SetWindowPos(tab.mHwnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, NULL);
	
	//std::vector<TabItem> tItems = {
	//	{ &button1, button1.GetText()},
	//	{ &edit, edit.GetText()}
	//};
	TabItem item{ nullptr, "null" };
	TabItem item1{ &edit, edit.GetText() };
	TabItem btnTabItem{ &button1, button1.GetText() };
	
	//tab.SetItems(tItems);
	tab.InsertItem(item);
	tab.InsertItem(item1);
	//tab.InsertItem(btnTabItem);

	IStatusBar s(&window);

	for (auto a : SuperClassControl::mRegistedSuperClasses) {
		LOG("%s __" ,a.c_str());
	}
	
	return app.Exec();
}