#include "Core.h"
#include "Application.h"
#include "Instance.h"

namespace NuimVulkan {

	const std::vector<STRING> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	Application::Application() : window(nullptr), vkInstance(nullptr) {
#ifdef NUIM_DEBUG
		this->debug_mode = NM_TRUE;
#else 
		this->debug_mode = NM_FALSE;
#endif // NUIM_DEBUG

		Application::initWindow();
		Application::initVulkan();
		std::cout << "Debug mode is: " << this->debug_mode << std::endl;
	}

	Application::~Application() {
		if (this->debug_mode) {
			DestroyDebugUtilsMessengerEXT(vkInstance, debugMessenger, nullptr);
		}
		vkInstance.destroy();
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

		if (this->debug_mode && !checkValidationLayerSupport()) {
			throw std::runtime_error("validation layers requested, but not available!");
		}
		U32 version{ 0 };
		vk::Result result = vk::enumerateInstanceVersion(&version);
		if (result != vk::Result::eSuccess) {
			throw std::runtime_error("Failed to check Vulkan version");
		}
		//version &= ~(0xFFFU);
		version = VK_MAKE_API_VERSION(0, 1, 0, 0);
		vk::ApplicationInfo applicationInfo = vk::ApplicationInfo(
			this->applicationName,
			version,
			"Nuim",
			version,
			version
		);

		auto extensions = getRequiredExtensions();
		U32 layerCount{ 0 };

		NM_VK_INIT::Supported(extensions, debug_mode);

		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&applicationInfo,
			0, nullptr, static_cast<U32>(extensions.size()), extensions.data()
		);
		try {
			this->vkInstance = vk::createInstance(createInfo);
		}
		catch (vk::SystemError err) {
			if (debug_mode) {
				std::cout << err.what() << std::endl;
			}
		}
	}

	void Application::setupDebugMessenger() {
		
		if (!this->debug_mode) {
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
		if (this->debug_mode) {
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
					std::cout << layerName << "Is supported" << std::endl;
					break;
				}
			}
			if (!layerFound) {
				std::cout << layerName << "Is not supported" << std::endl;
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
	}
}