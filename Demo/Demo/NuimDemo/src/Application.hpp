#pragma once
#include "Window.hpp"
#include "EventSystem.hpp"
#include "Renderer.hpp"
#include "ImGuiLayer.hpp"

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

            window = new Window();
            this->window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
            renderer = new Renderer(window);


            ImGuiRenderer* layer = new ImGuiRenderer(renderer);
            layer->Initialize();

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


                layer->OnUpdate();

                // Present
                HRESULT hr = renderer->GetSwapChain()->Present(1, 0);   // Present with vsync
            }
            layer->ShutDown();
        }
    public:
        void OnEvent(EventSystem::Event& e) {
            std::cout << e.GetName() << std::endl;
        }
    private:
        Window* window = nullptr;
        Renderer* renderer = nullptr;
        HINSTANCE instance;
    };
}