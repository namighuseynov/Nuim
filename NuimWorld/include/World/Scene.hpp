#pragma once
#include <vector>
#include <string>
#include <algorithm>

#include "Core/Events/Event.hpp"
#include "Core/UUID.hpp"
#include "World/GameObject.hpp"

namespace Nuim::World {

    class Scene
    {
    public:
        GameObject& CreateGameObject(const std::string& name = "GameObject");

        void DestroyGameObject(Nuim::UUID id);

        GameObject* Find(Nuim::UUID id);

        void Update(float dt);

        void DispatchEvent(Nuim::Event& e);

        const std::vector<std::unique_ptr<GameObject>>& GetAll() const { return m_objects; }
        std::vector<std::unique_ptr<GameObject>>& GetAll() { return m_objects; }

    private:
        std::vector<std::unique_ptr<GameObject>> m_objects;
    };

} 
