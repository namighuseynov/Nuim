#pragma once
#include "Core.h"

namespace NM_VK_INIT {
	bool Supported(std::vector<STRING> extensions, std::vector<STRING> layers, bool debugMode);
	std::vector<STRING> GetRequiredExtensions(bool debugMode);
	vk::Instance CreateInstance(bool debugMode, STRING applicationName);

	vk::Instance CreateInstance(bool debugMode, STRING applicationName) {
		auto extensions = GetRequiredExtensions(debugMode);
		const char* validationLayerName = "VK_LAYER_KHRONOS_validation";
		std::vector<STRING> layerss = { validationLayerName };
		if (debugMode && !Supported(extensions, layerss, debugMode)) {
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
			applicationName,
			version,
			"Nuim",
			version,
			version
		);
		vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo(
			vk::InstanceCreateFlags(),
			&applicationInfo,
			1, &validationLayerName, static_cast<U32>(extensions.size()), extensions.data()
		);
		try {
			return vk::createInstance(createInfo);
		}
		catch (vk::SystemError err) {
			if (debugMode) {
				std::cout << err.what() << std::endl;
			}
			return nullptr;
		}
	}

	std::vector<STRING> GetRequiredExtensions(bool debugMode)
	{
		U32 glfwExtensionCount = 0;
		STRING* glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<STRING> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (debugMode) {
			extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}
		return extensions;
	}


	bool Supported(std::vector<STRING> extensions, std::vector<STRING> layers, bool debugMode) {
		U32 extensionCount{ 0 };
		std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
		if (debugMode) {
			std::cout << "Device can support following extensions: " << std::endl;
			for (auto extension : extensionProperties) {
				std::cout << extension.extensionName << std::endl;
			}
		}
		std::cout << std::endl;
		bool found{ 0 };
		for (auto extension : extensions) {
			found = NM_FALSE;
			for (auto extensionProperty : extensionProperties) {
				if (strcmp(extension, extensionProperty.extensionName) == 0) {
					found = true;
					if (debugMode) {
						std::cout << "Extension " << extension << " supported" << std::endl;
					}
				}
			}
			if (!found) {
				if (debugMode) {
					std::cout << "Extension " << extension << " is not supported" << std::endl;
				}
				return NM_FALSE;
			}
		}

		std::vector<vk::LayerProperties> layersProperties = vk::enumerateInstanceLayerProperties();
		if (debugMode) {
			std::cout << "Device can support following layers " << std::endl;
			for (auto layer : layersProperties) {
				std::cout << "Layer " << layer.layerName << std::endl;
			}
		}
		std::cout << std::endl;
		found = NM_FALSE;
		for (auto layer : layers) {
			found = NM_FALSE;
			for (auto layerProperty : layersProperties) {
				if (strcmp(layer, layerProperty.layerName) == 0) {
					found = NM_TRUE;
					if (debugMode) {
						std::cout << "Layer " << layer << " Is supported " << std::endl;
					}
				}
			}
			if (!found) {
				if (debugMode) {
					std::cout << "Layer " << layer << " Is not supported " << std::endl;
				}
				return NM_FALSE;
			}
		}
		return NM_TRUE;
	}
}