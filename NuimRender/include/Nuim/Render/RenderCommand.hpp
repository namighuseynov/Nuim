#pragma once
#include "Core/Base.hpp"

namespace Nuim {
    class ClearColor;

    class RenderCommand {
    public:
        static void Clear(const ClearColor& color);
        static void Present(bool vsync);
        static void Resize(U32 w, U32 h);
    };
}
