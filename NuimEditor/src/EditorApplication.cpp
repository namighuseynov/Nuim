#include "EditorApplication.hpp"

#include <stdexcept>
#include <vector>

#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "Core/Time.hpp"
#include "Core/Input.hpp"
#include "Core/Log.hpp"

#include "Core/Events/Event.hpp"
#include "Core/Events/KeyEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/WindowEvent.hpp"
#include "Core/Events/ApplicationEvent.hpp"

#include "EditorImGuiLayer.hpp"
#include "EditorLayer.hpp"

namespace NuimEditor {

    static void SafeRelease(IUnknown*& p)
    {
        if (p) { p->Release(); p = nullptr; }
    }

    EditorApplication::EditorApplication(const EditorApplicationSpecification& spec)
        : m_spec(spec)
    {
        // --- Window ---
        Nuim::WindowProps wp;
        wp.Title = m_spec.Name;
        wp.Width = m_spec.Width;
        wp.Height = m_spec.Height;
        wp.VSync = m_spec.VSync;

        m_window = Nuim::Window::Create(wp);
        m_window->SetEventCallback([this](Nuim::Event& e) { OnEvent(e); });

        // --- Core systems ---
        Nuim::Time::Init();
        Nuim::Input::Init();

        // --- D3D11 ---
        InitD3D11();

        // --- ImGui ---
        m_imgui = std::make_unique<EditorImGuiLayer>();
        m_imgui->Init(
            (HWND)m_window->GetNativeHandle(),
            m_device,
            m_context
        );


        m_window->SetNativeMessageHook([this](void* hwnd, Nuim::U32 msg, Nuim::U64 wp, Nuim::I64 lp) -> bool {

            if (m_imgui && m_imgui->HandleWin32Message((HWND)hwnd, (UINT)msg, (WPARAM)wp, (LPARAM)lp))
                return true;


            if (msg == WM_CLOSE) {
                m_running = false;
                return false;
            }

            return false; 
            });

        // --- Editor layer ---
        m_editorLayer = std::make_unique<EditorLayer>();
        m_editorLayer->OnAttach();
        m_layers.PushLayer(std::move(m_editorLayer));
    }

    EditorApplication::~EditorApplication()
    {
        // detach layers
        m_layers.Clear();

        if (m_imgui) {
            m_imgui->Shutdown();
            m_imgui.reset();
        }

        ShutdownD3D11();
    }

    void EditorApplication::Close()
    {
        m_running = false;
    }

    void EditorApplication::Run()
    {
        while (m_running)
        {
            // input
            Nuim::Input::NewFrame();

            // events
            m_window->PollEvents();

            // time
            Nuim::Time::Tick();
            const float dt = Nuim::Time::GetDeltaTime();

            // update
            if (!m_minimized)
            {
                for (auto& layer : m_layers)
                    layer->OnUpdate(dt);
            }

            // render (DX11 clear)
            if (!m_minimized && m_context && m_rtv)
            {
                const float clear[4] = { 0.07f, 0.07f, 0.09f, 1.0f };
                m_context->OMSetRenderTargets(1, &m_rtv, nullptr);
                m_context->ClearRenderTargetView(m_rtv, clear);
            }

            // imgui
            if (!m_minimized && m_imgui)
            {
                m_imgui->BeginFrame();

                for (auto& layer : m_layers)
                    layer->OnImGuiRender();

                m_imgui->EndFrame();
            }

            // present
            if (m_swapChain)
                m_swapChain->Present(m_spec.VSync ? 1 : 0, 0);
        }
    }

    // -------------------- Events --------------------

    void EditorApplication::OnEvent(Nuim::Event& e)
    {
        HandleCoreInputEvents(e);

        switch (e.GetType())
        {
        case Nuim::EventType::WindowSizeEvent:
        {
            auto& ev = static_cast<Nuim::WindowSizeEvent&>(e);
            const Nuim::U32 w = ev.GetWidth();
            const Nuim::U32 h = ev.GetHeight();

            m_minimized = (w == 0 || h == 0);

            if (!m_minimized)
                ResizeSwapChain(w, h);

            break;
        }
        case Nuim::EventType::ApplicationCloseEvent:
            m_running = false;
            e.Handled = true;
            break;
        default:
            break;
        }

        for (auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
        {
            if (e.Handled) break;
            (*it)->OnEvent(e);
        }
    }

    void EditorApplication::HandleCoreInputEvents(Nuim::Event& e)
    {
        using namespace Nuim;

        switch (e.GetType())
        {
        case EventType::KeyPressEvent:
        {
            auto& ke = static_cast<KeyPressEvent&>(e);
            Input::OnKeyDown((int)ke.GetKeyCode());
            break;
        }
        case EventType::KeyReleaseEvent:
        {
            auto& ke = static_cast<KeyReleaseEvent&>(e);
            Input::OnKeyUp((int)ke.GetKeyCode());
            break;
        }
        case EventType::MouseMoveEvent:
        {
            auto& me = static_cast<MouseMoveEvent&>(e);
            Input::OnMouseMove((int)me.GetX(), (int)me.GetY());
            break;
        }
        case EventType::MousePressEvent:
        {
            auto& me = static_cast<MousePressEvent&>(e);

            MouseButton btn = MouseButton::NM_LEFT;
            if (me.GetMouseButton() == 0) btn = MouseButton::NM_LEFT;
            else if (me.GetMouseButton() == 1) btn = MouseButton::NM_RIGHT;
            else if (me.GetMouseButton() == 2) btn = MouseButton::NM_MIDDLE;

            Input::OnMouseButtonDown(btn);
            break;
        }
        case EventType::MouseReleaseEvent:
        {
            auto& me = static_cast<MouseReleaseEvent&>(e);
            MouseButton btn = MouseButton::NM_LEFT;
            if (me.GetMouseButton() == 0) btn = MouseButton::NM_LEFT;
            else if (me.GetMouseButton() == 1) btn = MouseButton::NM_RIGHT;
            else if (me.GetMouseButton() == 2) btn = MouseButton::NM_MIDDLE;

            Input::OnMouseButtonUp(btn);
            break;
        }
        default:
            break;
        }
    }

    // -------------------- D3D11 --------------------

    void EditorApplication::InitD3D11()
    {
        HWND hwnd = (HWND)m_window->GetNativeHandle();
        if (!hwnd)
            throw std::runtime_error("EditorApplication: HWND is null");

        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = m_spec.Width;
        sd.BufferDesc.Height = m_spec.Height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.OutputWindow = hwnd;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.Windowed = TRUE;
        sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        sd.Flags = 0;

        UINT createFlags = 0;
#if defined(NUIM_DEBUG)
        createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };
        D3D_FEATURE_LEVEL obtained = {};

        HRESULT hr = D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createFlags,
            featureLevels,
            (UINT)std::size(featureLevels),
            D3D11_SDK_VERSION,
            &sd,
            &m_swapChain,
            &m_device,
            &obtained,
            &m_context
        );

        if (FAILED(hr))
            throw std::runtime_error("D3D11CreateDeviceAndSwapChain failed");

        CreateRenderTarget();
    }

    void EditorApplication::ShutdownD3D11()
    {
        CleanupRenderTarget();
        SafeRelease((IUnknown*&)m_swapChain);
        SafeRelease((IUnknown*&)m_context);
        SafeRelease((IUnknown*&)m_device);
    }

    void EditorApplication::CreateRenderTarget()
    {
        if (!m_swapChain || !m_device)
            return;

        ID3D11Texture2D* backBuffer = nullptr;
        HRESULT hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
        if (FAILED(hr) || !backBuffer)
            throw std::runtime_error("GetBuffer(backbuffer) failed");

        hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_rtv);
        backBuffer->Release();

        if (FAILED(hr) || !m_rtv)
            throw std::runtime_error("CreateRenderTargetView failed");
    }

    void EditorApplication::CleanupRenderTarget()
    {
        SafeRelease((IUnknown*&)m_rtv);
    }

    void EditorApplication::ResizeSwapChain(Nuim::U32 w, Nuim::U32 h)
    {
        if (!m_swapChain)
            return;

        CleanupRenderTarget();

        HRESULT hr = m_swapChain->ResizeBuffers(0, (UINT)w, (UINT)h, DXGI_FORMAT_UNKNOWN, 0);
        if (FAILED(hr))
            throw std::runtime_error("SwapChain ResizeBuffers failed");

        CreateRenderTarget();
    }

} 
