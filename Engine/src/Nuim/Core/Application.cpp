#include "nmpch.h"

#include "Application.h"
#include "Nuim/Renderer/Renderer.h"

namespace Nuim {
	Application* Application::instance = nullptr;

	Application::Application(const ApplicationSpecification& specification) : specification(specification) {
		instance = this;
		this->specification = specification;
		
		Renderer::Init();
	}
	Application::~Application() {
		Renderer::ShutDown();
	}
	int Application::Run() {
		while (running) {

		}
		return 0;
	}
	void Application::Close() {
		running = false;
	}
	bool Application::OnWindowClose()
	{
		return false;
	}
	bool Application::OnWindowResize()
	{
		return false;
	}
}