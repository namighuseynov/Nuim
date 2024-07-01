#include "NuimDemoPCH.h"
#include "Window.hpp"

namespace NuimDemo {
	int count = 0;
	Window::Window(HINSTANCE hInstance) : Height(600), Width(600)
	{
		WNDCLASSEX wcex = {};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.hInstance = hInstance;
		wcex.lpszClassName = L"NuimDemo";
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.lpszMenuName = L"NuimDemo";
		wcex.style = CS_HREDRAW;
		wcex.lpfnWndProc = Window::WndProcess;
		HRESULT hr = RegisterClassEx(&wcex);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"Registererror", L"Error", 0);
		}
		this->hwnd = CreateWindowEx(0, L"NuimDemo", L"NuimDemo", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, nullptr, nullptr, hInstance, this);
		if (!this->hwnd) {
			MessageBox(nullptr, L"create window error", L"Error", 0);
		}
		ShowWindow(this->hwnd, 1);
	}
	LRESULT Window::WndProcess(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CREATE) {
			LPCREATESTRUCT pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
			Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		else {
			Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (pWindow) {
			switch (msg) {
			case WM_SIZE: {
				if (count == 1) {
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);
					EventSystem::WindowSizeEvent windowSizeEvent(width, height);
					pWindow->eventCallbackFn(windowSizeEvent);
				}
				else {
					count += 1;
				}
				break;
			}
			case WM_MOUSEMOVE:{
				int x = (int)(short)LOWORD(lParam);
				int y = (int)(short)HIWORD(lParam);
				EventSystem::MouseMoveEvent mouseMoveEvent(x, y);
				pWindow->eventCallbackFn(mouseMoveEvent);
				break;
			}
			case WM_LBUTTONDOWN: {
				EventSystem::MousePressEvent mousePressEvent(EventSystem::MouseButton::NM_LEFT, LOWORD(lParam), HIWORD(lParam));
				pWindow->eventCallbackFn(mousePressEvent);
				break;
			}
			case WM_KEYDOWN: {
				EventSystem::KeyPressEvent keyPressEvent(static_cast<int>(lParam));
				pWindow->eventCallbackFn(keyPressEvent);
				break;
			}
			case WM_DESTROY: {
				PostQuitMessage(0);
				break;
			}
			case WM_SYSCOMMAND: {
				switch (wParam & 0xFFF0) {
				case SC_MINIMIZE: {
					EventSystem::WindowMinimizeEvent windowMinimizeEvent;
					pWindow->eventCallbackFn(windowMinimizeEvent);
					break;
				}
				case SC_RESTORE: {
					EventSystem::WindowRestoreEvent windowRestoreEvent;
					pWindow->eventCallbackFn(windowRestoreEvent);
					break;
				}
				case SC_MAXIMIZE: {
					EventSystem::WindowMaxmimizeEvent windowMaximizeEvent;
					pWindow->eventCallbackFn(windowMaximizeEvent);
					break;
				}
				}
				break;
			}
			
			default:
				return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
			
		}
	}
	void Window::SetEventCallback(EventCallback callbackFn)
	{
		this->eventCallbackFn = callbackFn;
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