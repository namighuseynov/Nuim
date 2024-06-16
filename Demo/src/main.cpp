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
#include "EntryPoint.h"

//LRESULT Wndproc(
//	HWND hWnd,
//	UINT uMsg,
//	WPARAM wParam,
//	LPARAM lParam
//) {
//	return NULL;
//}




#else
#define CONSOLE_APPLICATION
int main() {
	NuimDemo::NuimApplication* app = new NuimDemo::NuimApplication();
	app->Run();
	delete app;
	std::cout << "Hello, world" << std::endl;
}
#endif
