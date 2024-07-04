#include "NuimDemoPCH.h"
#include "ImGuiLayer.hpp"

namespace NuimDemo {
	void ImGuiLayer::Begin(D3DApp* pApp)
	{
		d3d_app = pApp;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(d3d_app->GetDevice().Get());
		ImGui_ImplDX11_Init(d3d_app->GetDevice().Get(), d3d_app->GetContext().Get());
	}
	void ImGuiLayer::End()
	{
		// Cleanup
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
	void ImGuiLayer::OnUpdate()
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame(); 
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();
		ImGui::Begin("Show");
		ImGui::End();

		// Rendering
		ImGui::Render();
		const float clear_color_with_alpha[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
		d3d_app->GetContext().Get()->OMSetRenderTargets(1, &d3d_app->GetRenderTarget(), nullptr);
		d3d_app->GetContext().Get()->ClearRenderTargetView(d3d_app->GetRenderTarget().Get(), clear_color_with_alpha);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		HRESULT hr = d3d_app->SwapChain().Get()->Present(1, 0);
		std::cout << "Update" << std::endl;

	}
	void ImGuiLayer::OnEvent(EventSystem::Event& e)
	{
		std::cout << e.GetName() << std::endl;
	}
}