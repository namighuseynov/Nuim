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
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &this->dxgi_swapChain, &this->d3d_device, &featureLevel, &this->d3d_deviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &dxgi_swapChain, &d3d_device, &featureLevel, &d3d_deviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();
        //this->d3d_deviceContext->OMSetRenderTargets(1, &this->d3d_backBuffer, nullptr);
        D3D11_VIEWPORT viewport;
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = window->Width;
        viewport.Height = window->Height;
        //d3d_deviceContext->OMSetRenderTargets(1, &d3d_backBuffer, nullptr);
        this->d3d_deviceContext->RSSetViewports(1, &viewport);
        return true;
	}
    void D3DApp::CleanupDeviceD3D() {
        CleanupRenderTarget();
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
        if (d3d_backBuffer) { d3d_backBuffer.Reset(); d3d_backBuffer = nullptr; }
    }
    void D3DApp::Begin()
    {
        if (!CreateDeviceD3D()) {
            CleanupDeviceD3D();
            MessageBox(nullptr, L"Error creating device", L"Error", 0);
        }
        this->window->Show();
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends
        if (!ImGui_ImplWin32_Init(this->window->GetHWND())) {
            MessageBox(nullptr, L"Imgui win32 init error", L"Error", 0);
        }
        if (!ImGui_ImplDX11_Init(this->d3d_device.Get(), this->d3d_deviceContext.Get())) {
            MessageBox(nullptr, L"Imgui DX11 init error", L"Error", 0);
        }
    }
    void D3DApp::ShutDown()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
    }
    void D3DApp::Render()
    {
        CleanupRenderTarget();
        CreateRenderTarget();

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Poll and handle messages (inputs, window resize, etc.)

        ImGui::ShowDemoWindow();
        // Rendering
        ImGui::Render();
        float clearColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
        //this->d3dApp->d3d_deviceContext->OMSetRenderTargets(1, &this->d3dApp->d3d_backBuffer, nullptr);
        this->d3d_deviceContext->ClearRenderTargetView(this->d3d_backBuffer.Get(), clearColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = this->dxgi_swapChain->Present(1, 0);
    }
}