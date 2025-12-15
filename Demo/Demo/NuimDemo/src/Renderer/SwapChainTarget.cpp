#include "NuimDemoPCH.h"
#include "SwapChainTarget.hpp"

namespace Nuim {

    bool SwapChainTarget::Init(HWND hwnd, ID3D11Device* dev, int w, int h, bool vsync)
    {
        m_vsync = vsync;
        m_w = w; m_h = h;

        if (!m_sc.Init(hwnd, dev, w, h)) return false;
        if (!m_depth.Create(dev, w, h)) return false;

        m_vp.TopLeftX = 0.0f;
        m_vp.TopLeftY = 0.0f;
        m_vp.Width = (float)w;
        m_vp.Height = (float)h;
        m_vp.MinDepth = 0.0f;
        m_vp.MaxDepth = 1.0f;

        return true;
    }

    bool SwapChainTarget::Resize(ID3D11Device* dev, int w, int h)
    {
        if (w <= 0 || h <= 0) return false;
        m_w = w; m_h = h;

        if (!m_sc.Resize(dev, w, h)) return false;
        if (!m_depth.Create(dev, w, h)) return false;

        m_vp.Width = (float)w;
        m_vp.Height = (float)h;

        return true;
    }

    void SwapChainTarget::Bind(ID3D11DeviceContext* ctx)
    {
        ID3D11RenderTargetView* rtv = m_sc.RTV();
        ctx->OMSetRenderTargets(1, &rtv, m_depth.DSV());
        ctx->OMSetDepthStencilState(m_depth.DSS(), 1);
        ctx->RSSetViewports(1, &m_vp);
    }

    void SwapChainTarget::Clear(ID3D11DeviceContext* ctx, const float color[4], bool clearDepth, bool clearStencil)
    {
        ctx->ClearRenderTargetView(m_sc.RTV(), color);

        UINT flags = 0;
        if (clearDepth)   flags |= D3D11_CLEAR_DEPTH;
        if (clearStencil) flags |= D3D11_CLEAR_STENCIL;

        if (flags)
            ctx->ClearDepthStencilView(m_depth.DSV(), flags, 1.0f, 0);
    }

    void SwapChainTarget::Present()
    {
        m_sc.Present(m_vsync);
    }

}
