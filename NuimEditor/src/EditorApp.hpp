#pragma once
#include "Core/Application.hpp"

#include "Render/IRenderContext.hpp"
#include "Render/ISwapChain.hpp"
#include "Render/IRenderTarget.hpp"
#include "Render/IImGuiBackend.hpp"

namespace NuimEditor {

    class EditorLayer;

    class EditorApp final : public Nuim::Application
    {
    public:
        explicit EditorApp(const Nuim::ApplicationSpecification& spec = {});
        ~EditorApp() override = default;

    protected:
        void OnInit() override;
        void OnShutdown() override;

        void OnBeginFrame() override;
        void OnImGuiBegin() override;
        void OnImGuiEnd() override;
        void OnEndFrame() override;

        void OnWindowResize(Nuim::U32 w, Nuim::U32 h) override;

    private:
        std::unique_ptr<Nuim::Render::IRenderContext> m_renderContext;
        std::unique_ptr<Nuim::Render::ISwapChain>     m_swapChain;
        std::unique_ptr<Nuim::Render::IRenderTarget>  m_viewportTarget;
        std::unique_ptr<Nuim::Render::IImGuiBackend>  m_imgui;

        EditorLayer* m_editorLayer = nullptr;
    };

}
