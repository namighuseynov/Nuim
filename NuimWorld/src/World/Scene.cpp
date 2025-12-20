#include "World/Scene.hpp"
#include "World/GameObject.hpp"

#include <algorithm>
#include <memory>

namespace Nuim::World {

    GameObject& Scene::CreateGameObject(const std::string& name)
    {
        auto obj = std::make_unique<GameObject>(this, Nuim::UUID{});
        obj->SetName(name);

        m_objects.emplace_back(std::move(obj));
        return *m_objects.back();
    }

    void Scene::DestroyGameObject(Nuim::UUID id)
    {
        auto it = std::find_if(m_objects.begin(), m_objects.end(),
            [&](const std::unique_ptr<GameObject>& p)
            {
                return p && p->GetUUID() == id;
            });

        if (it == m_objects.end())
            return;

        (*it)->RemoveAllComponents();

        if (it != std::prev(m_objects.end()))
            *it = std::move(m_objects.back());

        m_objects.pop_back();
    }

    GameObject* Scene::Find(Nuim::UUID id)
    {
        for (auto& p : m_objects)
            if (p && p->GetUUID() == id)
                return p.get();
        return nullptr;
    }

    void Scene::Update(float dt)
    {
        for (auto& p : m_objects)
            if (p)
                p->UpdateComponents(dt);
    }

    void Scene::DispatchEvent(Nuim::Event& e)
    {
        for (auto& p : m_objects)
            if (p)
                p->DispatchEventToScripts(e);
    }

}
