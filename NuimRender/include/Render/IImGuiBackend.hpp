#pragma once
#include <cstdint>

namespace Nuim::Render {

    class IImGuiBackend {
    public:
        virtual ~IImGuiBackend() = default;

        virtual void Init(void* nativeWindowHandle, void* renderContextNative) = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual bool HandleNativeMessage(void* hwnd, uint32_t msg, uint64_t wparam, int64_t lparam) = 0;

        virtual bool IsInitialized() const = 0;
    };
}
