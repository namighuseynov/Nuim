#include "nmpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Application.h"
#include "Nuim/Renderer/Renderer.h"


namespace Nuim {
	Application* Application::instance = nullptr;

	Application::Application(const ApplicationSpecification& specification) : specification(specification) {
		instance = this;
		this->specification = specification;
		

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		this->windowInstance = glfwCreateWindow(1280, 720, "Nuim", NULL, NULL);
		if (this->windowInstance == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(this->windowInstance);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
		glViewport(0, 0, 800, 600);

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(this->windowInstance, true);
		ImGui_ImplOpenGL3_Init("#version 330");

	}
	Application::~Application() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		glfwTerminate();
	}
	int Application::Run() {
		
		while (!glfwWindowShouldClose(this->windowInstance))
		{
			glClear(GL_COLOR_BUFFER_BIT);


			float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f
			};
			unsigned int VBO;
			glGenBuffers(1, &VBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	

			const char* vertexShaderSource = "#version 330 core\n"
				"layout (location = 0) in vec3 aPos;\n"
				"void main()\n"
				"{\n"
				"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
				"}\0";

			const char* fragmentShaderSource = "#version 330 core\n"
				"out vec4 FragColor;\n"
				"void main()\n"
				"{\n"
				"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
				"}\n";

			unsigned int vertexShader;
			vertexShader = glCreateShader(GL_VERTEX_SHADER);

			glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
			glCompileShader(vertexShader);
			int  success;
			char infoLog[512];
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			}

			unsigned int fragmentShader;
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
			glCompileShader(fragmentShader);

			unsigned int shaderProgram;
			shaderProgram = glCreateProgram();

			glAttachShader(shaderProgram, vertexShader);
			glAttachShader(shaderProgram, fragmentShader);
			glLinkProgram(shaderProgram);

			glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			}

			glUseProgram(shaderProgram);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();


			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
					if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
					if (ImGui::MenuItem("Close", "Ctrl+W")) { /* Do stuff */ }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}


			ImGui::Begin("Debug"); {
				if (ImGui::IsWindowHovered()) {
					ImGui::Text("Some messages");
					
				}
				ImGui::End();
				
			}
			
			ImGui::Begin("Scene"); {
				ImGui::Text("Scene");
				ImGui::End();
			}






			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


			glfwSwapBuffers(this->windowInstance);
			glfwPollEvents();
		}

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