#pragma once
#include "Window.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


class Renderer {
public:
    Renderer(HWND hwnd) {
        // Initialize Direct3D
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
        }
        CompileShaders();
    }

    void BeginRender(const float clearColor[4] = nullptr) 
    {
        float defaultColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        float* color = clearColor ? (float*)clearColor : defaultColor;

        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, color);

        g_pd3dDeviceContext->IASetInputLayout(g_pInputLayout);
        g_pd3dDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
        g_pd3dDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
    }

    void EndRender()
    {
        g_pSwapChain->Present(1, 0);
    }

    void Resize(UINT width, UINT height)
    {
        if (!g_pSwapChain) return;
        g_pd3dDeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
        CleanupRenderTarget();

        HRESULT hr = g_pSwapChain->ResizeBuffers(
            2,
            width,
            height,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        );

        if (FAILED(hr))
        {
            std::cout << "ResizeBuffers failed: " << hr << std::endl;
            return;
        }

        CreateRenderTarget();
    }
public:
    ID3D11Device* GetDevice() { return g_pd3dDevice; }
    ID3D11DeviceContext* GetContext() { return g_pd3dDeviceContext; }
    IDXGISwapChain* GetSwapChain() { return g_pSwapChain; }
    ID3D11RenderTargetView* GetTargetView() { return g_mainRenderTargetView; }
    bool CreateDeviceD3D(HWND hWnd)
    {
        // Setup swap chain
        DXGI_SWAP_CHAIN_DESC sd;
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = 2;
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hWnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;


        UINT createDeviceFlags = 0;
        D3D_FEATURE_LEVEL featureLevel;
        const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
        HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
        if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
            res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
        if (res != S_OK)
            return false;

        CreateRenderTarget();
        return true;
    }

    void CleanupDeviceD3D()
    {
        CleanupRenderTarget();
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    void CleanupRenderTarget()
    {
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
    }

    void CompileShaders() {
        ID3DBlob* vsBlob = nullptr;
        ID3DBlob* psBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

        // Vertex shader compile
        HRESULT hr = D3DCompileFromFile(
            L"Shaders/VertexShader.hlsl",
            nullptr, nullptr,
            "main", "vs_5_0",
            0, 0,
			&vsBlob, &errorBlob
        );

        if (FAILED(hr)) {
            std::wstring msg = L"Error loading shader. Current directory: ";
            wchar_t buffer[MAX_PATH];
            GetCurrentDirectoryW(MAX_PATH, buffer);
            std::wcout << L"Current Working Directory: " << buffer << std::endl;

            msg += buffer;
            msg += L"\n";

            OutputDebugStringW(msg.c_str());
        }

        // Pixel shader compile
        D3DCompileFromFile(
            L"Shaders/PixelShader.hlsl",
            nullptr, nullptr,
            "main", "ps_5_0",
            0, 0,
            &psBlob, nullptr
        );

		// Create shader objects
        g_pd3dDevice->CreateVertexShader(
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr, &g_pVertexShader
        );

        g_pd3dDevice->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr, &g_pPixelShader
        );

        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
              0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

              { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
                12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        UINT numElements = ARRAYSIZE(layout);

        g_pd3dDevice->CreateInputLayout(
            layout,
            numElements,
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            &g_pInputLayout
        );

        vsBlob->Release();
        psBlob->Release();
    }
private:
    // Data
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

    ID3D11VertexShader* g_pVertexShader = nullptr;
    ID3D11PixelShader* g_pPixelShader = nullptr;
    ID3D11InputLayout* g_pInputLayout = nullptr;
};

