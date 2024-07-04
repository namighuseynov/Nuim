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
	auto app = std::unique_ptr<NuimDemo::Application>(new NuimDemo::Application(hInstance, hPrevInstance, pCmdLine, nCmdShow));
	app->Run();
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


