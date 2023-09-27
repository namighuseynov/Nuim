#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace NuimVulkan {
	class Application {
	public:
		Application();
		~Application();
	public:
		void Run();
		void InitVulkan();
	private:
		GLFWwindow* window;
		VkInstance vkInstance;
	};

	extern Application* CreateApplication();
}
