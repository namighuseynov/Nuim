#pragma once
#include "../Core/Core.h"
#include "../Platform/glad/glad.h"
#include <GLFW/glfw3.h>
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