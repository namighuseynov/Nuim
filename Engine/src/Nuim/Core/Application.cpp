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
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
		float vertices[] = {
				-0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f
		};

		unsigned int VBO, VAO;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);

		Shader shader(Nuim::LoadShader("Render/nvs.vert"), Nuim::LoadShader("Render/nfs.frag")); 
		char buf[512] = {};
		float r = 0.1f;
		float g = 0.5f;
		float b = 0.2f;

		while (!glfwWindowShouldClose(this->windowInstance)) 
		{
			glClear(GL_COLOR_BUFFER_BIT);
			
			//GL CODE
			glBindVertexArray(VAO);
			glBindBuffer(GL_VERTEX_ARRAY, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
			glEnableVertexAttribArray(0); 

			
			shader.Use();
			shader.SetColor(r, g, b);

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
			ImGui::Begin("Edit"); {
				ImGui::SliderFloat("r", &r, 0.0f, 1.0f);
				ImGui::SliderFloat("g", &g, 0.0f, 1.0f);
				ImGui::SliderFloat("b", &b, 0.0f, 1.0f);
				ImGui::End();
			}
			
			ImGui::ShowDemoWindow();
			
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