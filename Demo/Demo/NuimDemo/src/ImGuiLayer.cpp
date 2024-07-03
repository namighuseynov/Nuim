#include "NuimDemoPCH.h"
#include "ImGuiLayer.hpp"

namespace NuimDemo {
	void ImGuiLayer::Begin(Application* pApp)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(pApp->GetWndInstance()->GetHWND());
		ImGui_ImplDX11_Init(pApp->GetD3DApp()->GetDevice().Get(), pApp->GetD3DApp()->GetContext().Get());
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

		// Rendering
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	}
	void ImGuiLayer::OnEvent(EventSystem::Event& e)
	{
	}
}