#pragma once

#include "Core/Layer.hpp"

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
    };

}
