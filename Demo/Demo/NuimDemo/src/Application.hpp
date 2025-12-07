#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "Renderer.hpp"
#include "ImGuiLayer.hpp"
#include <d3d11.h>
#include "Cube.hpp"
#include "Cube3d.hpp"

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

                static Cube3d cube(renderer->GetDevice());

                layer->BeginFrame();

                ImGui::ShowDemoWindow();

                float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
                renderer->BeginRender(clearColor);
                cube.Draw(renderer->GetContext());

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
        HINSTANCE instance;
    };
}