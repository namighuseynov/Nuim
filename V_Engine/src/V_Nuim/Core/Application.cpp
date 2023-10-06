#include "Core.h"
#include "Application.h"
#include "Instance.h"
#include "DebugMessenger.h"

namespace NuimVulkan {

	Application::Application() {
#ifdef NUIM_DEBUG
		this->debug_mode = NM_TRUE;
#else 
		this->debug_mode = NM_FALSE;
#endif // NUIM_DEBUG

		Application::InitWindow();
		Application::InitVulkan();
	}

	Application::~Application() {
		if (this->debug_mode) {
			//vkInstance.destroyDebugUtilsMessengerEXT(this->debugMessenger,nullptr,this->dldi);
		}
		vkInstance.destroy();
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void Application::InitWindow() {
		if (!glfwInit()) return;
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		window = glfwCreateWindow(1000, 800, "Nuim", NULL, NULL);

		if (!window)
		{
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
	}

	void Application::InitVulkan()
	{
		this->vkInstance = NM_VK_INIT::CreateInstance(this->debug_mode, "Nuim Engine");
		this->debugMessenger = NM_VK_INIT::CreateDebugMessenger(this->vkInstance, this->dldi);
	}

	void Application::Run() {
		while (!glfwWindowShouldClose(window)) {
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		std::system("pause");
	}
}