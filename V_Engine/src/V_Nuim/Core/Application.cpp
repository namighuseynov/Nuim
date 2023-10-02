#include "Core.h"
#include "Application.h"


namespace NuimVulkan {
	const U32 WIDTH = 800;
	const U32 HEIGHT = 600;

	const std::vector<STRING> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

#ifndef NDEBUG
	const bool enableValidationLayers = NM_FALSE;
#else
	const bool enableValidationLayers = NM_TRUE;
#endif

	Application::Application() : window(nullptr), vkInstance(nullptr) {
		Application::initWindow();
		Application::initVulkan();

		U32 extensionCount = 0;
		STRING* glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);
		std::vector<STRING> extensions(glfwExtensions, glfwExtensions + extensionCount);
		std::cout << "Extensions:" << std::endl;
		for (auto extension : extensions) {
			std::cout << extension << std::endl;
		}
		U32 supportedExtensionCount = 0;
		VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, nullptr);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to enumerate supported extensions");
		}
		std::vector<VkExtensionProperties> supportedExtensions(supportedExtensionCount);
		result = vkEnumerateInstanceExtensionProperties(nullptr, &supportedExtensionCount, supportedExtensions.data());
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to enumerate supported extensions");
		}
		std::cout << "Supported Extensions:" << std::endl;
		for (auto item : supportedExtensions) {
			std::cout << item.extensionName << std::endl;
		}
		U32 supportedLayersCount = 0;
		result = vkEnumerateInstanceLayerProperties(&supportedLayersCount, nullptr);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to enumerate supported layers");
		}
		std::vector<VkLayerProperties> supportedLayers(supportedLayersCount);
		result = vkEnumerateInstanceLayerProperties(&supportedLayersCount, supportedLayers.data());
		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to enumerate supported layers");
		}
		std::cout << "Supported Layers:" << std::endl;
		for (VkLayerProperties layer : supportedLayers) {
			std::cout << layer.layerName << std::endl;
		}
	}

	Application::~Application() {
		if (enableValidationLayers) {
			DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
		}
		vkDestroyInstance(vkInstance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Application::initWindow() {
		if (!glfwInit()) return;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(1000, 800, "Nuim", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
	}

	void Application::initVulkan()
	{
		Application::createInstance();
		Application::setupDebugMessenger();
	}

	void Application::createInstance() {

		if (enableValidationLayers && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}

		VkApplicationInfo applicationInfo{};
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pApplicationName = "Nuim Engine";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Nuim";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &applicationInfo;

		if (enableValidationLayers) {
			createInfo.enabledLayerCount = static_cast<U32>(validationLayers.size());
			createInfo.ppEnabledLayerNames = validationLayers.data();
		}
		else {
			createInfo.enabledLayerCount = 0;
		}
		auto extensions = getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<U32>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create vulkan instance");
		}
	}

	void Application::setupDebugMessenger() {
		
		if (!enableValidationLayers) {
			return;
		}
		VkDebugUtilsMessengerCreateInfoEXT createInfo{};
		populateDebugMessengerCreateInfo(createInfo);

		if (CreateDebugUtilsMessengerEXT(vkInstance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
			throw std::runtime_error("failed to set up debug messenger!");
		}
		
	}

	void Application::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
		createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = debugCallback;
	}

	VkResult Application::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr) {
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else {
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
		
	}

	void Application::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr) {
			func(instance, debugMessenger, pAllocator);
		}
	}

	std::vector<STRING> Application::getRequiredExtensions()
	{
		U32 glfwExtensionCount = 0;
		STRING* glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<STRING> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return extensions;
	}

	bool Application::checkValidationLayerSupport() {
		U32 layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : validationLayers) {
			bool layerFound = NM_FALSE;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = NM_TRUE;
					break;
				}
			}
			if (!layerFound) {
				return NM_FALSE;
			}
		}
		return NM_TRUE;
	}



	void Application::Run() {
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		std::system("pause");
	}
	
	
	
}