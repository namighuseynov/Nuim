#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "Layer.hpp"
#include "EventSystem.hpp"
#include "Application.hpp"

namespace NuimDemo {
	class ImGuiLayer
	{
	public:
		void Begin(Application* pApp);
		void End();
		void OnUpdate();
		void OnEvent(EventSystem::Event& e);
	};
}