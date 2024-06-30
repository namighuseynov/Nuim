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
		
		if (e.GetType() == EventSystem::EventType::MouseMoveEvent) {
			EventSystem::MouseMoveEvent* mouseMoveEvent = dynamic_cast<EventSystem::MouseMoveEvent*>(&e);
			std::cout << std::to_string(mouseMoveEvent->GetX()) + "," + std::to_string(mouseMoveEvent->GetY()) << std::endl;
		}
		else if (e.GetType() == EventSystem::EventType::MousePressEvent) {
			EventSystem::MousePressEvent* mousePressEvent = dynamic_cast<EventSystem::MousePressEvent*>(&e);
			std::cout << std::to_string(mousePressEvent->GetX()) + "," + std::to_string(mousePressEvent->GetY()) << std::endl;
		}
		else {
			std::cout << e.GetName() << std::endl;
		}
	}
}