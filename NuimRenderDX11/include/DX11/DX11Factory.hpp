#pragma once
#include <memory>

#include "Render/IRenderContext.hpp"
#include "Render/ISwapChain.hpp"
#include "Render/IRenderTarget.hpp"
#include "Render/RenderTypes.hpp"

namespace Nuim::DX11 {

    std::unique_ptr<Nuim::Render::IRenderContext> CreateRenderContextDX11();

    std::unique_ptr<Nuim::Render::ISwapChain> CreateSwapChainDX11(
        Nuim::Render::IRenderContext& ctx,
        const Nuim::Render::SwapChainDesc& desc
    );

    std::unique_ptr<Nuim::Render::IRenderTarget> CreateRenderTargetDX11(
        Nuim::Render::IRenderContext& ctx,
        const Nuim::Render::RenderTargetDesc& desc
    );

}
