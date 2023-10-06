#pragma once

namespace NM_VK_INIT {
	void ShowPhysicalDevices(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi) {
		std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices(dldi);
		for (auto device : devices) {
			vk::PhysicalDeviceProperties deviceProperty = device.getProperties();
			std::cout << deviceProperty.deviceName << std::endl;
			std::cout << deviceProperty.driverVersion << std::endl;
		}
	}

	void GetPhysicalDevice(const vk::Instance& instance, const vk::DispatchLoaderDynamic& dldi) {
		std::vector<vk::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices(dldi);
		if (physicalDevices.size() == 0) {
			throw std::runtime_error("failed to find GPUs with Vulkan support!");
		}

		vk::PhysicalDevice physicalDevice = nullptr;
		for (const auto device : physicalDevices) {
			
		}
	}

}