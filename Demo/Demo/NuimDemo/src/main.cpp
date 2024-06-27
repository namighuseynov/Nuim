#include "NuimDemoPCH.h"
#include "EventSystem.hpp"

#ifdef WINDOWED
#define WINDOWED_APPLICATION

#include "EntryPoint.h"


#else
#define CONSOLE_APPLICATION
int main() {
	NuimDemo::NuimApplication* app = new NuimDemo::NuimApplication();
	app->Run();
	delete app;
	std::cout << "Hello, world" << std::endl;
}
#endif