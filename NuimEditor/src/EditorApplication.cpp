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
        HWND hwnd = (HWND)m_window->GetNativeHandle();
        m_imgui->Init(
            hwnd,
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
        CreateViewportResources(m_spec.Width, m_spec.Height);

        auto editor = std::make_unique<EditorLayer>();
        m_editorLayerPtr = editor.get();

        m_editorLayerPtr->SetViewportCallbacks(
            [this]() -> void* { return GetViewportTextureSRV(); },
            [this](Nuim::U32 w, Nuim::U32 h) { SetViewportSize(w, h); }
        );

        editor->OnAttach();
        m_layers.PushOverlay(std::move(editor));
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

    void EditorApplication::SetViewportSize(Nuim::U32 w, Nuim::U32 h)
    {
        if (w == 0 || h == 0) return;
        if (w == m_viewportW && h == m_viewportH) return;

        CleanupViewportResources();
        CreateViewportResources(w, h);
    }

    void EditorApplication::CreateViewportResources(Nuim::U32 w, Nuim::U32 h)
    {
        m_viewportW = w;
        m_viewportH = h;

        // --- Color texture (RenderTarget + ShaderResource) ---
        D3D11_TEXTURE2D_DESC td{};
        td.Width = w;
        td.Height = h;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_DEFAULT;
        td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

        HRESULT hr = m_device->CreateTexture2D(&td, nullptr, &m_viewportTex);
        if (FAILED(hr) || !m_viewportTex) throw std::runtime_error("CreateTexture2D viewport failed");

        hr = m_device->CreateRenderTargetView(m_viewportTex, nullptr, &m_viewportRTV);
        if (FAILED(hr) || !m_viewportRTV) throw std::runtime_error("CreateRenderTargetView viewport failed");

        hr = m_device->CreateShaderResourceView(m_viewportTex, nullptr, &m_viewportSRV);
        if (FAILED(hr) || !m_viewportSRV) throw std::runtime_error("CreateShaderResourceView viewport failed");

        // --- Depth texture ---
        D3D11_TEXTURE2D_DESC dd{};
        dd.Width = w;
        dd.Height = h;
        dd.MipLevels = 1;
        dd.ArraySize = 1;
        dd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        dd.SampleDesc.Count = 1;
        dd.Usage = D3D11_USAGE_DEFAULT;
        dd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

        hr = m_device->CreateTexture2D(&dd, nullptr, &m_viewportDepth);
        if (FAILED(hr) || !m_viewportDepth) throw std::runtime_error("CreateTexture2D depth failed");

        hr = m_device->CreateDepthStencilView(m_viewportDepth, nullptr, &m_viewportDSV);
        if (FAILED(hr) || !m_viewportDSV) throw std::runtime_error("CreateDepthStencilView failed");
    }

    void EditorApplication::CleanupViewportResources()
    {
        if (m_viewportDSV) { m_viewportDSV->Release(); m_viewportDSV = nullptr; }
        if (m_viewportDepth) { m_viewportDepth->Release(); m_viewportDepth = nullptr; }
        if (m_viewportSRV) { m_viewportSRV->Release(); m_viewportSRV = nullptr; }
        if (m_viewportRTV) { m_viewportRTV->Release(); m_viewportRTV = nullptr; }
        if (m_viewportTex) { m_viewportTex->Release(); m_viewportTex = nullptr; }
    }

    void EditorApplication::RenderViewport()
    {
        if (!m_viewportRTV || !m_viewportDSV || m_viewportW == 0 || m_viewportH == 0)
            return;

        // bind viewport RT
        m_context->OMSetRenderTargets(1, &m_viewportRTV, m_viewportDSV);

        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.Width = (float)m_viewportW;
        vp.Height = (float)m_viewportH;
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_context->RSSetViewports(1, &vp);

        const float color[4] = { 0.12f, 0.12f, 0.14f, 1.0f };
        m_context->ClearRenderTargetView(m_viewportRTV, color);
        m_context->ClearDepthStencilView(m_viewportDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
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

            if (!m_minimized)
            {
                RenderViewport();

                // --- DX11 clear ---
                const float clearColor[4] = { 0.08f, 0.08f, 0.09f, 1.0f };
                m_context->OMSetRenderTargets(1, &m_rtv, nullptr);
                m_context->ClearRenderTargetView(m_rtv, clearColor);

                D3D11_VIEWPORT wvp{};
                wvp.TopLeftX = 0;
                wvp.TopLeftY = 0;
                wvp.Width = (float)m_window->GetWidth();
                wvp.Height = (float)m_window->GetHeight();
                wvp.MinDepth = 0.0f;
                wvp.MaxDepth = 1.0f;
                m_context->RSSetViewports(1, &wvp); 

                // --- ImGui ---
                m_imgui->BeginFrame();
                for (auto& layer : m_layers)
                    layer->OnImGuiRender();
                m_imgui->EndFrame();

                // present
                m_swapChain->Present(m_spec.VSync ? 1 : 0, 0);
            }
            else
            {
                Sleep(1);
            }
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
        case Nuim::EventType::WindowCloseEvent: 
        {
            m_running = false;
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

        Nuim::Log::Info(e.GetName());

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

        if (!hwnd) {
            Nuim::Log::Error("EditorApplication: HWND is null");
            throw std::runtime_error("EditorApplication: HWND is null");
        }

        DXGI_SWAP_CHAIN_DESC sd{};
        sd.BufferCount = 2;
        sd.BufferDesc.Width = m_spec.Width;
        sd.BufferDesc.Height = m_spec.Height;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 60;
        sd.BufferDesc.RefreshRate.Denominator = 1;
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

        if (FAILED(hr)) {
            Nuim::Log::Error("D3D11CreateDeviceAndSwapChain failed");
            throw std::runtime_error("D3D11CreateDeviceAndSwapChain failed");
        }

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

        D3D11_VIEWPORT vp{};
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
        vp.Width = (float)m_window->GetWidth();
        vp.Height = (float)m_window->GetHeight();
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        m_context->RSSetViewports(1, &vp);
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
        if (FAILED(hr)) {
            Nuim::Log::Error("ResizeBuffers failed");
            throw std::runtime_error("SwapChain ResizeBuffers failed");
        }
            
        CreateRenderTarget();
    }

} 
