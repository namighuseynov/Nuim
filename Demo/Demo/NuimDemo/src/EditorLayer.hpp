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
        void DrawInspector(Engine& engine);
        void DrawAssetBrowser(Engine& engine);

        void DrawHierarchy(Engine& engine);
        void DrawHierarchyNode(Engine& engine, GameObject* obj);
        void DrawSceneView(Engine& engine);

    private:
        GameObject* m_selected = nullptr;
        std::unordered_map<Transform*, GameObject*> m_tfToObj;
        std::vector<GameObject*> m_roots;
    };

}
