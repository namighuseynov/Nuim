#include "World/World.hpp"
#include "World/ECS/View.hpp"

#include "World/Systems/HierarchySystem.hpp"
#include "World/Systems/TransformSystem.hpp"
#include "World/Systems/ScriptSystem.hpp"

namespace Nuim::World {

    World::World()
    {
        m_hierarchy = new HierarchySystem(m_registry);
        m_transform = new TransformSystem(m_registry, *m_hierarchy);
        m_scripts = new ScriptSystem(m_registry, *this);
    }

    World::~World()
    {
        Stop();
        delete m_scripts;
        delete m_transform;
        delete m_hierarchy;
    }

    Entity World::CreateEntity(const std::string& name)
    {
        Entity e = m_registry.CreateEntity();
        m_registry.Emplace<UUIDComponent>(e);
        m_registry.Emplace<NameComponent>(e).name = name;
        m_registry.Emplace<TransformComponent>(e);
        return e;
    }

    Entity World::FindByUUID(const Nuim::UUID& uuid) const
    {
        auto* set = m_registry.TryGetSet<UUIDComponent>();
        if (!set) return NullEntity;

        const auto& ents = set->DenseEntities();
        const auto& data = set->DenseData();
        for (size_t i = 0; i < ents.size(); ++i)
            if (data[i].uuid == uuid)
                return ents[i];

        return NullEntity;
    }

    void World::DestroyEntity(Entity e, bool recursive)
    {
        if (!m_registry.IsAlive(e)) return;

        if (recursive && m_registry.Has<TransformComponent>(e))
        {
            std::vector<Entity> nodes;
            m_hierarchy->CollectSubtree(e, nodes);

            for (size_t i = nodes.size(); i-- > 0; )
            {
                Entity x = nodes[i];
                m_scripts->OnEntityDestroy(x);
                m_hierarchy->Detach(x);
                m_registry.DestroyEntity(x);
            }
            return;
        }

        if (m_registry.Has<TransformComponent>(e))
        {
            auto& tr = m_registry.Get<TransformComponent>(e);
            Entity ch = tr.firstChild;
            while (ch != NullEntity)
            {
                Entity next = m_registry.Get<TransformComponent>(ch).nextSibling;
                m_transform->SetParent(ch, NullEntity, true);
                ch = next;
            }

            m_hierarchy->Detach(e);
        }

        m_scripts->OnEntityDestroy(e);
        m_registry.DestroyEntity(e);
    }

    void World::Start()
    {
        if (m_running) return;
        m_running = true;
        m_scripts->OnRuntimeStart();
    }

    void World::Stop()
    {
        if (!m_running) return;
        m_scripts->OnRuntimeStop();
        m_running = false;
    }

    void World::Update(float dt)
    {
        if (!m_running) return;
        m_scripts->Update(dt);
    }

    void World::DispatchEvent(Nuim::Event& e)
    {
        if (!m_running) return;
        m_scripts->DispatchEvent(e);
    }

}
