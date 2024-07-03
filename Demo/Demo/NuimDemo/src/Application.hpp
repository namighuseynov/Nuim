#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "D3DApp.hpp"
#include "LayerStack.hpp"

namespace NuimDemo {
	class Application
	{
	public:
		Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
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
		std::unique_ptr<D3DApp> d3d_app;
		std::unique_ptr<LayerStack> layerStack;
		HINSTANCE hInstance;
		int nCmdShow;
	};
}