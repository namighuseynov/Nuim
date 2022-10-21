#include "Engine.h"

class NuimEngine :
	public Nuim::Application {
public:
	NuimEngine()
	{
		;
	}
	~NuimEngine()
	{
		;
	}
};

Nuim::Application* Nuim::CreateApplication() {
	return new NuimEngine();
}