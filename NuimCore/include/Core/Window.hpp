#pragma once
#include "Base.hpp"

#include <string>
#include <functional>
#include <memory>

namespace Nuim {

    class Event; 

    struct WindowProps {
        std::string Title = "Nuim Engine";
        U32 Width = 1280;
        U32 Height = 720;
        bool VSync = true;
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        using NativeMessageHook = std::function<bool(void* hwnd, U32 msg, U32 wparam, I64 lparam)>;

        virtual ~Window() = default;

        virtual void PollEvents() = 0;
        virtual void SetEventCallback(const EventCallbackFn& cb) = 0;

        virtual void SetNativeMessageHook(const NativeMessageHook& hook) = 0;

        virtual U32 GetWidth()  const = 0;
        virtual U32 GetHeight() const = 0;

        virtual void* GetNativeHandle() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
    };

}
