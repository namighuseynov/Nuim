#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

#include "Core/UUID.hpp"
#include "Core/Events/Event.hpp"
#include "World/GameObject.hpp"

namespace Nuim::World {

    class Scene
    {
    public:
        GameObject& CreateGameObject(const std::string& name = "GameObject")
        {
            auto obj = std::make_unique<GameObject>(this, Nuim::UUID{});
            obj->SetName(name);

            m_objects.emplace_back(std::move(obj));
            return *m_objects.back();
        }

        void DestroyGameObject(Nuim::UUID id)
        {
            auto it = std::find_if(m_objects.begin(), m_objects.end(),
                [&](const std::unique_ptr<GameObject>& o) { return o->GetUUID() == id; });

            if (it != m_objects.end())
            {
                (*it)->RemoveAllComponents();
                *it = std::move(m_objects.back());
                m_objects.pop_back();
            }
        }

        GameObject* Find(Nuim::UUID id)
        {
            for (auto& o : m_objects)
                if (o->GetUUID() == id)
                    return o.get();
            return nullptr;
        }

        void OnRuntimeStart()
        {
            m_running = true;
            for (auto& o : m_objects)
                o->OnRuntimeStart();
        }

        void OnRuntimeStop()
        {
            for (auto& o : m_objects)
                o->OnRuntimeStop();
            m_running = false;
        }

        bool IsRunning() const { return m_running; }

        void Update(float dt)
        {
            if (!m_running) return;
            for (auto& o : m_objects)
                o->UpdateComponents(dt);
        }

        void DispatchEvent(Nuim::Event& e)
        {
            if (!m_running) return;
            for (auto& o : m_objects)
                o->DispatchEventToScripts(e);
        }

        const auto& GetAll() const { return m_objects; }

    private:
        std::vector<std::unique_ptr<GameObject>> m_objects;
        bool m_running = false;
    };

} 
