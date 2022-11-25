#include "Nuim.h"

class NuimEngine : public Nuim::Application {
public:
	NuimEngine(const Nuim::ApplicationSpecification& specification) : Nuim::Application(specification) {
		
	}
};

Nuim::Application* Nuim::CreateApplication(Nuim::ApplicationCommandLineArgs args) {
	Nuim::ApplicationSpecification spec;
	spec.Name = "Nuim";
	spec.WorkingDirectory = "../Nuim";
	spec.CommandLineArgs = args;
	return new NuimEngine(spec);
}