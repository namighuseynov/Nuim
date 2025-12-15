#include "NuimDemoPCH.h"
#include "DepthStencil.hpp"

namespace Nuim {

    bool DepthStencil::Create(ID3D11Device* dev, int w, int h, DXGI_FORMAT fmt)
    {
        Reset();
        m_w = w; m_h = h;

        D3D11_TEXTURE2D_DESC td = {};
        td.Width = w;
        td.Height = h;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = fmt;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        HRESULT hr = dev->CreateTexture2D(&td, nullptr, &m_tex);
        if (FAILED(hr)) return false;

        hr = dev->CreateDepthStencilView(m_tex.Get(), nullptr, &m_dsv);
        if (FAILED(hr)) return false;

        D3D11_DEPTH_STENCIL_DESC ds = {};
        ds.DepthEnable = TRUE;
        ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        ds.DepthFunc = D3D11_COMPARISON_LESS;

        ds.StencilEnable = TRUE;
        ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

        ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        ds.BackFace = ds.FrontFace;

        hr = dev->CreateDepthStencilState(&ds, &m_dss);
        if (FAILED(hr)) return false;

        return true;
    }

    void DepthStencil::Reset()
    {
        m_tex.Reset();
        m_dsv.Reset();
        m_dss.Reset();
        m_w = m_h = 0;
    }

}
