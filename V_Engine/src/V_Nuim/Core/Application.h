#pragma once
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <string>

namespace NuimVulkan {

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
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
	private:
		void createInstance();
		void setupDebugMessenger();
	private:
		bool checkValidationLayerSupport();
		VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
		std::vector<STRING> getRequiredExtensions();
		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	private:
		GLFWwindow* window;
		VkInstance vkInstance;
		VkDebugUtilsMessengerEXT debugMessenger;
	};

	extern Application* CreateApplication();
}
