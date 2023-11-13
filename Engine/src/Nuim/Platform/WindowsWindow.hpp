#pragma once
#include "../Core/Core.h"
#include "Window.hpp"

namespace Nuim {
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


    class WindowsWindow :
        public Window {
    public:
        WindowsWindow(USTRING title, U32 width, U32 height, HINSTANCE hInstance, I32 nCmdShow);
        ~WindowsWindow();
    public:
        USTRING Title;
    private:
        USTRING title;
        HWND instance;
    };
}