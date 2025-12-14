#pragma once
#include "Scene.hpp"
#include "Renderer/Renderer.hpp"
#include "ResourceManager.hpp"
#include "Time.hpp"
#include "GameObject.hpp"

namespace Nuim {

    class Engine;

    class EditorLayer {
    public:
        void OnGui(Engine& engine);

    private:
        void DrawStats(Renderer* renderer);
        void DrawHierarchy(Scene& scene);
        void DrawInspector(Engine& engine);

    private:
        GameObject* m_selected = nullptr;
        void DrawAssetBrowser(Engine& engine);
        bool m_showAssets = true;
    };

}
