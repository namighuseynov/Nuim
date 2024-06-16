#include <iostream>
#include "Application.h"

namespace NuimDemo {
	class NuimApplication :
		public Application {

	};
}

#ifdef WINDOWED
#define WINDOWED_APPLICATION
#include <Windows.h>

LRESULT Wndproc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	return NULL;
}

int WINAPI WinMain( 
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		lpCmdLine,
	int		nShowCmd
) {
	HWND hwnd = nullptr;
	WNDCLASSEX wc = {};


	return MessageBox(NULL,L"Message", L"caption", 0);
}


#else
#define CONSOLE_APPLICATION
int main() {
	NuimDemo::NuimApplication* app = new NuimDemo::NuimApplication();
	app->Run();
	delete app;
	std::cout << "Hello, world" << std::endl;
}
#endif
