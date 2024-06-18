#include "NuimDemoPCH.h"
#include "BaseWindow.h"
#include "Renderer.hpp"


namespace NuimDemo {

	Renderer::Renderer(BaseWindow* window) {
		this->window = window;
		Initialize();
		
	}

	bool Renderer::Initialize() {
		//Device and context
		HRESULT hr = D3D11CreateDevice(
			0,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			D3D11_CREATE_DEVICE_DEBUG,
			0,
			0,
			D3D11_SDK_VERSION,
			&this->device,
			&this->featureLevel,
			&this->deviceContext
		);
		if (FAILED(hr)) {
			MessageBox(nullptr, L"Failed to create d3d11 device", L"Error", 0);
			return false;
		}
		if (featureLevel != D3D_FEATURE_LEVEL_11_0) {
			MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
			return false;
		}

		//MSAA
		UINT m4xMsaaQuality;
		this->device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);

		//Swap chain desc
		this->sd.BufferDesc.Width = this->window->Width;
		this->sd.BufferDesc.Height = this->window->Height;
		this->sd.BufferDesc.RefreshRate.Numerator = 60;
		this->sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		this->sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		this->sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		this->sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		this->sd.BufferCount = 1;
		this->sd.OutputWindow = this->window->GetHWND();
		this->sd.Windowed = true;
		this->sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		this->sd.Flags = 0;

		//Swap chain
		ComPtr<IDXGIDevice> dxgiDevice;
		this->device.As(&dxgiDevice);
		ComPtr<IDXGIAdapter> dxgiAdapter;
		dxgiDevice->GetAdapter(&dxgiAdapter);
		ComPtr<IDXGIFactory> dxgiFactory;
		dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)& dxgiFactory);

		//this->device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
		//IDXGIAdapter* dxgiAdapter = 0;
		//dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
		//IDXGIFactory* dxgiFactory = 0;
		//dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

		HRESULT res = dxgiFactory->CreateSwapChain(
			this->device.Get(),
			&this->sd,
			&this->swapChain
		);
		return true;
	}
}
