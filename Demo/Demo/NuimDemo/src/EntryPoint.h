#pragma once
#include "Application.hpp"
using namespace NuimDemo;

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int nCmdShow
) {
	Application* app = new Application(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->Run();
	delete app;
}