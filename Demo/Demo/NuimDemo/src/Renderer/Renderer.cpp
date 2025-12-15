// Renderer.cpp
#include "NuimDemoPCH.h"
#include "Renderer.hpp"

namespace Nuim {

    bool Renderer::Init(HWND hwnd, int width, int height, bool vsync)
    {
        if (!m_device.Init()) return false;
        if (!m_backbuffer.Init(hwnd, m_device.Device(), width, height, vsync)) return false;
        if (!m_renderContext.Init(m_device.Device())) return false;

        m_inited = true;
        return true;
    }

    void Renderer::Resize(int width, int height)
    {
        if (!m_inited) return;
        m_backbuffer.Resize(m_device.Device(), width, height);
    }

    void Renderer::BeginFrameStats()
    {
        m_stats.Reset();
    }

    void Renderer::SetCamera(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
    {
        m_renderContext.SetCamera(view, proj);
    }

    void Renderer::DrawItem(const RenderItem& item)
    {
        m_renderContext.DrawItem(m_device.Context(), item);
        m_stats.drawCalls += 1;
        m_stats.triangles += item.mesh ? (item.mesh->GetIndexCount() / 3) : 0;
    }

    void Renderer::BeginPass(RenderTarget2D& target, const float clearColor[4])
    {
        auto* ctx = m_device.Context();
        target.Bind(ctx);
        target.Clear(ctx, clearColor, true, true);
    }

    void Renderer::BeginBackbufferPass(const float clearColor[4])
    {
        auto* ctx = m_device.Context();
        m_backbuffer.Bind(ctx);
        m_backbuffer.Clear(ctx, clearColor, true, true);
    }

    void Renderer::EndFrame()
    {
        m_backbuffer.Present();
    }

} // namespace Nuim
