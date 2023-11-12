#include "GL_Renderer.hpp"
#include "Shader.hpp"

namespace Nuim {

    GL_Renderer::GL_Renderer(LinuxWindow* window) {
        this->window = window;

        

    }

    void GL_Renderer::Begin() {
        U32 VAO, VBO;
        U32 vertex_shader, fragment_shader, shader_program;
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        Shader shader("../assets/shaders/n_vertex.glsl", "../assets/shaders/n_fragment.glsl");

        while (!glfwWindowShouldClose(this->window->Instance))
        {
            // input
            // -----
            processInput(this->window->Instance);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Use();

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(this->window->Instance);
            glfwPollEvents();
        }
    }

    void GL_Renderer::End() {

    }
}