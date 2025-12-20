#pragma once
#include "Core/Base.hpp"

namespace Nuim::Render {

    enum class Backend : U8
    {
        None = 0,
        DX11 = 1
    };

    struct SwapChainDesc {
        void* WindowHandle = nullptr;
        U32 Width = 1280;
        U32 Height = 720;
        bool VSync = true;
    };

    struct RenderTargetDesc
    {
        U32 Width = 1280;
        U32 Height = 720;
        bool HasDepth = true;
    };
}