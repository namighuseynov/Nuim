#include "V_Nuim.h"


class NuimEngine : public NuimVulkan::Application {
	
};

NuimVulkan::Application* NuimVulkan::CreateApplication() {
	return new NuimEngine();
}
