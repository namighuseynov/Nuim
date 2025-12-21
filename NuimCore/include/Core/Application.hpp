#pragma once
#include "Base.hpp"
#include "LayerStack.hpp"
#include "Window.hpp"

#include <memory>
#include <string>

namespace Nuim {

    struct ApplicationSpecification
    {
        std::string Name = "NuimApp";
        U32 Width = 1280;
        U32 Height = 720;
        bool VSync = true;

        bool EnableImGui = true;
    };

    class Application
    {
    public:
        explicit Application(const ApplicationSpecification& spec = {});
        virtual ~Application();

        void Run();
        void Close();

        void PushLayer(std::unique_ptr<Layer> layer);
        void PushOverlay(std::unique_ptr<Layer> overlay);

        Window& GetWindow() { return *m_window; }
        const ApplicationSpecification& GetSpec() const { return m_spec; }

        static Application& Get();

    protected:
        virtual void OnInit() {}
        virtual void OnShutdown() {}
        virtual void OnBeginFrame() {}
        virtual void OnEndFrame() {}

        virtual void OnImGuiBegin() {}
        virtual void OnImGuiEnd() {}
        virtual void OnWindowResize(U32 w, U32 h) {}

        LayerStack& GetLayerStack() { return m_layerStack; }

    private:
        void OnEvent(Event& e);

    private:
        ApplicationSpecification m_spec;
        std::unique_ptr<Window> m_window;
        LayerStack m_layerStack;

        bool m_running = true;
        bool m_minimized = false;
        bool m_initialized = false;

        static Application* s_instance;

    };

}
