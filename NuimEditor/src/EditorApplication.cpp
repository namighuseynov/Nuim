#include "EditorApplication.hpp"
#include "EditorLayer.hpp"


#include "Core/Base.hpp"
#include "Core/Input.hpp"
#include "Core/Time.hpp"
#include "Core/Log.hpp"

// events
#include "Core/Events/Event.hpp"
#include "Core/Events/ApplicationEvent.hpp"
#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/KeyEvent.hpp"
#include "Core/Events/MouseEvent.hpp"

// DX11 factory
#include "DX11/DX11Factory.hpp"

namespace NuimEditor {

    EditorApplication::EditorApplication(const EditorApplicationSpecification& spec)
        : m_spec(spec)
    {
        
        // 1) Window
        Nuim::WindowProps wp;
        wp.Title = m_spec.Name;
        wp.Width = m_spec.Width;
        wp.Height = m_spec.Height;
        wp.VSync = m_spec.VSync;

        m_window = Nuim::Window::Create(wp);
        m_window->SetEventCallback([this](Nuim::Event& e) { OnEvent(e); });

        m_editorLayer = nullptr;

        // Input/Time
        Nuim::Input::Init();
        Nuim::Time::Init();

        // 2) Render backend (DX11)
        m_renderContext = Nuim::DX11::CreateRenderContextDX11();

        Nuim::Render::SwapChainDesc sc{};
        sc.WindowHandle = m_window->GetNativeHandle();
        sc.Width = m_spec.Width;
        sc.Height = m_spec.Height;
        sc.VSync = m_spec.VSync;

        m_swapChain = Nuim::DX11::CreateSwapChainDX11(*m_renderContext, sc);

        Nuim::Render::RenderTargetDesc rt{};
        rt.Width = m_spec.Width;
        rt.Height = m_spec.Height;
        rt.HasDepth = true;

        m_viewportTarget = Nuim::DX11::CreateRenderTargetDX11(*m_renderContext, rt);

        // 3) ImGui layer
        m_imgui = Nuim::DX11::CreateImGuiBackendDX11();
        m_imgui->Init(m_window->GetNativeHandle(), m_renderContext.get());

        m_window->SetNativeMessageHook([this](void* hwnd, Nuim::U32 msg, Nuim::U64 wparam, Nuim::I64 lparam) -> bool
        {
                return m_imgui ? m_imgui->HandleNativeMessage(hwnd, msg, wparam, lparam) : false;
        });

        // 4) Editor UI Layer (Dockspace + Viewport)
        auto editorLayer = std::make_unique<EditorLayer>();
        editorLayer->SetViewportTarget(m_viewportTarget.get());

        m_layers.PushLayer(std::move(editorLayer));
    }

    EditorApplication::~EditorApplication()
    {
        // detach layers
        m_layers.Clear();
        m_editorLayer = nullptr;

        if (m_imgui)
        {
            m_imgui->Shutdown();
            m_imgui.reset();
        }

        // viewport / swapchain / context
        m_viewportTarget.reset();
        m_swapChain.reset();
        m_renderContext.reset();
        m_window.reset();
    }

    void EditorApplication::Close()
    {
        m_running = false;
    }

    void EditorApplication::Run()
    {
        while (m_running)
        {
            // 1) poll
            m_window->PollEvents();
            Nuim::Input::NewFrame();
            Nuim::Time::Tick();
            float dt = Nuim::Time::GetDeltaTime();

            // 2) update layers
            for (auto& layer : m_layers)
                layer->OnUpdate(dt);

            // 3) viewport resize from editor layer -> resize RT
            if (m_editorLayer && m_viewportTarget)
            {
                Nuim::U32 vw = 0, vh = 0;
                if (m_editorLayer->ConsumeViewportResize(vw, vh))
                    m_viewportTarget->Resize(vw, vh);
            }

            // 4) render scene to viewport target
            if (!m_minimized)
            {
                m_viewportTarget->Bind();
                m_viewportTarget->Clear(0.08f, 0.08f, 0.10f, 1.0f);

                // RenderScene() after

                // 5) render UI to swapchain backbuffer
                m_swapChain->BindBackbuffer();
                m_swapChain->ClearBackbuffer(0.06f, 0.06f, 0.07f, 1.0f);

                m_imgui->BeginFrame();
                for (auto& layer : m_layers)
                    layer->OnImGuiRender();
                m_imgui->EndFrame();
            }

            m_swapChain->Present();
        }
    }

    void EditorApplication::OnWindowResize(Nuim::U32 w, Nuim::U32 h)
    {
        if (w == 0 || h == 0)
        {
            m_minimized = true;
            return;
        }

        m_minimized = false;

        if (m_swapChain)
            m_swapChain->Resize(w, h);
    }

    void EditorApplication::OnEvent(Nuim::Event& e)
    {
        std::cout << e.GetName() << std::endl;
        switch (e.GetType())
        {
        case Nuim::EventType::WindowCloseEvent:
            m_running = false;
            e.Handled = true;
            break;

        case Nuim::EventType::WindowSizeEvent:
        {
            auto& ev = static_cast<Nuim::WindowSizeEvent&>(e);
            OnWindowResize(ev.GetWidth(), ev.GetHeight());
            break;
        }

        case Nuim::EventType::KeyPressEvent:
        {
            auto& ke = static_cast<Nuim::KeyPressEvent&>(e);
            Nuim::Input::OnKeyDown(ke.GetKeyCode());
            break;
        }
        case Nuim::EventType::KeyReleaseEvent:
        {
            auto& ke = static_cast<Nuim::KeyReleaseEvent&>(e);
            Nuim::Input::OnKeyUp(ke.GetKeyCode());
            break;
        }
        case Nuim::EventType::MouseMoveEvent: 
        {
            auto& me = static_cast<Nuim::MouseMoveEvent&>(e);
            Nuim::Input::OnMouseMove(me.GetX(), me.GetY());
            break;
        }
        case Nuim::EventType::MousePressEvent: 
        {
            auto& me = static_cast<Nuim::MousePressEvent&>(e);
            Nuim::MouseButton btn = (Nuim::MouseButton)me.GetMouseButton();
            Nuim::Input::OnMouseButtonDown(btn);
            break;
        }
        case Nuim::EventType::MouseReleaseEvent: 
        {
            auto& me = static_cast<Nuim::MouseReleaseEvent&>(e); 
            Nuim::MouseButton btn = (Nuim::MouseButton) me.GetMouseButton();
            Nuim::Input::OnMouseButtonUp(btn); 
            break;
        }

        default:
            break;
        }

        for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
        {
            if (e.Handled) break;
            (*it)->OnEvent(e);
        }
    }

}
