#pragma once
#include <memory>
#include "Core/Base.hpp"

namespace Nuim {

    class Window;

    struct RendererDesc {
        Window* WindowHandle = nullptr;
        bool EnableDebug = false;
    };

    class Renderer {
    public:
        virtual ~Renderer() = default;

        virtual void BeginFrame() = 0;
        virtual void Clear(float r, float g, float b, float a) = 0;
        virtual void EndFrame() = 0;

        virtual void Resize(U32 w, U32 h) = 0;

        static std::unique_ptr<Renderer> Create(const RendererDesc& desc);
    };
}
