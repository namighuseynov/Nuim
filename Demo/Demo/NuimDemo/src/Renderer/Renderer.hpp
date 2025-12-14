#pragma once
#include "GraphicsDevice.hpp"
#include "SwapChain.hpp"
#include "FrameBuffers.hpp"
#include "RenderContext.hpp"

namespace Nuim {
    class Renderer {
    public:
        bool Init(HWND hwnd, int width, int height);

        void BeginFrame(const float clearColor[4]);
        void EndFrame();

        void Resize(int width, int height);

        void SetCamera(
            const DirectX::XMMATRIX& view,
            const DirectX::XMMATRIX& proj
        );

        void DrawItem(const RenderItem& item);

        ID3D11Device* GetDevice() { return m_device.Device(); }
        ID3D11DeviceContext* GetContext() { return m_device.Context(); }

    private:
        GraphicsDevice m_device;
        SwapChain m_swapChain;
        FrameBuffers m_frameBuffers;
        RenderContext m_renderContext;
    };
}