#pragma once
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

namespace NuimDemo {
	class D3DApp
	{
	public:
		D3DApp(BaseWindow* window) : window(window) {}
	public:
		bool CreateDeviceD3D();
		void CleanupDeviceD3D();
		void CreateRenderTarget();
		void CleanupRenderTarget();
	public:
		ComPtr<ID3D11Device> d3d_device;
		ComPtr<ID3D11DeviceContext> d3d_deviceContext;
		ComPtr<IDXGISwapChain> dxgi_swapChain;
		ComPtr<ID3D11RenderTargetView> d3d_backBuffer;
		BaseWindow* window;
	};
}