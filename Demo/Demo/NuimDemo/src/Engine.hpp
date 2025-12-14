#pragma once
#include "Time.hpp"
#include "Input.hpp"
#include "Renderer/Renderer.hpp"
#include "EngineConfig.hpp"
#include "Window.hpp"
#include "ImGuiLayer.hpp"
#include "Scene.hpp"
#include "CameraComponent.hpp"
#include "IScene.hpp"
#include "ResourceManager.hpp"

namespace Nuim {
    class Engine {
    public:
        bool Init(const EngineConfig& config);
        void Run();
        void Shutdown();

        Scene& GetScene() { return m_scene; }
        Renderer* GetRenderer() const { return m_renderer.get(); }

        ResourceManager& GetResources() { return m_resources; }
        const ResourceManager& GetResources() const { return m_resources; }

        const EngineConfig& GetConfig() const { return m_config; }

        void LoadScene(std::unique_ptr<IScene> scene);

    private:
        void ProcessEvents();
        void Update(float dt);
        void LateUpdate(float dt);
        void Render();

        void OnEvent(EventSystem::Event& e);
        void OnWindowResize(uint32_t w, uint32_t h);
        void DrawEditorUI();
    private:
        bool m_running = false;

        EngineConfig m_config;

        std::unique_ptr<Window> m_window;
        std::unique_ptr<Renderer> m_renderer;
        std::unique_ptr<ImGuiRenderer> m_imgui;

        Scene m_scene;
        std::unique_ptr<IScene> m_activeScene;
        GameObject* m_selected = nullptr;
        ResourceManager m_resources;
    };
}