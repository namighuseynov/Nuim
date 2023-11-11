#pragma once
#include "Renderer.hpp"
#include "../Platform/LinuxWindow.hpp"

namespace Nuim {
    class GL_Renderer :
        public Renderer {
    public:
        GL_Renderer(LinuxWindow* window);
    public:
        void Begin();
        void End();
    private:
        LinuxWindow* window;
    };
}