#include "NuimDemoPCH.h"
#include "Renderer.hpp"


namespace Nuim {
    bool Renderer::Init(HWND hwnd, int width, int height, bool vsync)
    {
        m_vsync = vsync;

        if (!m_device.Init()) return false;
        if (!m_swapChain.Init(hwnd, m_device.Device(), width, height)) return false;

        if (!m_backDepth.Create(m_device.Device(), width, height)) return false;

        m_backVp.TopLeftX = 0;
        m_backVp.TopLeftY = 0;
        m_backVp.Width = (float)width;
        m_backVp.Height = (float)height;
        m_backVp.MinDepth = 0;
        m_backVp.MaxDepth = 1;

        if (!m_renderContext.Init(m_device.Device())) return false;
        return true;
    }

    void Renderer::EndFrame()
    {
        m_swapChain.Present(m_vsync);
    }

    void Renderer::Resize(int width, int height)
    {
        m_swapChain.Resize(m_device.Device(), width, height);
        m_backDepth.Create(m_device.Device(), width, height);

        m_backVp.Width = (float)width;
        m_backVp.Height = (float)height;
    }

    void Renderer::SetCamera(
        const DirectX::XMMATRIX& view,
        const DirectX::XMMATRIX& proj
    )
    {
        m_renderContext.SetCamera(view, proj);
    }

    void Renderer::DrawItem(const RenderItem& item)
    {
        m_renderContext.DrawItem(
            m_device.Context(),
            item
        );

        m_stats.drawCalls += 1;

        m_stats.triangles += item.mesh ? (item.mesh->GetIndexCount() / 3) : 0;
    }

    void Renderer::BeginBackbufferPass(const float clearColor[4])
    {
        m_stats.Reset();
        auto* ctx = m_device.Context();

        ID3D11RenderTargetView* rtv = m_swapChain.RTV();
        ctx->OMSetRenderTargets(1, &rtv, m_backDepth.DSV());
        ctx->OMSetDepthStencilState(m_backDepth.DSS(), 1);
        ctx->RSSetViewports(1, &m_backVp);

        ctx->ClearRenderTargetView(rtv, clearColor);
        ctx->ClearDepthStencilView(m_backDepth.DSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

    void Renderer::BeginPass(RenderTarget2D& target, const float clearColor[4])
    {
        m_stats.Reset();
        auto* ctx = m_device.Context();
        target.Bind(ctx);
        target.Clear(ctx, clearColor, true, true);
    }
}