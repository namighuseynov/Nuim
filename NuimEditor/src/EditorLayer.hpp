#pragma once

#include <memory>
#include "Core/Layer.hpp"
#include "Core/Base.hpp"

#include "Render/IRenderTarget.hpp"
#include "World/Scene.hpp"
#include "World/FlyCameraScript.hpp"

namespace NuimEditor {

    class EditorLayer final : public Nuim::Layer
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
        void TogglePlayStop();

    private:
        // Scene runtime
        std::unique_ptr<Nuim::World::Scene> m_scene;
        bool m_playMode = false;

        // Viewport target (owned by EditorApplication)
        Nuim::Render::IRenderTarget* m_viewportTarget = nullptr;

        // Viewport size tracking
        Nuim::U32 m_viewportW = 1;
        Nuim::U32 m_viewportH = 1;

        Nuim::U32 m_pendingViewportW = 1;
        Nuim::U32 m_pendingViewportH = 1;
        bool m_viewportResizeRequested = false;

        // Optional: focus/hover state
        bool m_viewportFocused = false;
        bool m_viewportHovered = false;

        Nuim::World::FlyCameraScript* m_camera = nullptr;
    };

}
