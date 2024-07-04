#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "D3DApp.hpp"
#include "ImGuiLayer.hpp"

namespace NuimDemo {
	class Application
	{
	public:
		Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
	public:
		void Run();
		void OnEvent(EventSystem::Event& e);
		D3DApp* GetD3DApp() {
			return d3d_app.get();
		}
		Window* GetWndInstance() {
			return this->m_window.get();
		}
	private:
		std::shared_ptr<Window> m_window;
		std::shared_ptr<D3DApp> d3d_app;

		std::vector<ImGuiLayer*> layers;
		HINSTANCE hInstance;
		int nCmdShow;
	};
}