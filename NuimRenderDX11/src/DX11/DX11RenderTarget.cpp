#include "NuimPCH.h"
#include "DX11/DX11RenderTarget.hpp"
#include "DX11/DX11RenderContext.hpp"

#include <stdexcept>

namespace Nuim::DX11 {

    DX11RenderTarget::DX11RenderTarget(DX11RenderContext& ctx, const Nuim::Render::RenderTargetDesc& desc)
        : m_ctx(ctx)
    {
        m_width = desc.Width ? desc.Width : 1;
        m_height = desc.Height ? desc.Height : 1;
        m_hasDepth = desc.HasDepth;

        CreateResources();
    }

    void DX11RenderTarget::ReleaseResources()
    {
        m_dsv.Reset();
        m_depthTex.Reset();

        m_srv.Reset();
        m_rtv.Reset();
        m_colorTex.Reset();
    }

    void DX11RenderTarget::CreateResources()
    {
        ReleaseResources();

        // Color
        D3D11_TEXTURE2D_DESC td{};
        td.Width = (UINT)m_width;
        td.Height = (UINT)m_height;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.SampleDesc.Count = 1;
        td.SampleDesc.Quality = 0;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        td.CPUAccessFlags = 0;
        td.MiscFlags = 0;

        HRESULT hr = m_ctx.GetDevice()->CreateTexture2D(&td, nullptr, m_colorTex.GetAddressOf());
        if (FAILED(hr))
            throw std::runtime_error("DX11RenderTarget: CreateTexture2D(color) failed");

        hr = m_ctx.GetDevice()->CreateRenderTargetView(m_colorTex.Get(), nullptr, m_rtv.GetAddressOf());
        if (FAILED(hr))
            throw std::runtime_error("DX11RenderTarget: CreateRenderTargetView failed");

        hr = m_ctx.GetDevice()->CreateShaderResourceView(m_colorTex.Get(), nullptr, m_srv.GetAddressOf());
        if (FAILED(hr))
            throw std::runtime_error("DX11RenderTarget: CreateShaderResourceView failed");

        // Depth
        if (m_hasDepth)
        {
            D3D11_TEXTURE2D_DESC dd{};
            dd.Width = (UINT)m_width;
            dd.Height = (UINT)m_height;
            dd.MipLevels = 1;
            dd.ArraySize = 1;
            dd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            dd.SampleDesc.Count = 1;
            dd.SampleDesc.Quality = 0;
            dd.Usage = D3D11_USAGE_DEFAULT;
            dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            dd.CPUAccessFlags = 0;
            dd.MiscFlags = 0;

            hr = m_ctx.GetDevice()->CreateTexture2D(&dd, nullptr, m_depthTex.GetAddressOf());
            if (FAILED(hr))
                throw std::runtime_error("DX11RenderTarget: CreateTexture2D(depth) failed");

            hr = m_ctx.GetDevice()->CreateDepthStencilView(m_depthTex.Get(), nullptr, m_dsv.GetAddressOf());
            if (FAILED(hr))
                throw std::runtime_error("DX11RenderTarget: CreateDepthStencilView failed");
        }
    }

    void DX11RenderTarget::Resize(Nuim::U32 width, Nuim::U32 height)
    {
        if (width == 0 || height == 0)
            return;

        if (width == m_width && height == m_height)
            return;

        m_width = width;
        m_height = height;
        CreateResources();
    }

    void DX11RenderTarget::Bind()
    {
        ID3D11RenderTargetView* rtvs[] = { m_rtv.Get() };
        ID3D11DepthStencilView* dsv = m_hasDepth ? m_dsv.Get() : nullptr;

        m_ctx.GetContext()->OMSetRenderTargets(1, rtvs, dsv);

        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Width = (FLOAT)m_width;
        vp.Height = (FLOAT)m_height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;

        m_ctx.GetContext()->RSSetViewports(1, &vp);
    }

    void DX11RenderTarget::Clear(float r, float g, float b, float a)
    {
        float color[4] = { r, g, b, a };
        m_ctx.GetContext()->ClearRenderTargetView(m_rtv.Get(), color);

        if (m_hasDepth && m_dsv)
            m_ctx.GetContext()->ClearDepthStencilView(m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }

}
