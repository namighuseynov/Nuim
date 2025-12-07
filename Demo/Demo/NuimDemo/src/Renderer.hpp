#pragma once
#include "Window.hpp"
#include "ConstantBufferData.hpp"
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")


class Renderer {
public:
    Renderer(HWND hwnd, int width, int height) {
        // Initialize Direct3D
        if (!InitD3D(hwnd, width, height))
        {
            CleanupDeviceD3D();
        }
        if (!LoadShaders())
        {
            MessageBox(nullptr, L"Failed to load shaders!", L"Error", MB_OK);
        }
        CreateConstantBuffer();
    }

    bool InitD3D(HWND hWnd, int width, int height)
    {
        DXGI_SWAP_CHAIN_DESC scd = {};
        scd.BufferCount = 1;
        scd.BufferDesc.Width = width;
        scd.BufferDesc.Height = height;
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hWnd;
        scd.SampleDesc.Count = 1;
        scd.Windowed = TRUE;

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
            nullptr, 0, D3D11_SDK_VERSION,
            &scd, &g_pSwapChain, &g_pd3dDevice, nullptr, &g_pd3dDeviceContext
        );

        if (FAILED(hr)) return false;

        ID3D11Texture2D* backBuffer = nullptr;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
        g_pd3dDevice->CreateRenderTargetView(backBuffer, nullptr, &g_mainRenderTargetView);
        backBuffer->Release();

        if (!CreateDepthStencil(width, height)) {
			std::cout << "error creating depth stencil" << std::endl;
        }

        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, g_pDepthStencilView);

        D3D11_VIEWPORT vp = {};
        vp.Width = (float)width;
        vp.Height = (float)height;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        g_pd3dDeviceContext->RSSetViewports(1, &vp);

        return true;
    }

    void BeginRender(const float clearColor[4] = nullptr) 
    {
        static float angle = 0.0f;
        angle += 0.01f; // rotate object

        DirectX::XMMATRIX world =
            DirectX::XMMatrixRotationX(angle * 0.7f) *
            DirectX::XMMatrixRotationY(angle * 0.9f) *
            DirectX::XMMatrixRotationZ(angle * 0.5f);

        DirectX::XMMATRIX view =
            DirectX::XMMatrixLookAtLH(
                DirectX::XMVectorSet(0.0f, 0.0f, -4.0f, 1.0f),
                DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
            );

        DirectX::XMMATRIX projection =
            DirectX::XMMatrixPerspectiveFovLH(
                DirectX::XMConvertToRadians(60.0f),
                1280.0f / 720.0f,
                0.1f,
                100.0f
            );

        DirectX::XMMATRIX wvp = world * view * projection;
        wvp = DirectX::XMMatrixTranspose(wvp);

        ConstantBufferData cbData;
        cbData.wvp = wvp;

        g_pd3dDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cbData, 0, 0);
        g_pd3dDeviceContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

        float defaultColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
        float* color = clearColor ? (float*)clearColor : defaultColor;

        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, g_pDepthStencilView);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, color);
        g_pd3dDeviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
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
        if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }

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

        if (g_pDepthStencilView) { g_pDepthStencilView->Release(); g_pDepthStencilView = nullptr; }
        if (g_pDepthStencil) { g_pDepthStencil->Release(); g_pDepthStencil = nullptr; }

        CreateDepthStencil(width, height);
    }

    bool CreateConstantBuffer()
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DEFAULT;
        cbd.ByteWidth = sizeof(ConstantBufferData);
        cbd.CPUAccessFlags = 0;

        HRESULT hr = g_pd3dDevice->CreateBuffer(&cbd, nullptr, &g_pConstantBuffer);
        return SUCCEEDED(hr);
    }

    bool CreateDepthStencil(int width, int height) {
        D3D11_TEXTURE2D_DESC desc = {};
        desc.Width = width;
        desc.Height = height;
        desc.MipLevels = 1;
        desc.ArraySize = 1;
        desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc.Count = 1;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        HRESULT hr = g_pd3dDevice->CreateTexture2D(&desc, NULL, &g_pDepthStencil);
        if (FAILED(hr)) return false;

        hr = g_pd3dDevice->CreateDepthStencilView(g_pDepthStencil, NULL, &g_pDepthStencilView);
        if (FAILED(hr)) return false;


        D3D11_DEPTH_STENCIL_DESC dsDesc = {};
        dsDesc.DepthEnable = TRUE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

        ID3D11DepthStencilState* pDSState;
        hr = g_pd3dDevice->CreateDepthStencilState(&dsDesc, &pDSState);
        if (FAILED(hr)) return false;
        g_pd3dDeviceContext->OMSetDepthStencilState(pDSState, 1);

        return true;
    }
public:
    ID3D11Device* GetDevice() { return g_pd3dDevice; }
    ID3D11DeviceContext* GetContext() { return g_pd3dDeviceContext; }
    IDXGISwapChain* GetSwapChain() { return g_pSwapChain; }
    ID3D11RenderTargetView* GetTargetView() { return g_mainRenderTargetView; }

    void CleanupDeviceD3D()
    {
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
        if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
        if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
        if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
		if (g_pConstantBuffer) { g_pConstantBuffer->Release(); g_pConstantBuffer = nullptr; }
		if (g_pDepthStencil) { g_pDepthStencil->Release(); g_pDepthStencil = nullptr; }
		if (g_pDepthStencilView) { g_pDepthStencilView->Release(); g_pDepthStencilView = nullptr; }
    }

    void CreateRenderTarget()
    {
        ID3D11Texture2D* pBackBuffer;
        g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
        g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
        pBackBuffer->Release();
    }

    bool LoadShaders() {
        HRESULT hr;

        ID3DBlob* vsBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

		hr = D3DCompileFromFile(
			L"Shaders/VertexShader.hlsl",
			nullptr, nullptr,
			"main", "vs_5_0",
			0, 0,
			&vsBlob, &errorBlob
		);

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "VS Compile Error", MB_OK);
            }
            return false;
        }

        hr = g_pd3dDevice->CreateVertexShader(
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr,
            &g_pVertexShader
        );

        if (FAILED(hr))
        {
            return false;
        }

		D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

        //D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
        //{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        //};

        hr = g_pd3dDevice->CreateInputLayout(
            layoutDesc,
            ARRAYSIZE(layoutDesc),
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            &g_pInputLayout
        );

        if (FAILED(hr))
        {
            std::cout << "Failed to create input layout" << std::endl;
            return false;
        }
        vsBlob->Release();

        ID3DBlob* psBlob = nullptr;
		hr = D3DCompileFromFile(
			L"Shaders/PixelShader.hlsl",
			nullptr, nullptr,
			"main", "ps_5_0",
			0, 0,
			&psBlob, &errorBlob
		);

        if (FAILED(hr))
        {
            if (errorBlob)
                MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "PS Compile Error", MB_OK);
            return false;
        }

        hr = g_pd3dDevice->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            &g_pPixelShader
        );
        psBlob->Release();

        if (FAILED(hr))
        {
            return false;
        }
        return true;
    }
private:
    // Data
    ID3D11Device* g_pd3dDevice = nullptr;
    ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
    IDXGISwapChain* g_pSwapChain = nullptr;
    ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
    ID3D11Texture2D* g_pDepthStencil = nullptr;
    ID3D11DepthStencilView* g_pDepthStencilView = nullptr;

    ID3D11VertexShader* g_pVertexShader = nullptr;
    ID3D11PixelShader* g_pPixelShader = nullptr;
    ID3D11InputLayout* g_pInputLayout = nullptr;
    ID3D11Buffer* g_pConstantBuffer = nullptr;
};

