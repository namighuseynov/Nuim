#include "NuimDemoPCH.h"
#include "Application.hpp"

namespace NuimDemo {

	Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
		this->hInstance = hInstance;
		this->nCmdShow = nCmdShow;
		this->m_window = std::unique_ptr<Window>(Window::Create(hInstance));

	};
	void Application::Run() {
		this->m_window->OnUpdate();
	}
	void Application::OnEvent(EventSystem::Event& e)
	{
	}
	;
}