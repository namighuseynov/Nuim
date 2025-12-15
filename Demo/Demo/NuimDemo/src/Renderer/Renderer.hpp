#pragma once
#include "GraphicsDevice.hpp"
#include "SwapChain.hpp"
#include "FrameBuffers.hpp"
#include "RenderContext.hpp"
#include "RenderStats.hpp"

namespace Nuim {
    class Renderer {
    public:
        bool Init(HWND hwnd, int width, int height);

        void BeginFrame(const float clearColor[4]);
        void BeginFrame(const float clearColor[4], ID3D11RenderTargetView* rtv, FrameBuffers& fb);
        void EndFrame();

        void Resize(int width, int height);

        void SetCamera(
            const DirectX::XMMATRIX& view,
            const DirectX::XMMATRIX& proj
        );

        void DrawItem(const RenderItem& item);

        ID3D11Device* GetDevice() { return m_device.Device(); }
        ID3D11DeviceContext* GetContext() { return m_device.Context(); }

    public:
        const RenderStats& GetStats() const { return m_stats; }
        void ResetStats() { m_stats.Reset(); }

    private:
        GraphicsDevice m_device;
        SwapChain m_swapChain;
        FrameBuffers m_frameBuffers;
        RenderContext m_renderContext;

        RenderStats m_stats;
    };
}