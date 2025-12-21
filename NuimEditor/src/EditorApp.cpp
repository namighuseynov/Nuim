#include "EditorApp.hpp"
#include "EditorLayer.hpp"

// DX11 factory
#include "DX11/DX11Factory.hpp"

namespace NuimEditor {

    EditorApp::EditorApp(const Nuim::ApplicationSpecification& spec)
        : Nuim::Application(spec)
    {
    }

    void EditorApp::OnInit()
    {
        // 1) DX11 context + swapchain
        m_renderContext = Nuim::DX11::CreateRenderContextDX11();

        Nuim::Render::SwapChainDesc sc{};
        sc.WindowHandle = GetWindow().GetNativeHandle();
        sc.Width = GetSpec().Width;
        sc.Height = GetSpec().Height;
        sc.VSync = GetSpec().VSync;

        m_swapChain = Nuim::DX11::CreateSwapChainDX11(*m_renderContext, sc);

        // 2) Viewport render target
        Nuim::Render::RenderTargetDesc rt{};
        rt.Width = GetSpec().Width;
        rt.Height = GetSpec().Height;
        rt.HasDepth = true;
        m_viewportTarget = Nuim::DX11::CreateRenderTargetDX11(*m_renderContext, rt);

        // 3) ImGui backend
        m_imgui = Nuim::DX11::CreateImGuiBackendDX11();
        m_imgui->Init(GetWindow().GetNativeHandle(), m_renderContext.get());

        GetWindow().SetNativeMessageHook([this](void* hwnd, Nuim::U32 msg, Nuim::U64 wp, Nuim::I64 lp) -> bool {
            return m_imgui ? m_imgui->HandleNativeMessage(hwnd, msg, wp, lp) : false;
            });

        // 4) Editor layer
        auto editorLayer = std::make_unique<EditorLayer>();
        editorLayer->SetViewportTarget(m_viewportTarget.get());

        m_editorLayer = editorLayer.get();
        PushLayer(std::move(editorLayer));
    }

    void EditorApp::OnShutdown()
    {
        GetLayerStack().Clear();
        m_editorLayer = nullptr;

        if (m_imgui)
        {
            m_imgui->Shutdown();
            m_imgui.reset();
        }

        m_viewportTarget.reset();
        m_swapChain.reset();
        m_renderContext.reset();
    }

    void EditorApp::OnBeginFrame()
    {
        if (m_editorLayer && m_viewportTarget)
        {
            Nuim::U32 vw = 0, vh = 0;
            if (m_editorLayer->ConsumeViewportResize(vw, vh))
                m_viewportTarget->Resize(vw, vh);
        }

        if (m_viewportTarget)
        {
            m_viewportTarget->Bind();
            m_viewportTarget->Clear(0.10f, 0.11f, 0.12f, 1.0f);
        }
    }

    void EditorApp::OnImGuiBegin()
    {
        if (m_swapChain)
        {
            m_swapChain->BindBackbuffer();
            m_swapChain->ClearBackbuffer(0.06f, 0.06f, 0.07f, 1.0f);
        }
        if (m_imgui) m_imgui->BeginFrame();
    }

    void EditorApp::OnImGuiEnd()
    {
        if (m_imgui) m_imgui->EndFrame();
    }

    void EditorApp::OnEndFrame()
    {
        if (m_swapChain) m_swapChain->Present();
    }

    void EditorApp::OnWindowResize(Nuim::U32 w, Nuim::U32 h)
    {
        if (m_swapChain)
            m_swapChain->Resize(w, h);
    }

}
