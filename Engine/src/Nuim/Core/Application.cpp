#include "nmpch.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Application.h"
#include "Nuim/Renderer/Renderer.h"

#include "Nuim/Imgui/imgui.h"
#include "Nuim/Imgui/imgui_impl_glfw.h"
#include "Nuim/Imgui/imgui_impl_opengl3.h"



namespace Nuim {
	Application* Application::instance = nullptr;

	Application::Application(const ApplicationSpecification& specification) : specification(specification) {
		instance = this;
		this->specification = specification;
		
		Renderer::Init();
	}
	Application::~Application() {
		Renderer::ShutDown();
	}
	int Application::Run() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		glViewport(0, 0, 800, 600);


		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
	void Application::Close() {
		running = false;
	}
	bool Application::OnWindowClose()
	{
		return false;
	}
	bool Application::OnWindowResize()
	{
		return false;
	}
}