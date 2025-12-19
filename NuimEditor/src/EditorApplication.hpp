#pragma once

#include <memory>
#include <string>

#include "Core/Base.hpp"
#include "Core/Window.hpp"
#include "Core/LayerStack.hpp"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;

namespace NuimEditor {

    struct EditorApplicationSpecification
    {
        std::string Name = "NuimEditor";
        U32 Width = 1280;
        U32 Height = 720;
        bool VSync = true;
    };

    class EditorImGuiLayer;
    class EditorLayer;

    class EditorApplication
    {
    public:
        explicit EditorApplication(const EditorApplicationSpecification& spec = {});
        ~EditorApplication();

        void Run();
        void Close();

    private:
        void OnEvent(Nuim::Event& e);
        void HandleCoreInputEvents(Nuim::Event& e);

        // DX11
        void InitD3D11();
        void ShutdownD3D11();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        void ResizeSwapChain(U32 w, U32 h);

    private:
        EditorApplicationSpecification m_spec;

        std::unique_ptr<Nuim::Window> m_window;
        Nuim::LayerStack m_layers;

        bool m_running = true;
        bool m_minimized = false;

        // DX11 resources
        ID3D11Device* m_device = nullptr;
        ID3D11DeviceContext* m_context = nullptr;
        IDXGISwapChain* m_swapChain = nullptr;
        ID3D11RenderTargetView* m_rtv = nullptr;

        // Layers
        std::unique_ptr<EditorImGuiLayer> m_imgui;
        std::unique_ptr<EditorLayer>      m_editorLayer;
    };

} // namespace NuimEditor
