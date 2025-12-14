#include "NuimDemoPCH.h"
#include "Renderer.hpp"

namespace Nuim {
    bool Renderer::Init(HWND hwnd, int width, int height)
    {
        if (!m_device.Init())
            return false;

        if (!m_swapChain.Init(hwnd, m_device.Device(), width, height))
            return false;

        if (!m_frameBuffers.Init(m_device.Device(), width, height))
            return false;

        if (!m_renderContext.Init(m_device.Device()))
            return false;

        return true;
    }

    void Renderer::BeginFrame(const float clearColor[4])
    {
        m_stats.Reset();

        auto* ctx = m_device.Context();

        m_frameBuffers.Bind(ctx, m_swapChain.RTV());
        m_frameBuffers.Clear(ctx, m_swapChain.RTV(), clearColor);
    }

    void Renderer::EndFrame()
    {
        m_swapChain.Present(true);
    }

    void Renderer::Resize(int width, int height)
    {
        m_swapChain.Resize(m_device.Device(), width, height);
        m_frameBuffers.Resize(m_device.Device(), width, height);
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
}