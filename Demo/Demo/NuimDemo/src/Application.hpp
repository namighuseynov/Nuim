#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "D3DApp.hpp"

namespace NuimDemo {
	class Application
	{
	public:
		Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
	public:
		void Run();
		void OnEvent(EventSystem::Event& e);
	private:
		std::shared_ptr<Window> m_window;
		std::unique_ptr<D3DApp> d3d_app;
		HINSTANCE hInstance;
		int nCmdShow;
	};
}