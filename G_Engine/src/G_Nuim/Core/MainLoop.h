#pragma once 

#include "G_Nuim/Core/Core.h"
#include "G_Nuim/Core/Application.h"

#ifdef NUIM_PLATFORM_WINDOWS


int main(int argc, char** argv) {
	auto app = Nuim::CreateApplication({ argc, argv });
	app->Run();
	delete app;
	return 0;
}
#endif // NUIM_PLATFORM_WINDOWS
