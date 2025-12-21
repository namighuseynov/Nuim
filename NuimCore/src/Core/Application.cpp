#include "NuimPCH.h"
#include "Application.hpp"

#include "Core/Time.hpp"
#include "Core/Input.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/KeyEvent.hpp"
#include "Core/Events/ApplicationEvent.hpp"
#include "Core/Events/WindowEvent.hpp"


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

        Input::Init();
        Time::Init();
    }

    Application::~Application()
    {
        s_instance = nullptr;
    }

    void Application::Close()
    {
        m_running = false;
    }

    void Application::PushLayer(std::unique_ptr<Layer> layer)
    {
        if (!layer) return;
        m_layerStack.PushLayer(std::move(layer));
    }

    void Application::PushOverlay(std::unique_ptr<Layer> overlay)
    {
        if (!overlay) return;
        m_layerStack.PushOverlay(std::move(overlay));
    }

    void Application::Run()
    {
        if (!m_initialized)
        {
            OnInit();
            m_initialized = true;
        }

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
                OnImGuiBegin();
                for (auto& layer : m_layerStack)
                    layer->OnImGuiRender();
                OnImGuiEnd();
            }

            OnEndFrame();
        }

        if (m_initialized)
        {
            OnShutdown();
            m_initialized = false;
        }
    }

    void Application::OnEvent(Event& e)
    {
        std::cout << e.GetName() << std::endl;

        switch (e.GetType()) {
        case EventType::WindowCloseEvent:
            m_running = false;
            e.Handled = true;
            break;

        case EventType::WindowSizeEvent:
        {
            auto& re = static_cast<WindowSizeEvent&>(e);

            m_minimized = (re.GetWidth() == 0 || re.GetHeight() == 0);

            if (!m_minimized)
                OnWindowResize(re.GetWidth(), re.GetHeight());
            break;
        }
        case EventType::KeyPressEvent:
        {
            auto& ke = static_cast<KeyPressEvent&>(e);
            Nuim::Input::OnKeyDown(ke.GetKeyCode());
            break;
        }
        case EventType::KeyReleaseEvent: 
        {
            auto& ke = static_cast<KeyReleaseEvent&>(e); 
            Input::OnKeyUp(ke.GetKeyCode()); 
            break;
        }
        case EventType::MouseMoveEvent: 
        {
            auto& me = static_cast<MouseMoveEvent&>(e); 
            Input::OnMouseMove(me.GetX(), me.GetY()); 
            break;
        }
        case EventType::MousePressEvent: 
        {
            auto& me = static_cast<MousePressEvent&>(e); 
            Input::OnMouseButtonDown((MouseButton)me.GetMouseButton()); 
            break;
        }
        case EventType::MouseReleaseEvent:
        {
            auto& me = static_cast<MouseReleaseEvent&>(e);
            Input::OnMouseButtonUp((MouseButton)me.GetMouseButton());
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
