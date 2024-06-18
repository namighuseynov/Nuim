#include "NuimDemoPCH.h"
#include "BaseWindow.h"
#include "Renderer.hpp"


namespace NuimDemo {

	Renderer::Renderer(BaseWindow* window) {
		Initialize();
		this->window = window;
	}

	bool Renderer::Initialize() {
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
		UINT m4xMsaaQuality;
		this->device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
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
		return true;
	}
}
