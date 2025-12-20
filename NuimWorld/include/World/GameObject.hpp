#pragma once
#include <memory>
#include <vector>
#include <string>
#include <type_traits>

#include "Core/UUID.hpp"
#include "Core/Events/Event.hpp"
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

        void OnRuntimeStart()
        {
            for (auto& s : m_scripts)
            {
                if (!s->m_attached)
                {
                    s->m_attached = true;
                    s->OnAttach();
                }
            }
        }

        void OnRuntimeStop()
        {
            for (auto& s : m_scripts)
            {
                if (s->m_attached)
                {
                    s->OnDetach();
                    s->m_attached = false;
                }
            }
        }

        void UpdateComponents(float dt)
        {
            for (auto& s : m_scripts)
                if (s->m_attached)
                    s->OnUpdate(dt);
        }

        void DispatchEventToScripts(Nuim::Event& e)
        {
            for (auto& s : m_scripts)
                if (s->m_attached)
                    s->OnEvent(e);
        }

        void ImGuiScripts()
        {
            for (auto& s : m_scripts)
                s->OnImGuiRender();
        }

        void RemoveAllComponents()
        {
            OnRuntimeStop();
            m_scripts.clear();
        }

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args)
        {
            static_assert(std::is_base_of_v<IScriptComponent, T>, "T must derive from IScriptComponent");

            auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
            ptr->SetOwner(this);

            ptr->m_attached = false;

            m_scripts.emplace_back(std::move(ptr));
            return *static_cast<T*>(m_scripts.back().get());
        }

        template<typename T>
        T* GetScript()
        {
            for (auto& s : m_scripts)
                if (auto* p = dynamic_cast<T*>(s.get()))
                    return p;
            return nullptr;
        }

    private:
        Scene* m_scene = nullptr;
        Nuim::UUID m_id = Nuim::UUID(0);

        std::string m_name = "GameObject";
        Transform m_transform;

        std::vector<std::unique_ptr<IScriptComponent>> m_scripts;
    };

} 
