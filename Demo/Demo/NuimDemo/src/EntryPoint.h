#pragma once
#include "Application.hpp"
#include "EventSystem.hpp"

using namespace EventSystem;

EventDispatcher eventDispatcher;

void OnApplicationStart(Event& e) {
	MessageBox(nullptr, L"Application has started", L"Message", 0);
}

LRESULT CALLBACK WndProcess(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {

	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR pCmdLine,
	int nCmdShow
) {
	eventDispatcher.RegisterListener("ApplicationStart", OnApplicationStart);
	ApplicationStartEvent applicationStartEvent;
	eventDispatcher.Dispatch(applicationStartEvent);
	NuimDemo::Application* app = new NuimDemo::Application(hInstance, hPrevInstance, pCmdLine, nCmdShow);
	app->Run();
	delete app;
}