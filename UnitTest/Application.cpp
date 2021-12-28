#include "PCH.h"
#include "Application.h"

Application::Application()
{
	InitCommonControls();
}

int Application::GetCommonContorlVersion() const
{
	HMODULE ctrl_lib = LoadLibrary("COMCTL32.DLL");
	if (!ctrl_lib)
		return 0;



	return 0;
}

int Application::Exec()
{
	MSG msg{};
	while (GetMessage(&msg, NULL, NULL, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.lParam;
}
