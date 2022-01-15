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



	Label label(&window, 320, 100, 200, 200, "About Label");
	
	
	return app.Exec();
}