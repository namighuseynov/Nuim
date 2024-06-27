#include "pch.h"
#include <Windows.h>
#include "EventSystem.hpp"

using namespace EventSystem;
EventDispatcher eventDispatcher;

LRESULT CALLBACK WinProc(
	HWND hwnd,
	UINT msg,
	WPARAM wParam, 
	LPARAM lParam
) {
	
	switch (msg) {
	case WM_KEYDOWN: {
		int keyCode = static_cast<int>(wParam);
		KeyPressEvent keyPressEvent( keyCode );
		eventDispatcher.Dispatch(keyPressEvent);
		return 0;
	}
	case WM_KEYUP: {
		int keyCode = static_cast<int>(wParam);
		KeyReleaseEvent keyReleasedEvent(keyCode);
		eventDispatcher.Dispatch(keyReleasedEvent);
		return 0;
	}
	case WM_LBUTTONDOWN: {
		int mouseButton = MouseButton::NM_LEFT;
		MousePressEvent mousePressEvent(mouseButton);
		eventDispatcher.Dispatch(mousePressEvent);
		return 0;
	}
	case WM_CLOSE: {
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZE:

		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

void OnKeyPress(Event& e) {
	KeyPressEvent* keyPressEvent = dynamic_cast<KeyPressEvent*>(&e);
	if (keyPressEvent->GetKeyCode() == KeyCode::NM_KEY_P) {
		int keyCode = keyPressEvent->GetKeyCode();
		std::wstring msg = L"Presed key" + std::to_wstring(keyCode);
		MessageBox(nullptr, msg.c_str(), L"Message", 0);
	}
}

void OnKeyUp(Event& e) {
	KeyReleaseEvent* keyReleaseEvent = dynamic_cast<KeyReleaseEvent*>(&e);
	int keyCode = keyReleaseEvent->GetKeyCode();
	if (keyCode == KeyCode::NM_KEY_U) {
		std::wstring msg = L"Released key" + std::to_wstring(keyCode);
		MessageBox(nullptr, msg.c_str(), L"Message", 0);
	}
}

void OnMouseButtonPressed(Event& e) {
	MousePressEvent* mousePressEvent = dynamic_cast<MousePressEvent*>(&e);
	int pressedMouseButton = mousePressEvent->GetMouseButton();
	if (pressedMouseButton == MouseButton::NM_LEFT) {
		MessageBox(nullptr, L"Left mouse button pressed!", L"Message", 0);
	}
}

void OnMouseMove(Event& e) {
	
	
}

void OnApplicationStart(Event& e) {
	MessageBox(nullptr, L"Application has started", L"Message", 0);
}

void OnApplicationClose(Event& e) {
	MessageBox(nullptr, L"Application successfuly was closed", L"Message", 0);
}

HINSTANCE hInstance;

int WINAPI WinMain(
	HINSTANCE hINSTANCE,
	HINSTANCE hPrevHinstance,
	PSTR sCommandLine,
	int nCmdShow
) {
	eventDispatcher.RegisterListener("KeyPress", OnKeyPress);
	eventDispatcher.RegisterListener("KeyRelease", OnKeyUp);
	eventDispatcher.RegisterListener("MousePress", OnMouseButtonPressed);
	eventDispatcher.RegisterListener("ApplicationStart", OnApplicationStart);
	ApplicationStartEvent startEvent;
	eventDispatcher.Dispatch(startEvent);

	WNDCLASSEX wcex = {};
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = hINSTANCE;
	wcex.lpszClassName = L"EventSystem";
	wcex.lpszMenuName = L"EventSystem";
	wcex.lpfnWndProc = WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClassEx(&wcex)) {
		MessageBox(nullptr, L"Errors occured while registration WndClassEx", L"Error", 0);
		return 1;
	}
	hInstance = hINSTANCE;
	HWND hWnd = CreateWindowEx(
		WS_EX_OVERLAPPEDWINDOW,
		L"EventSystem",
		L"EventSystem",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		600,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hWnd) {
		MessageBox(nullptr, L"Errors occured while creating Window", L"Error", 0);
		return 1;
	}
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	

	MSG msg;
	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ApplicationCloseEvent applicationCloseEvent;
	eventDispatcher.Dispatch(applicationCloseEvent);
	return 0;
}