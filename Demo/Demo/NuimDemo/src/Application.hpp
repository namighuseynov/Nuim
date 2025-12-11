#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "Renderer.hpp"
#include "ImGuiLayer.hpp"
#include <d3d11.h>

#include "Mesh.hpp"
#include "Material.hpp"
#include "Model.hpp"
#include "Camera.hpp"

namespace NuimDemo {
    class Application {
    public:
        Application(const HINSTANCE& instance) : instance(instance) {};
        ~Application() {
            if (window) delete window;
            if (renderer) delete renderer;
        }
    public:
        void Run() {
            AllocConsole();
            freopen("CONOUT$", "w", stdout);

            window = new Window(1280, 800);
            this->window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
            renderer = new Renderer(window->GetHWND(), window->GetWidth(), window->GetHeight());

			float aspect = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());
			m_camera.SetPosition(DirectX::XMFLOAT3(0.0f, 0.0f, -4.0f));
            m_camera.SetYawPitch(0.0f, 0.0f);
            m_camera.SetPerspective(60.0f, aspect, 0.1f, 100.0f);

			renderer->SetCamera(
				m_camera.GetViewMatrix(),
				m_camera.GetProjMatrix()
			);

            float angle = 0.0f;

            struct VertexColored {
                DirectX::XMFLOAT3 position;
                DirectX::XMFLOAT4 color;
            };

            VertexColored vertices[] = {
            { { -0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f, 1.0f} },
            { { -0.5f,  0.5f, -0.5f }, {0.0f, 1.0f, 0.0f, 1.0f} },
            { {  0.5f,  0.5f, -0.5f }, {0.0f, 0.0f, 1.0f, 1.0f} },
            { {  0.5f, -0.5f, -0.5f }, {1.0f, 1.0f, 0.0f, 1.0f} },
            { { -0.5f, -0.5f,  0.5f }, {1.0f, 0.0f, 1.0f, 1.0f} },
            { { -0.5f,  0.5f,  0.5f }, {0.0f, 1.0f, 1.0f, 1.0f} },
            { {  0.5f,  0.5f,  0.5f }, {1.0f, 1.0f, 1.0f, 1.0f} },
            { {  0.5f, -0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f} },
            };

            UINT indices[] = {
            0,1,2,  0,2,3,
            4,6,5,  4,7,6,
            4,5,1,  4,1,0,
            3,2,6,  3,6,7,
            1,5,6,  1,6,2,
            4,0,3,  4,3,7
            };


            
            Mesh cubeMesh;
            if (!cubeMesh.Init(
                renderer->GetDevice(),
                vertices,
                sizeof(VertexColored),
                _countof(vertices),
                indices,
                _countof(indices))) {
                std::cout << "Failed to init cube mesh\n";
            }

            D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,
              0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
              0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
                };

			Material cubeMaterial;
            if (cubeMaterial.Init(
				renderer->GetDevice(),
				L"Shaders/VertexShader.hlsl",
				L"Shaders/PixelShader.hlsl",
				layoutDesc,
				_countof(layoutDesc)) == false) {
				std::cout << "Failed to init cube material\n";
			}

            Model cubeModel(&cubeMesh, &cubeMaterial);

            ImGuiRenderer* layer = new ImGuiRenderer(window->GetHWND(), renderer->GetDevice(), renderer->GetContext());

            bool done = false;

            while (!done) {

                MSG msg;
                while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
                {
                    ::TranslateMessage(&msg);
                    ::DispatchMessage(&msg);
                    if (msg.message == WM_QUIT)
                        done = true;
                }
                if (done)
                    break;

                angle += 0.02f; 

                float radius = 4.0f;
                float x = sinf(angle) * radius;
                float z = cosf(angle) * radius;

                m_camera.SetPosition(DirectX::XMFLOAT3(x, 0.0f, z));

                m_camera.LookAt(
                    DirectX::XMFLOAT3(x, 0.0f, z),
                    DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
                    DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)
                );


                renderer->SetCamera(
                    m_camera.GetViewMatrix(),
                    m_camera.GetProjMatrix()
                );

                layer->BeginFrame();

                ImGui::ShowDemoWindow();

                float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
                renderer->BeginRender(clearColor);
                
				cubeModel.Draw(renderer->GetContext(),
                    renderer->GetConstantBuffer(),
                    renderer->GetView(),
                    renderer->GetProj()
                );

                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

                renderer->EndRender(); 

            }
            layer->ShutDown();
        }

        void OnWindowResize(uint32_t width, uint32_t height)
        {
            if (width == 0 || height == 0)
                return;

            if (renderer)
                renderer->Resize(width, height);

            float aspect = static_cast<float>(width) / static_cast<float>(height);
            m_camera.SetPerspective(60.0f, aspect, 0.1f, 100.0f);

            renderer->SetCamera(
                m_camera.GetViewMatrix(),
                m_camera.GetProjMatrix()
            );
        }
    public:
        void OnEvent(EventSystem::Event& e) {
            std::cout << e.GetName() << std::endl;

            if (e.GetType() == EventSystem::EventType::WindowSizeEvent)
            {
                auto& ws = (EventSystem::WindowSizeEvent&)e;
                OnWindowResize(ws.GetWidth(), ws.GetHeight());
            }
        }
    private:
        Window* window = nullptr;
        Renderer* renderer = nullptr;
        Camera m_camera;
        HINSTANCE instance;
    };
}