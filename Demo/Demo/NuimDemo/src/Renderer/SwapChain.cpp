#include "NuimDemoPCH.h"
#include "SwapChain.hpp"

namespace Nuim {
    bool SwapChain::Init(HWND hwnd, ID3D11Device* device, int w, int h) {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 2;
        scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        scd.Flags = 0;
        scd.BufferDesc.Width = w;
        scd.BufferDesc.Height = h;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;
        scd.BufferDesc.RefreshRate.Numerator = 60;
        scd.BufferDesc.RefreshRate.Denominator = 1;

        Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
        device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));

        Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
        dxgiDevice->GetAdapter(&adapter);

        Microsoft::WRL::ComPtr<IDXGIFactory> factory;
        adapter->GetParent(IID_PPV_ARGS(&factory));

        HRESULT hr = factory->CreateSwapChain(device, &scd, &m_swapChain);
        if (FAILED(hr)) return false;

        CreateRTV(device);
        return true;
    }

    bool SwapChain::Resize(ID3D11Device* device, int w, int h)
    {
        m_rtv.Reset();
        HRESULT hr = m_swapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
        if (FAILED(hr)) return false;
        CreateRTV(device);
        return true;
    }

    void SwapChain::CreateRTV(ID3D11Device* device) {
        Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
        m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        device->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_rtv);
    }

    void SwapChain::Present(bool vsync) {
        m_swapChain->Present(vsync ? 1 : 0, 0);
    }
}


