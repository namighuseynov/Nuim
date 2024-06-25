#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"

#include "BaseWindow.h"
#include "D3DRenderer.hpp"

namespace NuimDemo {
	class Application
	{
	public:
		Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
		~Application();
	public:
		void Run();
	private:
		bool CreateAppWindow();
	private:
		HINSTANCE hInstance;
		int nCmdShow;
		BaseWindow* baseWindow = nullptr;
		D3DRenderer* d3d_renderer = nullptr;
	};
}