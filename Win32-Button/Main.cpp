#include <PCH.h>
#include <Wnd.h>
#include <Control.h>
#include <Application.h>

#pragma comment(lib,"Win32-Native.lib")

#include <Label.h>

int main(int args, char* argv[])
{
	Application app;

	Wnd window(nullptr, 700, 200, 700, 400,"main window");
	window.Create();
	window.Show();

	Control button(&window, 50, 50, 300, 100, "Button1");
	button.lpClass = "Button";
	button.style = WS_VISIBLE | BS_PUSHBUTTON;
	button.Create();

	Control status(&window, 100, -200, 100, 10, "status");
	status.lpClass = STATUSCLASSNAME;
	status.style = WS_VISIBLE | WS_BORDER;
	status.Create();

	Label label(&window, 320, 100, 200, 200, "About Label");

	//Log::log_message("hello");
	
	
	return app.Exec();
}