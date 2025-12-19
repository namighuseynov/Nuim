#include "NuimPCH.h"
#include "Application.hpp"


#include "Event.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "ApplicationEvent.hpp"
#include "WindowEvent.hpp"


namespace Nuim {

    Application* Application::s_instance = nullptr;

    Application& Application::Get()
    {
        return *s_instance;
    }

    Application::Application(const ApplicationSpecification& spec)
        : m_spec(spec)
    {
        s_instance = this;

        WindowProps wp;
        wp.Title = m_spec.Name;
        wp.Width = m_spec.Width;
        wp.Height = m_spec.Height;
        wp.VSync = m_spec.VSync;

        m_window = Window::Create(wp);

        m_window->SetEventCallback([this](Event& e) { OnEvent(e); });

        Time::Init();
        OnInit();
    }

    Application::~Application()
    {
        OnShutdown();
        s_instance = nullptr;
    }

    void Application::Close()
    {
        m_running = false;
    }

    void Application::PushLayer(std::unique_ptr<Layer> layer)
    {
        if (!layer) return;
        layer->OnAttach();
        m_layerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(std::unique_ptr<Layer> overlay)
    {
        if (!overlay) return;
        overlay->OnAttach();
        m_layerStack.PushOverlay(std::move(overlay));
    }

    void Application::Run()
    {
        while (m_running)
        {
            m_window->PollEvents();

            Time::Tick();
            Input::NewFrame();
            float dt = Time::GetDeltaTime();

            OnBeginFrame();

            if (!m_minimized)
            {
                for (auto& layer : m_layerStack)
                    layer->OnUpdate(dt);
            }

            if (m_spec.EnableImGui)
            {
                for (auto& layer : m_layerStack)
                    layer->OnImGuiRender();
            }

            OnEndFrame();

        }
    }

    void Application::OnEvent(Event& e)
    {
        switch (e.GetType()) {
        case EventType::ApplicationCloseEvent:
            m_running = false;
            e.Handled = true;
            break;

        case EventType::WindowSizeEvent:
        {
            auto& re = static_cast<WindowSizeEvent&>(e);

            m_minimized = (re.GetWidth() == 0 || re.GetHeight() == 0);
            break;
        }
        default:
            break;
        }

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it)
        {
            if (e.Handled) break;
            (*it)->OnEvent(e);
        
        }
    }

}
