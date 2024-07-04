#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "Window.hpp"


using Microsoft::WRL::ComPtr;

namespace NuimDemo {
	class D3DApp
	{
	public:
		D3DApp(std::shared_ptr<Window> pWindow) {
			windowInstance = pWindow;
		};
	public:
		bool CreateDevice();
		void CreateRenderTarget();
		void CleanupRenderTarget();
		ComPtr<ID3D11Device> GetDevice() { return d3d_device; }
		ComPtr<ID3D11DeviceContext> GetContext() { return d3d_context; }
		ComPtr<IDXGISwapChain> SwapChain() { return dxgi_swapChain; }
		ComPtr<ID3D11RenderTargetView> GetRenderTarget() { return d3d_renderTarget; }
	private:
		ComPtr<ID3D11Device> d3d_device;
		ComPtr<ID3D11DeviceContext> d3d_context;
		ComPtr<IDXGISwapChain> dxgi_swapChain;
		ComPtr<ID3D11RenderTargetView> d3d_renderTarget;
		std::shared_ptr<Window> windowInstance;
	};
}