#pragma once
#include "Core/Base.hpp"
#include <memory>

namespace Nuim {

    struct ClearColor {
        float r = 0.1f, g = 0.1f, b = 0.1f, a = 1.0f;
    };

    class SwapChain {
    public:
        virtual ~SwapChain() = default;

        virtual void Present(bool vsync) = 0;
        virtual void Resize(U32 width, U32 height) = 0;

        virtual void Clear(const ClearColor& color) = 0;

        static std::unique_ptr<SwapChain> Create(void* nativeWindowHandle);
    };

}
