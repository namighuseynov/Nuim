#include "Engine.h"

class NuimEngine :
	public Nuim::Application {
};

Nuim::Application* Nuim::CreateApplication() {
	return new NuimEngine();
}