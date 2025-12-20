#pragma once
#include "Core/Base.hpp"
#include "RenderTypes.hpp"

namespace Nuim::Render {

    class IRenderTarget
    {
    public:
        virtual ~IRenderTarget() = default;

        virtual void Resize(U32 width, U32 height) = 0;
        virtual U32 GetWidth() const = 0;
        virtual U32 GetHeight() const = 0;

        virtual void Bind() = 0;
        virtual void Clear(float r, float g, float b, float a) = 0;

        virtual void* GetImGuiTextureID() const = 0;
    };

}
