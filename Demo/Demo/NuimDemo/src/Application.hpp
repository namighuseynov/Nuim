#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"

namespace NuimDemo {
	class Application
	{
	public:
		Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
	public:
		void Run();
		void OnEvent(EventSystem::Event& e);
	private:
		std::unique_ptr<Window> m_window;
		HINSTANCE hInstance;
		int nCmdShow;
	};
}