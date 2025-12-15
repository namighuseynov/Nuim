#include "NuimDemoPCH.h"
#include "FrameBuffers.hpp"

namespace Nuim {

    bool FrameBuffers::Init(ID3D11Device* device, int w, int h) {
        CreateDepth(device, w, h);

        m_viewport = {};
        m_viewport.Width = (float)w;
        m_viewport.Height = (float)h;
        m_viewport.MinDepth = 0.0f;
        m_viewport.MaxDepth = 1.0f;
        return true;
    }

    void FrameBuffers::CreateDepth(ID3D11Device* device, int w, int h) {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = w;
        desc.Height = h;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        device->CreateTexture2D(&desc, nullptr, &m_depth);
        device->CreateDepthStencilView(m_depth.Get(), nullptr, &m_dsv);

        D3D11_DEPTH_STENCIL_DESC ds = {};
        ds.DepthEnable = TRUE;
        ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        ds.DepthFunc = D3D11_COMPARISON_LESS;

        device->CreateDepthStencilState(&ds, &m_dss);
    }

    void FrameBuffers::Bind(ID3D11DeviceContext* ctx, ID3D11RenderTargetView* rtv) {
        ctx->OMSetRenderTargets(1, &rtv, m_dsv.Get());
        ctx->OMSetDepthStencilState(m_dss.Get(), 1);
        ctx->RSSetViewports(1, &m_viewport);
    }

    void FrameBuffers::Clear(ID3D11DeviceContext* ctx, ID3D11RenderTargetView* rtv, const float color[4]) {
        ctx->ClearRenderTargetView(rtv, color);
        ctx->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
    
    bool FrameBuffers::Resize(ID3D11Device* device, int w, int h) {
        m_depth.Reset();
        m_dsv.Reset();
        m_dss.Reset();
        CreateDepth(device, w, h);

        m_viewport.Width = (float)w;
        m_viewport.Height = (float)h;
        return true;
    }
}