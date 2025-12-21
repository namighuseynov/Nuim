#pragma once
#include "World/ECS/Registry.hpp"

#include "World/Components/UUIDComponent.hpp"
#include "World/Components/NameComponent.hpp"
#include "World/Components/TransformComponent.hpp"
#include "World/Components/HierarchyComponent.hpp"
#include "World/Components/TransformCacheComponent.hpp"
#include "World/Components/ScriptComponent.hpp"
#include "World/Components/ActiveComponent.hpp"

#include "Core/UUID.hpp"
#include <string>
#include <vector>

namespace Nuim::World {

    class HierarchySystem;
    class TransformSystem;
    class ScriptSystem;
    class IWorldRenderer;

    class World
    {
    public:
        World();
        ~World();

        // --- Lifecycle ---
        void OnLoad();     // scene loaded
        void OnUnload();   // unload + clear
        void Clear();      // clears all entities/components safely

        // --- User API ---
        Entity CreateEntity(const std::string& name = "Entity");
        void   DestroyEntity(Entity e, bool recursive = true);

        Entity DuplicateEntity(Entity src, bool recursive = true);
        void   SetActive(Entity e, bool active, bool recursive = true);
        bool   IsActive(Entity e) const;

        bool IsAlive(Entity e) const { return m_registry.IsAlive(e); }
        Entity FindByUUID(const Nuim::UUID& uuid) const;

        Registry& GetRegistry() { return m_registry; }
        const Registry& GetRegistry() const { return m_registry; }

        HierarchySystem& Hierarchy() { return *m_hierarchy; }
        TransformSystem& Transform() { return *m_transform; }
        ScriptSystem& Scripts() { return *m_scripts; }

        // --- Runtime ---
        void Start();
        void Stop();
        bool IsRunning() const { return m_running; }

        // System order:
        // Transform (pre) -> Script -> Transform (post)
        void Update(float dt);
        void FixedUpdate(float fixedDt);

        // Render must happen after transforms are correct
        void Render(IWorldRenderer& renderer);

        void DispatchEvent(Nuim::Event& e);

    private:
        Registry m_registry;
        bool m_running = false;

        HierarchySystem* m_hierarchy = nullptr;
        TransformSystem* m_transform = nullptr;
        ScriptSystem* m_scripts = nullptr;
    };

} 
