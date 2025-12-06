#pragma once
#include "EventSystem.hpp"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace NuimDemo {
    class Window {
        using EventCallback = std::function<void(EventSystem::Event& e)>;
    public:
        Window(const int width, const int height) : width(width), height(height) {
            WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"NuimDemo", nullptr };
            ::RegisterClassExW(&wc);
            hwnd = ::CreateWindowW(wc.lpszClassName, L"NuimDemo", WS_OVERLAPPEDWINDOW, 100, 100, width, height, nullptr, nullptr, wc.hInstance, this);

            // Show the window
            ::ShowWindow(hwnd, SW_SHOWDEFAULT);
            ::UpdateWindow(hwnd);
        }
    public:
        void SetEventCallback(EventCallback callbackFn) {
            this->eventCallbackFn = callbackFn;
        }
		int GetWidth() const { return this->width; }
		int GetHeight() const { return this->height; }
        const HWND& GetHWND() { return this->hwnd; }
        static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
                return true;
            Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
            switch (msg) {
            case WM_CREATE: {
                LPCREATESTRUCT pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
                Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
                SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
                break;
            }
            case WM_SIZE: {
                if (pWindow->eventCallbackFn) {
                    int width = GET_X_LPARAM(lParam);
                    int height = GET_Y_LPARAM(lParam);
                    EventSystem::WindowSizeEvent evt(width, height);
                    pWindow->eventCallbackFn(evt);
                }
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
            case WM_MOUSEMOVE: {
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
            default: {
                return DefWindowProc(hWnd, msg, wParam, lParam);
            }

            }
            return ::DefWindowProcW(hWnd, msg, wParam, lParam);
        }
    private:
        HWND hwnd;
        EventCallback eventCallbackFn;
		int width = 1280;
		int height = 800;
    };
}