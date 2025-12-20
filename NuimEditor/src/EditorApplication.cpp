#include "EditorApplication.hpp"

#include "EditorImGuiLayer.hpp"
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

#ifdef NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#include <d3d11.h>
#endif

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
        m_imgui = std::make_unique<EditorImGuiLayer>();
        m_imgui->Init(
            (HWND)m_window->GetNativeHandle(),
            (ID3D11Device*)m_renderContext->GetNativeDevice(),
            (ID3D11DeviceContext*)m_renderContext->GetNativeContext()
        );

        m_window->SetNativeMessageHook([this](void* hwnd, Nuim::U32 msg, Nuim::U64 wparam, Nuim::I64 lparam) -> bool
        {
                return m_imgui ? m_imgui->HandleWin32Message((HWND)hwnd, (UINT)msg, (WPARAM)wparam, (LPARAM)lparam) : false;
        });

        // 4) Editor UI Layer (Dockspace + Viewport)
        auto editorLayer = std::make_unique<EditorLayer>();
        editorLayer->SetViewportTarget(m_viewportTarget.get());

        m_editorLayer = editorLayer.get();

        m_editorLayer->OnAttach();
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
            // 1) messages/events
            m_window->PollEvents();

            Nuim::Input::NewFrame(); 

            // 2) time
            Nuim::Time::Tick();
            float dt = Nuim::Time::GetDeltaTime();

            if (m_editorLayer && m_viewportTarget)
            {
                Nuim::U32 vw = 0, vh = 0;
                if (m_editorLayer->ConsumeViewportResize(vw, vh))
                {
                    m_viewportTarget->Resize(vw, vh);
                }
            }

            if (!m_minimized)
            {
                // 5.1) Render to viewport target (offscreen)
                m_viewportTarget->Bind();
                m_viewportTarget->Clear(0.08f, 0.08f, 0.10f, 1.0f);

                // RenderScene()

                // 5.2) Backbuffer for ImGui
                BindBackbuffer();

#ifdef NUIM_PLATFORM_WINDOWS
                auto* ctx = (ID3D11DeviceContext*)m_renderContext->GetNativeContext();
                auto* rtv = (ID3D11RenderTargetView*)m_swapChain->GetNativeBackbufferRTV();
                float bg[4] = { 0.06f, 0.06f, 0.07f, 1.0f };
                ctx->ClearRenderTargetView(rtv, bg);
#endif

                // 4) ImGui
                m_imgui->BeginFrame();

                for (auto& layer : m_layers)
                {
                    layer->OnUpdate(dt);
                    layer->OnImGuiRender();
                }

                m_imgui->EndFrame();
            }

            // 5) Present
            m_swapChain->Present();
        }
    }

    void EditorApplication::BindBackbuffer()
    {
#ifdef NUIM_PLATFORM_WINDOWS
        auto* ctx = (ID3D11DeviceContext*)m_renderContext->GetNativeContext();
        auto* rtv = (ID3D11RenderTargetView*)m_swapChain->GetNativeBackbufferRTV();

        ctx->OMSetRenderTargets(1, &rtv, nullptr);

        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        vp.Width = (float)m_swapChain->GetWidth();
        vp.Height = (float)m_swapChain->GetHeight();
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        ctx->RSSetViewports(1, &vp);
#endif
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
            Nuim::Input::OnKeyDown((int)ke.GetKeyCode());
            break;
        }
        case Nuim::EventType::KeyReleaseEvent:
        {
            auto& ke = static_cast<Nuim::KeyReleaseEvent&>(e);
            Nuim::Input::OnKeyUp((int)ke.GetKeyCode());
            break;
        }
        case Nuim::EventType::MouseMoveEvent: 
        {
            auto& me = static_cast<Nuim::MouseMoveEvent&>(e);
            Nuim::Input::OnMouseMove((int)me.GetX(), (int)me.GetY());
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
