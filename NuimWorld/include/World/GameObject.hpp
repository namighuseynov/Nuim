#pragma once
#include <memory>
#include <vector>
#include <string>

#include "Core/Events/Event.hpp"
#include "Core/UUID.hpp"
#include "World/Transform.hpp"
#include "World/IScriptComponent.hpp"

namespace Nuim::World {

    class Scene;

    class GameObject
    {
    public:
        GameObject() = default;
        GameObject(Scene* scene, Nuim::UUID id) : m_scene(scene), m_id(id) {}

        Nuim::UUID GetUUID() const { return m_id; }
        const std::string& GetName() const { return m_name; }
        void SetName(const std::string& n) { m_name = n; }

        Transform& GetTransform() { return m_transform; }
        const Transform& GetTransform() const { return m_transform; }

        // --- Script components (behaviours) ---
        template<typename T, typename... Args>
        T& AddComponent(Args&&... args);

        template<typename T>
        T* GetScript();

        void RemoveAllComponents();

        // internal update from Scene
        void UpdateComponents(float dt);

        void DispatchEventToScripts(Nuim::Event& e);

        void ImGuiScripts();

    private:
        Scene* m_scene = nullptr;
        Nuim::UUID m_id = Nuim::UUID(0);

        std::string m_name = "GameObject";
        Transform m_transform;

        std::vector<std::unique_ptr<IScriptComponent>> m_scripts;
    };

} // namespace Nuim::World
