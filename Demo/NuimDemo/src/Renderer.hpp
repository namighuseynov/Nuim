#pragma once
#include <d3d10.h>

using Microsoft::WRL::ComPtr;

namespace NuimDemo {

	class Renderer
	{
	public:
		Renderer(BaseWindow* window);
	public:
		bool Initialize();
		void ShutDown();
		void RenderFrame();
	private:
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> deviceContext;
		D3D_FEATURE_LEVEL featureLevel;
		DXGI_SWAP_CHAIN_DESC sd = {};
		BaseWindow* window = nullptr;
		ComPtr<IDXGISwapChain> swapChain;
		ID3D11RenderTargetView* backBuffer;
		D3D11_VIEWPORT viewport;
	};
}


