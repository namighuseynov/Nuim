#pragma once
#include "Core.h"

namespace NM_VK_INIT {

	VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	) {
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

	vk::DebugUtilsMessengerEXT CreateDebugMessenger(const vk::Instance& instance, const bool& debugMode, vk::DispatchLoaderDynamic& dldi) {
		if (!debugMode) return nullptr;
		dldi = vk::DispatchLoaderDynamic(instance, vkGetInstanceProcAddr);
		vk::DebugUtilsMessengerCreateInfoEXT createInfo = vk::DebugUtilsMessengerCreateInfoEXT(vk::DebugUtilsMessengerCreateFlagsEXT(),
			vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo,
			vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
			debugCallback,
			nullptr
		);

		return instance.createDebugUtilsMessengerEXT(createInfo, nullptr, dldi);
	}
}