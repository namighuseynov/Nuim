#include "NuimDemoPCH.h"
#include "RenderTarget2D.hpp"

namespace Nuim {

    bool RenderTarget2D::Create(ID3D11Device* dev, int w, int h, DXGI_FORMAT fmt)
    {
        m_w = w; m_h = h; m_fmt = fmt;
        if (!CreateColor(dev)) return false;
        if (!m_depth.Create(dev, w, h)) return false;

        m_vp.TopLeftX = 0.0f;
        m_vp.TopLeftY = 0.0f;
        m_vp.Width = (float)w;
        m_vp.Height = (float)h;
        m_vp.MinDepth = 0.0f;
        m_vp.MaxDepth = 1.0f;

        return true;
    }

    bool RenderTarget2D::Resize(ID3D11Device* dev, int w, int h)
    {
        if (w <= 0 || h <= 0) return false;
        if (w == m_w && h == m_h && m_rtv && m_srv) return true;

        m_w = w; m_h = h;

        m_color.Reset();
        m_rtv.Reset();
        m_srv.Reset();
        m_depth.Reset();

        if (!CreateColor(dev)) return false;
        if (!m_depth.Create(dev, w, h)) return false;

        m_vp.TopLeftX = 0.0f;
        m_vp.TopLeftY = 0.0f;
        m_vp.Width = (float)w;
        m_vp.Height = (float)h;
        m_vp.MinDepth = 0.0f;
        m_vp.MaxDepth = 1.0f;

        return true;
    }

    bool RenderTarget2D::CreateColor(ID3D11Device* dev)
    {
        D3D11_TEXTURE2D_DESC td = {};
        td.Width = m_w;
        td.Height = m_h;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = m_fmt;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        HRESULT hr = dev->CreateTexture2D(&td, nullptr, &m_color);
        if (FAILED(hr)) return false;

        hr = dev->CreateRenderTargetView(m_color.Get(), nullptr, &m_rtv);
        if (FAILED(hr)) return false;

        hr = dev->CreateShaderResourceView(m_color.Get(), nullptr, &m_srv);
        if (FAILED(hr)) return false;

        return true;
    }

    void RenderTarget2D::Bind(ID3D11DeviceContext* ctx)
    {
        ID3D11RenderTargetView* rtv = m_rtv.Get();
        ctx->OMSetRenderTargets(1, &rtv, m_depth.DSV());
        ctx->OMSetDepthStencilState(m_depth.DSS(), 1);
        ctx->RSSetViewports(1, &m_vp);
    }

    void RenderTarget2D::Clear(ID3D11DeviceContext* ctx, const float color[4], bool clearDepth, bool clearStencil)
    {
        ctx->ClearRenderTargetView(m_rtv.Get(), color);

        UINT flags = 0;
        if (clearDepth) flags |= D3D11_CLEAR_DEPTH;
        if (clearStencil) flags |= D3D11_CLEAR_STENCIL;

        if (flags)
            ctx->ClearDepthStencilView(m_depth.DSV(), flags, 1.0f, 0);
    }

}
