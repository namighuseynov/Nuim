#pragma once

#include "Core/Layer.hpp"
#include "Render/IRenderTarget.hpp"

namespace NuimEditor {

    class EditorLayer : public Nuim::Layer
    {
    public:
        EditorLayer();
        ~EditorLayer() override = default;

        void OnAttach() override;
        void OnDetach() override;

        void OnUpdate(float dt) override;
        void OnImGuiRender() override;
        void OnEvent(Nuim::Event& e) override;

        void SetViewportTarget(Nuim::Render::IRenderTarget* target) { m_viewportTarget = target; }

        bool ConsumeViewportResize(Nuim::U32& outW, Nuim::U32& outH);

    private:
        Nuim::Render::IRenderTarget* m_viewportTarget = nullptr;

        float m_viewportW = 0.0f;
        float m_viewportH = 0.0f;

        Nuim::U32 m_pendingViewportW = 0;
        Nuim::U32 m_pendingViewportH = 0;
        bool m_viewportResizeRequested = false;
    };

}
