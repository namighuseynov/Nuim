#include "NuimPCH.h"
#include "Nuim/DX11/DX11Context.hpp"
#include "Nuim/DX11/DX11Helpers.hpp"

namespace Nuim {

    void DX11Context::Init(void* nativeWindowHandle, U32 width, U32 height, bool enableDebug)
    {
        m_hwnd = (HWND)nativeWindowHandle;
        m_width = width;
        m_height = height;

        UINT createFlags = 0;
#if defined(NUIM_DEBUG)
        if (enableDebug) createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
        D3D_FEATURE_LEVEL createdFL{};

        DXGI_SWAP_CHAIN_DESC scd{};
        scd.BufferCount = 2;
        scd.BufferDesc.Width = width;
        scd.BufferDesc.Height = height;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = m_hwnd;
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; 

        NUIM_DX11_CHECK(D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createFlags,
            featureLevels,
            1,
            D3D11_SDK_VERSION,
            &scd,
            m_swapChain.GetAddressOf(),
            m_device.GetAddressOf(),
            &createdFL,
            m_context.GetAddressOf()
        ));

        CreateBackbufferRTV();
        SetViewport(width, height);
    }

    void DX11Context::Shutdown()
    {
        m_rtv.Reset();
        m_swapChain.Reset();
        m_context.Reset();
        m_device.Reset();

        m_hwnd = nullptr;
        m_width = m_height = 0;
    }

    void DX11Context::CreateBackbufferRTV()
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        NUIM_DX11_CHECK(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));
        NUIM_DX11_CHECK(m_device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_rtv.GetAddressOf()));

        ID3D11RenderTargetView* rtvs[] = { m_rtv.Get() };
        m_context->OMSetRenderTargets(1, rtvs, nullptr);
    }

    void DX11Context::SetViewport(U32 width, U32 height)
    {
        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_context->RSSetViewports(1, &vp);
    }

    void DX11Context::Resize(U32 width, U32 height)
    {
        if (!m_swapChain) return;
        if (width == 0 || height == 0) return;

        m_width = width;
        m_height = height;

        m_rtv.Reset();
        m_context->OMSetRenderTargets(0, nullptr, nullptr);

        NUIM_DX11_CHECK(m_swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0));

        CreateBackbufferRTV();
        SetViewport(width, height);
    }

    void DX11Context::Clear(float r, float g, float b, float a)
    {
        float color[4] = { r, g, b, a };
        m_context->ClearRenderTargetView(m_rtv.Get(), color);
    }

    void DX11Context::Present(bool vsync)
    {
        m_swapChain->Present(vsync ? 1 : 0, 0);
    }

}
