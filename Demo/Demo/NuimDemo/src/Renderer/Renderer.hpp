#pragma once
#include "GraphicsDevice.hpp"
#include "RenderContext.hpp"
#include "RenderStats.hpp"
#include "RenderTarget2D.hpp"
#include "SwapChainTarget.hpp"

namespace Nuim {
    class Renderer {
    public:
        bool Init(HWND hwnd, int width, int height, bool vsync);

        void Resize(int width, int height);

        void SetCamera(const DirectX::XMMATRIX& view,
            const DirectX::XMMATRIX& proj);

        void DrawItem(const RenderItem& item);

        void BeginFrameStats();

        void BeginPass(RenderTarget2D& target, const float clearColor[4]);

        void BeginBackbufferPass(const float clearColor[4]);

        void EndFrame();

        ID3D11Device* GetDevice() { return m_device.Device(); }
        ID3D11DeviceContext* GetContext() { return m_device.Context(); }

        const RenderStats& GetStats() const { return m_stats; }

        void Renderer::Shutdown()
        {
            if (auto* ctx = GetContext())
            {
                ctx->ClearState();
                ctx->Flush();
            }
        }

        void ResetStats() { m_stats.Reset(); }

    private:
        GraphicsDevice m_device;
        RenderContext m_renderContext;
        RenderStats m_stats;

        bool m_inited = true;
        SwapChainTarget  m_backbuffer;
    };
}