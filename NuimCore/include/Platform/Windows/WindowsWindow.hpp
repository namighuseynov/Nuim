#pragma once

#include "include/Core/Window.hpp"

#if NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#include <windowsx.h> // GET_X_LPARAM / GET_Y_LPARAM
#endif

#include <string>

namespace Nuim {

    class WindowsWindow final : public Window {
    public:
        explicit WindowsWindow(const WindowProps& props);
        ~WindowsWindow() override;

        void PollEvents() override;

        void SetEventCallback(const EventCallbackFn& cb) override { m_data.EventCallback = cb; }
        void SetNativeMessageHook(const NativeMessageHook& hook) override { m_data.NativeHook = hook; }

        U32 GetWidth()  const override { return m_data.Width; }
        U32 GetHeight() const override { return m_data.Height; }

        void* GetNativeHandle() const override { return (void*)m_hwnd; }

        void SetVSync(bool enabled) override { m_data.VSync = enabled; }
        bool IsVSync() const override { return m_data.VSync; }

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
        static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        HWND m_hwnd = nullptr;

        struct WindowData {
            std::string Title;
            U32 Width = 0, Height = 0;
            bool VSync = true;

            EventCallbackFn EventCallback;
            NativeMessageHook NativeHook;
        } m_data;
    };

}
