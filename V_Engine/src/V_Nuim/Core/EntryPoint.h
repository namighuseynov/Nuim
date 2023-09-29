#pragma once

#ifdef NUIM_PLATFORM_WINDOWS
int main() {
	auto app = NuimVulkan::CreateApplication();
	app->Run();
}
#endif // NUIM_PLATFORM_WINDOWS
