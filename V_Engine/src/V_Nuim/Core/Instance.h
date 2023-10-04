#pragma once
#include "Core.h"

namespace NM_VK_INIT {
	bool Supported(std::vector<STRING> extensions, bool debugMode);


	bool Supported(std::vector<STRING> extensions, bool debugMode) {
		U32 extensionCount{ 0 };
		std::vector<vk::ExtensionProperties> extensionProperties = vk::enumerateInstanceExtensionProperties();
		if (debugMode) {
			std::cout << "Device can support following extensions: " << std::endl;
			for (auto extension : extensionProperties) {
				std::cout << extension.extensionName << std::endl;
			}
		}
		std::cout << std::endl;
		bool finded{ 0 };
		for (auto extension : extensions) {
			finded = NM_FALSE;
			for (auto extensionProperty : extensionProperties) {
				if (strcmp(extension, extensionProperty.extensionName) == 0) {
					finded = true;
					if (debugMode) {
						std::cout << "Extension " << extension << " supported" << std::endl;
					}
				}
			}
			if (!finded) {
				if (debugMode) {
					std::cout << "Extension " << extension << " is not supported" << std::endl;
				}
				return NM_FALSE;
			}
		}
		return NM_TRUE;
	}
}