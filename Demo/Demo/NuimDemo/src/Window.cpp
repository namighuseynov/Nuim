#include "NuimDemoPCH.h"
#include "Window.hpp"

LRESULT CALLBACK WndProcess(
	HWND hwnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
) {

	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

namespace NuimDemo {
	Window* Window::Create(HINSTANCE hInstance)
	{
		Window* w = new Window();
		w->Width = 600;
		w->Height = 600;

		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hInstance = hInstance;
		wcex.lpszClassName = L"NuimDemo";
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.lpszMenuName = L"NuimDemo";
		wcex.style = CS_HREDRAW;
		wcex.lpfnWndProc = WndProcess;
	 	HRESULT hr = RegisterClassEx(&wcex);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"Registererror", L"Error", 0);
			return nullptr;
		}
		w->hwnd = CreateWindowEx(0, L"NuimDemo", L"NuimDemo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, w->Width, w->Height, nullptr, nullptr, hInstance, nullptr);
		if (!w->hwnd) {
			MessageBox(nullptr, L"create window error", L"Error", 0);
			return nullptr;
		}
		ShowWindow(w->hwnd, 1);
		return w;
	}
	void Window::OnUpdate()
	{
		MSG msg;
		while (GetMessage(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}