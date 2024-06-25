#include "NuimDemoPCH.h"
#include "BaseWindow.h"
#include "D3DApp.hpp"

namespace NuimDemo {
	bool D3DApp::CreateDeviceD3D() {
		DXGI_SWAP_CHAIN_DESC sd = {};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = window->GetHWND();
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

        UINT createDeviceFlags = 0;
        //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &this->dxgi_swapChain, &this->d3d_device, &featureLevel, &this->d3d_deviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &dxgi_swapChain, &d3d_device, &featureLevel, &d3d_deviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();

        //D3D11_VIEWPORT viewport;
        //viewport.TopLeftX = 0;
        //viewport.TopLeftY = 0;
        //viewport.Width = window->Width;
        //viewport.Height = window->Height;

        //this->d3d_deviceContext->RSSetViewports(1, &viewport);
        return true;
	}
    void D3DApp::CleanupDeviceD3D() {
        CleanupRenderTarget();
        if (dxgi_swapChain) { dxgi_swapChain->Release(); dxgi_swapChain = nullptr; }
        if (d3d_deviceContext) { d3d_deviceContext->Release(); d3d_deviceContext = nullptr; }
        if (d3d_device) { d3d_device->Release(); d3d_device = nullptr; }
    }
    void D3DApp::CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        dxgi_swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        d3d_device->CreateRenderTargetView(pBackBuffer, nullptr, &d3d_backBuffer);
        pBackBuffer->Release();
    }
    void D3DApp::CleanupRenderTarget()
    {
        if (d3d_backBuffer) { d3d_backBuffer->Release(); d3d_backBuffer = nullptr; }
    }
}