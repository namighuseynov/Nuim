#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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
		void initWindow();
		void initVulkan();
		void createInstance();
		void setupDebugMessenger();
	private:
		bool checkValidationLayerSupport();
		VkResult CreateDebugUtilsMessengerEXT();
		void DestroyDebugUtilsMessengerEXT();
		std::vector<STRING> getRequiredExtensions();
	private:
		GLFWwindow* window;
		VkInstance vkInstance;
		VkDebugUtilsMessengerEXT debugMessenger;
	};

	extern Application* CreateApplication();
}
