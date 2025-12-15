#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "SwapChain.hpp"
#include "DepthStencil.hpp"

namespace Nuim {

    class SwapChainTarget {
    public:
        bool Init(HWND hwnd, ID3D11Device* dev, int w, int h, bool vsync);
        bool Resize(ID3D11Device* dev, int w, int h);
        void Present();

        void Bind(ID3D11DeviceContext* ctx);
        void Clear(ID3D11DeviceContext* ctx, const float color[4], bool clearDepth = true, bool clearStencil = true);

        ID3D11RenderTargetView* RTV() const { return m_sc.RTV(); }
        int Width() const { return m_w; }
        int Height() const { return m_h; }

    private:
        SwapChain m_sc;
        DepthStencil m_depth;
        D3D11_VIEWPORT m_vp{};
        bool m_vsync = true;
        int m_w = 0, m_h = 0;
    };

}
