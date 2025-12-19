#pragma once

#include "Core/Layer.hpp"
#include <functional>

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

        void SetViewportCallbacks(
            std::function<void* ()> getSrv,
            std::function<void(Nuim::U32, Nuim::U32)> requestResize
        )
        {
            m_getViewportSRV = std::move(getSrv);
            m_requestViewportResize = std::move(requestResize);
        }
    private:
        std::function<void* ()> m_getViewportSRV; 
        std::function<void(Nuim::U32, Nuim::U32)> m_requestViewportResize;

        Nuim::U32 m_lastViewportW = 0;
        Nuim::U32 m_lastViewportH = 0;
    };

}
