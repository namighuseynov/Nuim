#include "NuimDemoPCH.h"
#include "Application.hpp"

namespace NuimDemo {

	Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
		this->hInstance = hInstance;
		this->nCmdShow = nCmdShow;
		this->m_window = std::unique_ptr<Window>(new Window(hInstance));
		this->m_window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

	};
	void Application::Run() {
		this->m_window->OnUpdate();
	}
	void Application::OnEvent(EventSystem::Event& e) {
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		std::cout << e.GetName() << std::endl;
	}
}