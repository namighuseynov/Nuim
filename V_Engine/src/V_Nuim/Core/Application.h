#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace NuimVulkan {
	class Application {
	public:
		Application();
		~Application();
	public:
		void Run();
	private:
		void InitWindow();
		void InitVulkan();
	private:
		STRING applicationName = "Nuim Engine";
		U16 debug_mode{ NM_TRUE };
		GLFWwindow* window{ nullptr };
		vk::Instance vkInstance{ nullptr };
		vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
		vk::DispatchLoaderDynamic dldi;
	};
	extern Application* CreateApplication();
}
