#include "NuimDemoPCH.h"
#include "Window.hpp"

namespace NuimDemo {
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
					if (pWindow->eventCallbackFn) {
						int width = GET_X_LPARAM(lParam);
						int height = GET_Y_LPARAM(lParam);
						EventSystem::WindowSizeEvent windowSizeEvent(width, height);
						pWindow->eventCallbackFn(windowSizeEvent);
					}
					break;
				}
				case WM_MOUSELEAVE: {
					MessageBox(nullptr, L"MouseLeaved", L"Message", 0);
					break;
				}
				case WM_MBUTTONDOWN: {
					EventSystem::MouseMiddleButtonDown mouseMiddleButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					pWindow->eventCallbackFn(mouseMiddleButtonDown);
					break;
				}
				case WM_MBUTTONUP: {
					EventSystem::MouseMiddleButtonRelease mouseMiddleButtonRelease(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
					pWindow->eventCallbackFn(mouseMiddleButtonRelease);
					break;
				}
				case WM_LBUTTONDOWN: {
					EventSystem::MousePressEvent mousePressEvent(EventSystem::MouseButton::NM_LEFT, LOWORD(lParam), HIWORD(lParam));
					pWindow->eventCallbackFn(mousePressEvent);
					break;
				}
				case WM_LBUTTONUP: {
					break;
				}
				case WM_SETFOCUS: {
					EventSystem::WindowGotFocusEvent windowGotFocusEvent;
					if (pWindow->eventCallbackFn) {
						pWindow->eventCallbackFn(windowGotFocusEvent);
					}
					break;
				}
			
				case WM_KILLFOCUS: {
					EventSystem::WindowLostFocusEvent windowLostFocusEvent;
					if (pWindow->eventCallbackFn) {
						pWindow->eventCallbackFn(windowLostFocusEvent);
					}
					break;
				}
				case WM_MOUSEMOVE:{
					int x = (int)(short)GET_X_LPARAM(lParam);
					int y = (int)(short)GET_Y_LPARAM(lParam);
					EventSystem::MouseMoveEvent mouseMoveEvent(x, y);
					pWindow->eventCallbackFn(mouseMoveEvent);
					break;
				}
			
				case WM_KEYDOWN: {
					EventSystem::KeyPressEvent keyPressEvent(static_cast<int>(wParam));
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
				case WM_CLOSE: {
					std::cout << "Closing.." << std::endl;
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