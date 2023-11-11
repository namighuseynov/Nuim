#pragma once
#include "Window.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"



namespace Nuim {
    void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    void processInput(GLFWwindow *window);

    class LinuxWindow :
        public Window {
    public:
        LinuxWindow();
        ~LinuxWindow();
    public:
        GLFWwindow* Instance = 0;
    };
}