#pragma once
#include "GraphicsDevice.hpp"
#include "SwapChain.hpp"
#include "RenderContext.hpp"
#include "RenderStats.hpp"
#include "RenderTarget2D.hpp"
#include "DepthStencil.hpp"

namespace Nuim {
    class Renderer {
    public:
        bool Init(HWND hwnd, int width, int height, bool vsync);

        void EndFrame();

        void Resize(int width, int height);

        void SetCamera(
            const DirectX::XMMATRIX& view,
            const DirectX::XMMATRIX& proj
        );

        void DrawItem(const RenderItem& item);
        void BeginBackbufferPass(const float clearColor[4]);
        void BeginPass(RenderTarget2D& target, const float clearColor[4]);

        ID3D11Device* GetDevice() { return m_device.Device(); }
        ID3D11DeviceContext* GetContext() { return m_device.Context(); }

    public:
        const RenderStats& GetStats() const { return m_stats; }
        void ResetStats() { m_stats.Reset(); }

    private:
        GraphicsDevice m_device;
        SwapChain m_swapChain;
        RenderContext m_renderContext;

        RenderStats m_stats;
        bool m_vsync = true;

        DepthStencil m_backDepth;
        D3D11_VIEWPORT m_backVp{};
    };
}