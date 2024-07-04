#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "Layer.hpp"
#include "EventSystem.hpp"
#include "D3DApp.hpp"

namespace NuimDemo {
	class ImGuiLayer
	{
	public:
		ImGuiLayer() : d3d_app(nullptr) {};
	public:
		void Begin(D3DApp* pApp);
		void End();
		void OnUpdate();
		void OnEvent(EventSystem::Event& e);
	private:
		D3DApp* d3d_app;
	};
}