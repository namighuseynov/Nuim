#pragma once
#include "G_Nuim/Core/Core.h"

#include "G_Nuim/Imgui/imgui.h"
#include "G_Nuim/Imgui/imgui_impl_glfw.h"
#include "G_Nuim/Imgui/imgui_impl_opengl3.h"


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
		GLFWwindow* windowInstance;
	};
	Application* CreateApplication(ApplicationCommandLineArgs args);
}