#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

namespace NuimVulkan {

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {

		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

		return VK_FALSE;
	}

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
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks& pAllocationCallbacks, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT();
		std::vector<STRING> getRequiredExtensions();
	private:
		GLFWwindow* window;
		VkInstance vkInstance;
		VkDebugUtilsMessengerEXT debugMessenger;
	};

	extern Application* CreateApplication();
}
