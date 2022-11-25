#include "nmpch.h"
#include "Application.h"

namespace Nuim {
	Application::Application(const ApplicationSpecification& specification) : specification(specification) {

	}
	Application::~Application() {

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