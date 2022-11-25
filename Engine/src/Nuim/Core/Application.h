#pragma once
#include "Nuim/Core/Core.h"
#include "Nuim/Events/ApplicationEvent.h"

#include <sstream>

namespace Nuim {
	struct ApplicationCommandLineArgs {
		int Count = 0;
		char** Args = nullptr;

		const char* operator[](int index) const {
			return Args[index];
		}
	};

	struct ApplicationSpecification {
		std::string Name;
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class Application {
	public:
		Application(const ApplicationSpecification& specification);
		virtual ~Application();
	public:
		int Run();
		void Close();
		static Application& GetApplication() { return *instance; }
	private:
		bool OnWindowClose();
		bool OnWindowResize();
	private:
		bool running = true;
		bool minimized = false;
		static Application* instance;
		ApplicationSpecification specification;

	};
	Application* CreateApplication(ApplicationCommandLineArgs args);
}