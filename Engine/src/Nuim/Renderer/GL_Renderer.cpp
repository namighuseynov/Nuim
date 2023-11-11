#include "GL_Renderer.hpp"

namespace Nuim {

    GL_Renderer::GL_Renderer(LinuxWindow* window) {
        this->window = window;
    }

    void GL_Renderer::Begin() {
        while (!glfwWindowShouldClose(this->window->Instance))
        {
            // input
            // -----
            processInput(this->window->Instance);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(this->window->Instance);
            glfwPollEvents();
        }
    }

    void GL_Renderer::End() {

    }


}