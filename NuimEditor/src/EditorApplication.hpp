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

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView; 

namespace NuimEditor {

    struct EditorApplicationSpecification
    {
        std::string Name = "NuimEditor";
        Nuim::U32 Width = 1280;
        Nuim::U32 Height = 720;
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

        void* GetViewportTextureSRV() const { return (void*)m_viewportSRV; } 
        void  SetViewportSize(Nuim::U32 w, Nuim::U32 h); 

    private:
        void OnEvent(Nuim::Event& e);
        void HandleCoreInputEvents(Nuim::Event& e);

        // DX11
        void InitD3D11();
        void ShutdownD3D11();
        void CreateRenderTarget();
        void CleanupRenderTarget();
        void ResizeSwapChain(Nuim::U32 w, Nuim::U32 h);

        void CreateViewportResources(Nuim::U32 w, Nuim::U32 h);
        void CleanupViewportResources();
        void RenderViewport();

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

        ID3D11Texture2D* m_viewportTex = nullptr;
        ID3D11RenderTargetView* m_viewportRTV = nullptr;
        ID3D11ShaderResourceView* m_viewportSRV = nullptr;
        ID3D11Texture2D* m_viewportDepth = nullptr;
        ID3D11DepthStencilView* m_viewportDSV = nullptr;
        Nuim::U32                  m_viewportW = 0;
        Nuim::U32                  m_viewportH = 0; 

        // Layers
        std::unique_ptr<EditorImGuiLayer> m_imgui;
        EditorLayer* m_editorLayerPtr = nullptr;
    };

} // namespace NuimEditor
