#pragma once
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

// Swap Chain

// Resources

// Math

namespace NuimDemo {

	class Renderer
	{
	public:
		Renderer(BaseWindow* window);
	public:
		bool Initialize();
	private:
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC sd;
		BaseWindow* window = nullptr;
		ComPtr<IDXGISwapChain> swapChain;
	};
}


