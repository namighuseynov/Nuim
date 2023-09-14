#include "nmpch.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "Application.h"
#include "Shader.h"


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
		int success;
		char infoLog[512];
		float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f
		};

		unsigned int VBO, VAO, vertexShader, fragmentShader, shaderProgram;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);

		const char* VERTEX_SHADER_SOURCE = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"void main() {\n"
			"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"}";

		const char* FRAGMENT_SHADER_SOURCE = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main() {\n"
			"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);"
			"}";

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << infoLog << std::endl;
		}

		std::cout << Nuim::LoadShader("Render/nvs.vert") << std::endl;
		

		while (!glfwWindowShouldClose(this->windowInstance))
		{
			glClear(GL_COLOR_BUFFER_BIT);


			//GL CODE
			glBindVertexArray(VAO);
			glBindBuffer(GL_VERTEX_ARRAY, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
			glEnableVertexAttribArray(0); 



			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			//

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
		
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);

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