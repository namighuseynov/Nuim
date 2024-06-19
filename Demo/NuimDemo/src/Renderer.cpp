#include "NuimDemoPCH.h"
#include "BaseWindow.h"
#include "Renderer.hpp"


namespace NuimDemo {

	Renderer::Renderer(BaseWindow* window) {
		this->window = window;
		Initialize();
		
	}

	void Renderer::ShutDown() {
		//this->swapChain->Release();
		//this->backBuffer->Release();		
		////this->deviceContext->Release();
		//this->device->Release();

	}

	void Renderer::RenderFrame()
	{
		float clearColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		this->deviceContext->ClearRenderTargetView(backBuffer, clearColor);
		this->swapChain->Present(0, 0);
	}

	bool Renderer::Initialize() {
		//Swap chain desc
		ZeroMemory(&this->sd, sizeof(DXGI_SWAP_CHAIN_DESC));
		this->sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		this->sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		this->sd.BufferCount = 1;
		this->sd.OutputWindow = this->window->GetHWND();
		this->sd.Windowed = true;
		this->sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		this->sd.SampleDesc.Count = 4;	
		this->sd.BufferDesc.Width = this->window->Width;
		this->sd.BufferDesc.Height = this->window->Height;
		this->sd.Flags = 0;
		this->sd.BufferDesc.RefreshRate.Numerator = 60;
		//Device and context
		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			NULL,
			NULL,
			NULL,
			D3D11_SDK_VERSION,
			&sd,
			&swapChain,
			&device,
			NULL,
			&deviceContext
		);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"Failed to create d3d11 device", L"Error", 0);
			return false;
		}

		// Viewport
		ID3D11Texture2D* pBackBuffer;
		this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

		this->device->CreateRenderTargetView(pBackBuffer, NULL, &this->backBuffer);
		pBackBuffer->Release();

		this->deviceContext->OMSetRenderTargets(1, &this->backBuffer, NULL);

		ZeroMemory(&this->viewport, sizeof(D3D11_VIEWPORT));
		this->viewport.TopLeftX = 0;
		this->viewport.TopLeftY = 0;
		this->viewport.Width = this->window->Width;
		this->viewport.Height = this->window->Height;	

		this->deviceContext->RSSetViewports(1, &this->viewport);
		return true;
	}
}
