#pragma once
#include <d3d11.h>

// directX device

// directX context device

// pCom

// Swap Chain

// Resources

// Math

namespace NuimDemo {
	
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	HRESULT hr = D3D11CreateDevice(
		0, //Default Adapter
		D3D_DRIVER_TYPE_HARDWARE,
		0, //No software module
		D3D11_CREATE_DEVICE_DEBUG,
		0, 0,
		D3D11_SDK_VERSION,
		&device,
		&featureLevel,
		&deviceContext
		);

	class Renderer
	{
		void Initialize() {

		}
	private:
		
	};
}


