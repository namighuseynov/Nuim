#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "Renderer.hpp"
#include "ImGuiLayer.hpp"
#include <d3d11.h>

#include "Mesh.hpp"
#include "Material.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "GameObject.hpp"
#include "MeshRenderer.hpp"
#include "Scene.hpp"
#include "CameraComponent.hpp"

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

            NuimDemo::Time::Init();
            NuimDemo::Input::Init();

            window = new Window(1280, 800);
            this->window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
            renderer = new Renderer(window->GetHWND(), window->GetWidth(), window->GetHeight());

			float aspect = static_cast<float>(window->GetWidth()) / static_cast<float>(window->GetHeight());

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

			NuimDemo::GameObject& cameraObject = m_scene.CreateObject();
			CameraComponent* cameraComponent = cameraObject.AddComponent<CameraComponent>(renderer, aspect);
            m_scene.SetMainCamera(cameraComponent);

            NuimDemo::GameObject& cube = m_scene.CreateObject();
			cube.transform.SetPosition(DirectX::XMFLOAT3(0, 0, 0));
			cube.AddComponent<MeshRenderer>(renderer, &cubeMesh, &cubeMaterial);

            ImGuiRenderer* layer = new ImGuiRenderer(window->GetHWND(), renderer->GetDevice(), renderer->GetContext());

            bool done = false;

            while (!done) {
                NuimDemo::Time::Tick();
                NuimDemo::Input::NewFrame();
                float dt = Time::GetDeltaTime();

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

                layer->BeginFrame();

                ImGui::ShowDemoWindow();

                float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

                m_scene.Update(dt);

                renderer->BeginRender(clearColor);

                m_scene.Draw();

                ImGui::Render();
                ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

                renderer->EndRender(); 
            }
            layer->ShutDown();
            delete layer;
        }

        void OnWindowResize(uint32_t width, uint32_t height)
        {
            if (width == 0 || height == 0)
                return;

            if (renderer)
                renderer->Resize(width, height);

            if (auto cam = m_scene.GetMainCamera())
                cam->OnResize(width, height);

            float aspect = static_cast<float>(width) / static_cast<float>(height);
        }
    public:
        void OnEvent(EventSystem::Event& e) {
            std::cout << e.GetName() << std::endl;

            using namespace EventSystem;

            switch (e.GetType())
            {
            case EventType::WindowSizeEvent:
            {
                auto& ws = static_cast<WindowSizeEvent&>(e);
                OnWindowResize(ws.GetWidth(), ws.GetHeight());
				std::cout << "width: " << ws.GetWidth() << "height" << ws.GetHeight() << std::endl;
                
                break;
            }
            case EventType::MousePressEvent:
            {
                auto& me = static_cast<MousePressEvent&>(e);
                auto btn = me.GetMouseButton(); // int

                if (btn == (int)EventSystem::MouseButton::NM_LEFT)
                    NuimDemo::Input::OnMouseButtonDown(NuimDemo::MouseButton::Left);
                else if (btn == (int)EventSystem::MouseButton::NM_RIGHT)
                    NuimDemo::Input::OnMouseButtonDown(NuimDemo::MouseButton::Right);
                else if (btn == (int)EventSystem::MouseButton::NM_MIDDLE)
                    NuimDemo::Input::OnMouseButtonDown(NuimDemo::MouseButton::Middle);

                break;
            }
            case EventType::KeyReleaseEvent:
            {
                auto& ke = static_cast<KeyReleaseEvent&>(e);
                int keyCode = ke.GetKeyCode();
                NuimDemo::Input::OnKeyUp(keyCode);
                break;
            }
            case EventType::KeyPressEvent:
            {
                auto& ke = static_cast<KeyPressEvent&>(e);
                int keyCode = ke.GetKeyCode();
                NuimDemo::Input::OnKeyDown(keyCode);
                break;
            }

            case EventType::MouseMoveEvent:
            {
                auto& me = static_cast<MouseMoveEvent&>(e);
                NuimDemo::Input::OnMouseMove(me.GetX(), me.GetY());
                break;
            }
            case EventType::MouseReleaseEvent:
            {
                auto& me = static_cast<MouseReleaseEvent&>(e);
                auto btn = me.GetMouseButton();

                if (btn == (int)EventSystem::MouseButton::NM_LEFT)
                    NuimDemo::Input::OnMouseButtonUp(NuimDemo::MouseButton::Left);
                else if (btn == (int)EventSystem::MouseButton::NM_RIGHT)
                    NuimDemo::Input::OnMouseButtonUp(NuimDemo::MouseButton::Right);
                else if (btn == (int)EventSystem::MouseButton::NM_MIDDLE)
                    NuimDemo::Input::OnMouseButtonUp(NuimDemo::MouseButton::Middle);
                break;
            }
            case EventType::MouseMiddleButtonDownEvent:
            {
                NuimDemo::Input::OnMouseButtonDown(NuimDemo::MouseButton::Middle);
                break;
            }
            case EventType::MouseMiddleButtonReleaseEvent:
            {
                NuimDemo::Input::OnMouseButtonUp(NuimDemo::MouseButton::Middle);
                break;
            }
            default:
                break;
            }
        }

    private:
        Window* window = nullptr;
        Renderer* renderer = nullptr;
        HINSTANCE instance;
        Scene m_scene;
    };
}