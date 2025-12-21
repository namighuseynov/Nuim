#include "NuimPCH.h"
#include "include/Platform/Windows/WindowsWindow.hpp"
#include "include/Core/Base.hpp"

#include "include/Core/Events/ApplicationEvent.hpp"
#include "include/Core/Events/KeyEvent.hpp"
#include "include/Core/Events/MouseEvent.hpp"
#include "include/Core/Events/WindowEvent.hpp"

#include "include/Platform/Windows/WindowsKeyMap.hpp"

#include <stdexcept>
#include <memory>

#if NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#include <windowsx.h> // GET_X_LPARAM / GET_Y_LPARAM
#endif

namespace Nuim {

    static std::wstring ToWide(const std::string& s)
    {
        std::wstring wsTmp(s.begin(), s.end());
        std::wstring out = wsTmp;
        return out;
    }

    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
#if NUIM_PLATFORM_WINDOWS
        return std::make_unique<WindowsWindow>(props);
#else
        static_assert(false, "Window::Create not implemented for this platform");
        return nullptr;
#endif
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_data.Title = props.Title;
        m_data.Width = props.Width;
        m_data.Height = props.Height;
        m_data.VSync = props.VSync;

        HINSTANCE hInst = GetModuleHandleW(nullptr);

        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WindowsWindow::WndProc;
        wc.hInstance = hInst;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.lpszClassName = L"NuimWindowClass";

        static bool s_registered = false;
        if (!s_registered)
        {
            if (!RegisterClassExW(&wc))
                throw std::runtime_error("RegisterClassExW failed");
            s_registered = true;
        }

        RECT rc{ 0, 0, (LONG)m_data.Width, (LONG)m_data.Height };
        AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

        std::wstring wtitle = ToWide(m_data.Title);

        m_hwnd = CreateWindowExW(
            0,
            wc.lpszClassName,
            wtitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            rc.right - rc.left,
            rc.bottom - rc.top,
            nullptr, nullptr,
            hInst,
            this
        );

        if (!m_hwnd)
            throw std::runtime_error("CreateWindowExW failed");

        ShowWindow(m_hwnd, SW_SHOW);
        UpdateWindow(m_hwnd);
    }

    void WindowsWindow::Shutdown()
    {
        if (m_hwnd)
        {
            DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }
    }

    void WindowsWindow::PollEvents()
    {
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0u, 0u, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    LRESULT CALLBACK WindowsWindow::WndProc(HWND hwnd, U32 msg, WPARAM wparam, LPARAM lparam)
    {
        if (msg == WM_NCCREATE)
        {
            auto* cs = reinterpret_cast<CREATESTRUCTW*>(lparam);
            auto* window = reinterpret_cast<WindowsWindow*>(cs->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            return DefWindowProcW(hwnd, msg, wparam, lparam);
        }

        auto* window = reinterpret_cast<WindowsWindow*>(
            GetWindowLongPtrW(hwnd, GWLP_USERDATA)
            );

        if (window)
            return window->HandleMessage(hwnd, msg, wparam, lparam);

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

    LRESULT WindowsWindow::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (m_data.NativeHook)
        {
            if (m_data.NativeHook(hwnd, msg, (U64)wparam, (I64)lparam))
                return 0;
        }

        switch (msg)
        {
        case WM_SIZE:
        {
            if (wparam == SIZE_MINIMIZED) return 0;

            m_data.Width = (U32)LOWORD(lparam);
            m_data.Height = (U32)HIWORD(lparam);

            if (m_data.EventCallback)
            {
                WindowSizeEvent e(m_data.Width, m_data.Height);
                m_data.EventCallback(e);
            }
            return 0;
        }

        case WM_SETFOCUS:
        {
            if (m_data.EventCallback)
            {
                WindowGotFocusEvent e; 
                m_data.EventCallback(e);
            }
            return 0;
        }

        case WM_KILLFOCUS:
        {
            if (m_data.EventCallback)
            {
                WindowLostFocusEvent e;
                m_data.EventCallback(e);
            }
            return 0;
        }

        case WM_MOUSEMOVE:
        {
            if (m_data.EventCallback)
            {
                U32 x = GET_X_LPARAM(lparam);
                U32 y = GET_Y_LPARAM(lparam);
                MouseMoveEvent e(x, y);
                m_data.EventCallback(e);
            }
            return 0;
        }

        case WM_LBUTTONDOWN:
            if (m_data.EventCallback) { 
                U32 x = (U32)GET_X_LPARAM(lparam);
                U32 y = (U32)GET_Y_LPARAM(lparam);
                MousePressEvent e((U32)MouseButton::Left, x, y);
                m_data.EventCallback(e);
            }
            return 0;
        case WM_LBUTTONUP:
            if (m_data.EventCallback) { 
                MouseReleaseEvent e((U32)MouseButton::Left);
                m_data.EventCallback(e);
            }
            return 0;

        case WM_RBUTTONDOWN:
            if (m_data.EventCallback) { 
                U32 x = (U32)GET_X_LPARAM(lparam);
                U32 y = (U32)GET_Y_LPARAM(lparam);
                MousePressEvent e((U32)MouseButton::Right, x, y); // <-- FIX
                m_data.EventCallback(e);
            }
            return 0;
        case WM_RBUTTONUP:
            if (m_data.EventCallback) { MouseReleaseEvent e((U32)MouseButton::Right); m_data.EventCallback(e); }
            return 0;

        case WM_MBUTTONDOWN:
            if (m_data.EventCallback) { 
                U32 x = (U32)GET_X_LPARAM(lparam);
                U32 y = (U32)GET_Y_LPARAM(lparam);
                MousePressEvent e((U32)MouseButton::Middle, x, y);
                m_data.EventCallback(e);
            }
            return 0;
        case WM_MBUTTONUP:
            if (m_data.EventCallback) { MouseReleaseEvent e((U32)MouseButton::Middle); m_data.EventCallback(e); }
            return 0;

        case WM_KEYDOWN:
        {
            if (m_data.EventCallback)
            {
                KeyCode key = WindowsKeyMap::ToKeyCode((U32)wparam);
                if (key != KeyCode::Unknown)
                {
                    KeyPressEvent e(key);
                    m_data.EventCallback(e);
                }
            }
            return 0;
        }

        case WM_KEYUP:
        {
            if (m_data.EventCallback)
            {
                KeyCode key = WindowsKeyMap::ToKeyCode((U32)wparam);
                if (key != KeyCode::Unknown)
                {
                    KeyReleaseEvent e(key);
                    m_data.EventCallback(e);
                }
            }
            return 0;
        }

        case WM_CLOSE:
        {
            if (m_data.EventCallback)
            {
                WindowCloseEvent e;
                m_data.EventCallback(e); 
            }
            PostQuitMessage(0);
            return 0;
        }
        case WM_SYSCOMMAND: {
            switch (wparam & 0xFFF0) {
            case SC_MINIMIZE: {
                WindowMinimizeEvent windowMinimizeEvent;
                m_data.EventCallback(windowMinimizeEvent);
                break;
            }
            case SC_RESTORE: {
                WindowRestoreEvent windowRestoreEvent;
                m_data.EventCallback(windowRestoreEvent);
                break;
            }
            case SC_MAXIMIZE: {
                WindowMaxmimizeEvent windowMaximizeEvent;
                m_data.EventCallback(windowMaximizeEvent);
                break;
            }
            }
            break;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }

        return DefWindowProcW(hwnd, msg, wparam, lparam);
    }

} 
