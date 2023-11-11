#include "Application.hpp"

namespace Nuim {

    Application::Application() {
#ifdef NUIM_PLATFORM_WINDOWS
        this->window = new WindowsWindow();
        this->renderer = new DX_Renderer();
#elif defined(NUIM_PLATFORM_LINUX)
        this->window = new LinuxWindow();
        this->renderer = new GL_Renderer(this->window);
#endif
    }

    Application::~Application() {
        this->renderer->End();
        delete this->window;
    }


    void Application::Run() {
        this->renderer->Begin();
    }
}