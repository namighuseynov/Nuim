#pragma once 
#include "Core.h"

#ifdef NUIM_PLATFORM_WINDOWS
#include "../Platform/WindowsWindow.hpp"
#include "../Renderer/DX_Renderer.hpp"
#elif defined(NUIM_PLATFORM_LINUX)
#include "../Platform/LinuxWindow.hpp"
#include "../Renderer/GL_Renderer.hpp"
#endif

namespace Nuim {
    class Application {
    public:
        Application();
        ~Application();
    public:
        void Run();
    private:
#ifdef NUIM_PLATFORM_WINDOWS
        WindowsWindow* window;
        DX_Renderer* renderer;
#elif defined(NUIM_PLATFORM_LINUX)
        LinuxWindow* window;
        GL_Renderer* renderer;
#endif
    };
}