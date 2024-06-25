#include "NuimDemoPCH.h"
#include "D3DRenderer.hpp"

namespace NuimDemo {
	D3DRenderer::D3DRenderer(BaseWindow* baseWindow) {
		d3dApp = nullptr;
		window = baseWindow;
	}
	void D3DRenderer::Begin() {
		d3dApp = new D3DApp(this->window);
		if (!d3dApp->CreateDeviceD3D()) {
			MessageBox(nullptr, L"Error Creating d3d device", L"Error", 0);
		}
		d3dApp->CreateRenderTarget();

	}

	void D3DRenderer::Render() {
		d3dApp->CleanupRenderTarget();
		d3dApp->CreateRenderTarget();
		// Poll and handle messages (inputs, window resize, etc.)
		

		// Rendering
		float clearColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		//this->d3dApp->d3d_deviceContext->OMSetRenderTargets(1, &this->d3dApp->d3d_backBuffer, nullptr);
		this->d3dApp->d3d_deviceContext->ClearRenderTargetView(this->d3dApp->d3d_backBuffer.Get(), clearColor);

		// Present
		HRESULT hr = this->d3dApp->dxgi_swapChain->Present(1, 0);
		
	}

	void D3DRenderer::ShutDown()
	{
		/*d3dApp->CleanupDeviceD3D();
		delete d3dApp;*/
	}
}