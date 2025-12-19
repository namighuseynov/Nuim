#include "NuimPCH.h"
#include "Nuim/Render/Renderer.hpp"
#include "Nuim/DX11/DX11Renderer.hpp"

namespace Nuim {

    std::unique_ptr<Renderer> Renderer::Create(const RendererDesc& desc)
    {
        return std::make_unique<DX11Renderer>(desc);
    }

}
