#ifndef ENTRY_POINT_H

#ifdef NUIM_PLATFORM_WINDOWS

#ifdef NUIM_SUBSYSTEM_WINDOWS
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {

	Nuim::Application* app = new Nuim::Application(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	app->Run();
	delete app;
	return NUIM_SUCCESS;
}
#elif NUIM_SUBSYSTEM_CONSOLE

int main() {
	Nuim::Application* app = new Nuim::Application();
	app->Run();
	delete app;
	return NUIM_SUCCESS;
}

#endif

#elif defined(NUIM_PLATFORM_LINUX)

#define lINUX

int main() {
    return NUIM_SUCCESS;
}

#endif

#define ENTRY_POINT_H
#endif