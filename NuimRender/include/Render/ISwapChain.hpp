#pragma once
#include "Core/Base.hpp"
#include <memory>

namespace Nuim::Render {

    struct ClearColor {
        float r = 0.1f, g = 0.1f, b = 0.1f, a = 1.0f;
    };

    class ISwapChain {
    public:
        virtual ~ISwapChain() = default;

        virtual void Resize(U32 width, U32 height) = 0;
        virtual void Present() = 0;

        virtual U32 GetWidth() const = 0;
        virtual U32 GetHeight() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeBackbufferRTV() const = 0;
    };

}
