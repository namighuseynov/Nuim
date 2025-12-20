#include "NuimPCH.h"
#include "DX11/DX11SwapChain.hpp"
#include "DX11/DX11RenderContext.hpp"

#include <stdexcept>

namespace Nuim::DX11 {

    static ComPtr<IDXGIFactory> GetFactoryFromDevice(ID3D11Device* device)
    {
        ComPtr<IDXGIDevice> dxgiDevice;
        if (FAILED(device->QueryInterface(IID_PPV_ARGS(dxgiDevice.GetAddressOf()))))
            return nullptr;

        ComPtr<IDXGIAdapter> adapter;
        if (FAILED(dxgiDevice->GetAdapter(adapter.GetAddressOf())))
            return nullptr;

        ComPtr<IDXGIFactory> factory;
        if (FAILED(adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf()))))
            return nullptr;

        return factory;
    }

    DX11SwapChain::DX11SwapChain(DX11RenderContext& ctx, const Nuim::Render::SwapChainDesc& desc)
        : m_ctx(ctx)
    {
        m_hwnd = (HWND)desc.WindowHandle;
        m_width = desc.Width;
        m_height = desc.Height;
        m_vsync = desc.VSync;

        if (!m_hwnd)
            throw std::runtime_error("DX11SwapChain: WindowHandle is null");

        CreateSwapChain(m_hwnd);
        CreateBackbufferRTV();
    }

    void DX11SwapChain::CreateSwapChain(HWND hwnd)
    {
        auto factory = GetFactoryFromDevice(m_ctx.GetDevice());
        if (!factory)
            throw std::runtime_error("DX11SwapChain: failed to get DXGIFactory");

        DXGI_SWAP_CHAIN_DESC scd{};
        scd.BufferDesc.Width = m_width;
        scd.BufferDesc.Height = m_height;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 0;
        scd.BufferDesc.RefreshRate.Denominator = 0;
        scd.SampleDesc.Count = 1;
        scd.SampleDesc.Quality = 0;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.BufferCount = 2; // double buffering
        scd.OutputWindow = hwnd;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;

        HRESULT hr = factory->CreateSwapChain(m_ctx.GetDevice(), &scd, m_swapChain.GetAddressOf());
        if (FAILED(hr))
            throw std::runtime_error("DX11SwapChain: CreateSwapChain failed");
    }

    void DX11SwapChain::CreateBackbufferRTV()
    {
        ComPtr<ID3D11Texture2D> backBuffer;
        HRESULT hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
        if (FAILED(hr))
            throw std::runtime_error("DX11SwapChain: GetBuffer failed");

        hr = m_ctx.GetDevice()->CreateRenderTargetView(backBuffer.Get(), nullptr, m_backbufferRTV.GetAddressOf());
        if (FAILED(hr))
            throw std::runtime_error("DX11SwapChain: CreateRenderTargetView failed");
    }

    void DX11SwapChain::CleanupBackbuffer()
    {
        m_backbufferRTV.Reset();
    }

    void DX11SwapChain::Resize(Nuim::U32 width, Nuim::U32 height)
    {
        if (!m_swapChain)
            return;

        if (width == 0 || height == 0)
            return;

        m_width = width;
        m_height = height;

        CleanupBackbuffer();

        HRESULT hr = m_swapChain->ResizeBuffers(
            0,
            (UINT)m_width,
            (UINT)m_height,
            DXGI_FORMAT_UNKNOWN,
            0
        );

        if (FAILED(hr))
            throw std::runtime_error("DX11SwapChain: ResizeBuffers failed");

        CreateBackbufferRTV();
    }

    void DX11SwapChain::Present()
    {
        if (!m_swapChain) return;
        m_swapChain->Present(m_vsync ? 1 : 0, 0);
    }

}
