#include "Application.hpp"

namespace Nuim {

#if defined(NUIM_PLATFORM_LINUX)
    Application::Application() {
        this->window = new LinuxWindow();
        this->renderer = new GL_Renderer(this->window);
    }
#elif defined(NUIM_PLATFORM_WINDOWS) 
    Application::Application(HINSTANCE hInstance, I32 nCmdShow) {
        this->window = new WindowsWindow(u"Nuim", 1000, 800, hInstance, nCmdShow);
        this->renderer = new DX_Renderer();
    }
#endif
    Application::~Application() {
        this->renderer->End();
        delete this->window;
    }

    void Application::Run() {
        this->renderer->Begin();
    }
}