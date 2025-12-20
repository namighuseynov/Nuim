#pragma once

#include <memory>
#include <string>

#include "Core/Base.hpp"
#include "Core/Window.hpp"
#include "Core/LayerStack.hpp"

#include "Render/IRenderContext.hpp"
#include "Render/ISwapChain.hpp"
#include "Render/IRenderTarget.hpp"
#include "Render/RenderTypes.hpp"

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

    private:
        void OnEvent(Nuim::Event& e);

        void OnWindowResize(Nuim::U32 w, Nuim::U32 h);
        void BindBackbuffer();

    private:
        EditorApplicationSpecification m_spec;

        std::unique_ptr<Nuim::Window> m_window;
        Nuim::LayerStack m_layers;

        bool m_running = true;
        bool m_minimized = false;

        std::unique_ptr<Nuim::Render::IRenderContext> m_renderContext;
        std::unique_ptr<Nuim::Render::ISwapChain>     m_swapChain;
        std::unique_ptr<Nuim::Render::IRenderTarget>  m_viewportTarget;

        std::unique_ptr<EditorImGuiLayer> m_imgui;
        EditorLayer* m_editorLayer = nullptr;

    };

}
