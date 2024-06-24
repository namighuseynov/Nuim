#include "NuimDemoPCH.h"
#include "BaseWindow.h"
#include "Renderer.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"


namespace NuimDemo {

	Renderer::Renderer(BaseWindow* window) {
		this->window = window;
		Initialize();
		
	}

	void Renderer::ShutDown() {
		if (this->swapChain.Get() == nullptr) this->swapChain->Release();
		if (this->backBuffer == nullptr) this->backBuffer->Release();		
		if (this->deviceContext.Get() == nullptr) this->deviceContext->Release();
		if (this->device.Get() == nullptr) this->device->Release();

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void Renderer::RenderFrame()
	{

		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();

		// Rendering
		ImGui::Render();
		float clearColor[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		this->deviceContext->ClearRenderTargetView(this->backBuffer, clearColor);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		this->swapChain->Present(0, 0);
	}

	bool Renderer::Initialize() {
		//Swap chain desc
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

		this->device->CreateRenderTargetView(pBackBuffer, nullptr, &this->backBuffer);
		pBackBuffer->Release();

		this->deviceContext->OMSetRenderTargets(1, &this->backBuffer, nullptr);

		ZeroMemory(&this->viewport, sizeof(D3D11_VIEWPORT));
		this->viewport.TopLeftX = 0;
		this->viewport.TopLeftY = 0;
		this->viewport.Width = this->window->Width;
		this->viewport.Height = this->window->Height;	

		this->deviceContext->RSSetViewports(1, &this->viewport);

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(this->window->GetHWND());
		ImGui_ImplDX11_Init(this->device.Get(), this->deviceContext.Get());
		return true;
	}
}
