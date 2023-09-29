#pragma once

#ifdef NUIM_PLATFORM_WINDOWS
#include <iostream>
int main() {
	auto app = NuimVulkan::CreateApplication();
	try {
		app->Run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
#endif // NUIM_PLATFORM_WINDOWS
