#include "NuimPCH.h"
#include "DX11/DX11SwapChain.hpp"

#ifdef NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#include <stdexcept>

using Microsoft::WRL::ComPtr;

namespace Nuim::DX11 {

    static void ThrowIfFailed(HRESULT hr, const char* msg)
    {
        if (FAILED(hr))
            throw std::runtime_error(msg);
    }

    DX11SwapChain::DX11SwapChain(Render::IRenderContext& ctx, const Render::SwapChainDesc& desc)
    {
        m_device = (ID3D11Device*)ctx.GetNativeDevice();
        m_context = (ID3D11DeviceContext*)ctx.GetNativeContext();

        if (!m_device || !m_context)
            throw std::runtime_error("DX11SwapChain: RenderContext has null device/context");

        m_hwnd = desc.WindowHandle;
        m_width = desc.Width;
        m_height = desc.Height;
        m_vsync = desc.VSync;

        if (!m_hwnd)
            throw std::runtime_error("DX11SwapChain: WindowHandle is null");

        CreateSwapChain(m_hwnd);
        CreateBackbufferRTV();
    }

    void DX11SwapChain::CreateSwapChain(void* windowHandle)
    {
        HWND hwnd = (HWND)windowHandle;

        ComPtr<IDXGIDevice> dxgiDevice;
        ThrowIfFailed(m_device.As(&dxgiDevice), "DX11SwapChain: device As IDXGIDevice failed");

        ComPtr<IDXGIAdapter> adapter;
        ThrowIfFailed(dxgiDevice->GetAdapter(&adapter), "DX11SwapChain: GetAdapter failed");

        ComPtr<IDXGIFactory> factory;
        ThrowIfFailed(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory),
            "DX11SwapChain: GetParent(IDXGIFactory) failed");

        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = m_width;
        sd.BufferDesc.Height = m_height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        ThrowIfFailed(factory->CreateSwapChain(m_device.Get(), &sd, &m_swapChain),
            "DX11SwapChain: CreateSwapChain failed");
    }

    void DX11SwapChain::CreateBackbufferRTV()
    {
        ComPtr<ID3D11Texture2D> backbuffer;
        ThrowIfFailed(m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backbuffer),
            "DX11SwapChain: GetBuffer(Backbuffer) failed");

        ThrowIfFailed(m_device->CreateRenderTargetView(backbuffer.Get(), nullptr, &m_rtv),
            "DX11SwapChain: CreateRenderTargetView failed");
    }

    void DX11SwapChain::DestroyBackbufferRTV()
    {
        m_rtv.Reset();
    }

    void DX11SwapChain::Resize(U32 width, U32 height)
    {
        if (!m_swapChain)
            return;

        if (width == 0 || height == 0)
            return;

        m_width = width;
        m_height = height;

        DestroyBackbufferRTV();

        HRESULT hr = m_swapChain->ResizeBuffers(0, m_width, m_height, DXGI_FORMAT_UNKNOWN, 0);
        ThrowIfFailed(hr, "DX11SwapChain: ResizeBuffers failed");

        CreateBackbufferRTV();
    }

    void DX11SwapChain::BindBackbuffer()
    {
        if (!m_context || !m_rtv)
            return;

        ID3D11RenderTargetView* rtv = m_rtv.Get();
        m_context->OMSetRenderTargets(1, &rtv, nullptr);

        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Width = (float)m_width;
        vp.Height = (float)m_height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_context->RSSetViewports(1, &vp);
    }

    void DX11SwapChain::ClearBackbuffer(float r, float g, float b, float a)
    {
        if (!m_context || !m_rtv)
            return;

        float col[4] = { r, g, b, a };
        m_context->ClearRenderTargetView(m_rtv.Get(), col);
    }

    void DX11SwapChain::Present()
    {
        if (!m_swapChain)
            return;

        m_swapChain->Present(m_vsync ? 1 : 0, 0);
    }

} 
