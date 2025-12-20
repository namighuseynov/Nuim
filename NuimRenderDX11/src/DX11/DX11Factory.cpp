#include "NuimPCH.h"
#include "DX11/DX11Factory.hpp"

#include "DX11/DX11RenderContext.hpp"
#include "DX11/DX11SwapChain.hpp"
#include "DX11/DX11RenderTarget.hpp"

#include <stdexcept>

namespace Nuim::DX11 {

    std::unique_ptr<Nuim::Render::IRenderContext> CreateRenderContextDX11()
    {
        return std::make_unique<DX11RenderContext>();
    }

    std::unique_ptr<Nuim::Render::ISwapChain> CreateSwapChainDX11(
        Nuim::Render::IRenderContext& ctx,
        const Nuim::Render::SwapChainDesc& desc)
    {
        auto* dx11 = dynamic_cast<DX11RenderContext*>(&ctx);
        if (!dx11)
            throw std::runtime_error("CreateSwapChainDX11: ctx is not DX11RenderContext");

        return std::make_unique<DX11SwapChain>(*dx11, desc);
    }

    std::unique_ptr<Nuim::Render::IRenderTarget> CreateRenderTargetDX11(
        Nuim::Render::IRenderContext& ctx,
        const Nuim::Render::RenderTargetDesc& desc)
    {
        auto* dx11 = dynamic_cast<DX11RenderContext*>(&ctx);
        if (!dx11)
            throw std::runtime_error("CreateRenderTargetDX11: ctx is not DX11RenderContext");

        return std::make_unique<DX11RenderTarget>(*dx11, desc);
    }

}
