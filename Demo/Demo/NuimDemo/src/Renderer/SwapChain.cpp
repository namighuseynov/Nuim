#include "NuimDemoPCH.h"
#include "SwapChain.hpp"

namespace Nuim {

    bool SwapChain::Init(HWND hwnd, ID3D11Device* device, int w, int h)
    {
        if (!hwnd || !device || w <= 0 || h <= 0) return false;

        DXGI_SWAP_CHAIN_DESC scd{};
        scd.BufferCount = 2;
        scd.BufferDesc.Width = w;
        scd.BufferDesc.Height = h;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;

        Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
        if (FAILED(device->QueryInterface(IID_PPV_ARGS(&dxgiDevice)))) return false;

        Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
        if (FAILED(dxgiDevice->GetAdapter(&adapter))) return false;

        Microsoft::WRL::ComPtr<IDXGIFactory> factory;
        if (FAILED(adapter->GetParent(IID_PPV_ARGS(&factory)))) return false;

        factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

        HRESULT hr = factory->CreateSwapChain(device, &scd, &m_sc);
        if (FAILED(hr)) return false;

        return CreateRTV(device);
    }

    bool SwapChain::Resize(ID3D11Device* device, int w, int h)
    {
        if (!device || !m_sc || w <= 0 || h <= 0) return false;

        m_rtv.Reset();

        HRESULT hr = m_sc->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
        if (FAILED(hr)) return false;

        return CreateRTV(device);
    }

    bool SwapChain::CreateRTV(ID3D11Device* device)
    {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        HRESULT hr = m_sc->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        if (FAILED(hr)) return false;

        hr = device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
        if (FAILED(hr)) return false;

        return true;
    }

    void SwapChain::Present(bool vsync)
    {
        if (m_sc) m_sc->Present(vsync ? 1 : 0, 0);
    }

}
