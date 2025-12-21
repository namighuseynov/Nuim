#pragma once
#include "World/ECS/Registry.hpp"

#include "World/Components/UUIDComponent.hpp"
#include "World/Components/NameComponent.hpp"
#include "World/Components/TransformComponent.hpp"
#include "World/Components/ScriptComponent.hpp"

#include "Core/UUID.hpp"
#include <string>

namespace Nuim::World {

    class HierarchySystem;
    class TransformSystem;
    class ScriptSystem;

    class World
    {
    public:
        World();
        ~World();

        // Entity
        Entity CreateEntity(const std::string& name = "Entity");
        void   DestroyEntity(Entity e, bool recursive = true);

        bool IsAlive(Entity e) const { return m_registry.IsAlive(e); }
        Entity FindByUUID(const Nuim::UUID& uuid) const;

        Registry& GetRegistry() { return m_registry; }
        const Registry& GetRegistry() const { return m_registry; }

        HierarchySystem& Hierarchy() { return *m_hierarchy; }
        TransformSystem& Transform() { return *m_transform; }
        ScriptSystem& Scripts() { return *m_scripts; }

        // Runtime
        void Start();
        void Stop();
        bool IsRunning() const { return m_running; }

        void Update(float dt);
        void DispatchEvent(Nuim::Event& e);

    private:
        Registry m_registry;
        bool m_running = false;

        HierarchySystem* m_hierarchy = nullptr;
        TransformSystem* m_transform = nullptr;
        ScriptSystem* m_scripts = nullptr;
    };

}
