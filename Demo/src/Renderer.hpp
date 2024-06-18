#pragma once
#include <d3d11.h>

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
		ID3D11Device* device;
		ID3D11DeviceContext* deviceContext;
		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC sd;
		BaseWindow* window = nullptr;
	};
}


