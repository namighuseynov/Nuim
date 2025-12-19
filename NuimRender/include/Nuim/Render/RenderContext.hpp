#pragma once
#include "Core/Base.hpp"
#include <memory>

namespace Nuim {

    class Window;

    class RenderContext {
    public:
        virtual ~RenderContext() = default;

        virtual void Init(Window& window) = 0;
        virtual void Shutdown() = 0;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

        virtual void Resize(U32 width, U32 height) = 0;

        static std::unique_ptr<RenderContext> Create();
    };
}
