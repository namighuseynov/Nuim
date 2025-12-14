#pragma once

#ifdef WINDOWED
#define WINDOWED_APPLICATION

#include <Windows.h>
#include "Application.hpp"


int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR  pCmdLine,
	int nCmdShow
) {
	auto app = new Nuim::Application(hInstance);
	app->Run();
	delete app;
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


