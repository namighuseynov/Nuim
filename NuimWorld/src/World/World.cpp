#include "World/World.hpp"
#include "World/ECS/View.hpp"

#include "World/Systems/HierarchySystem.hpp"
#include "World/Systems/TransformSystem.hpp"
#include "World/Systems/ScriptSystem.hpp"
#include "World/Render/IWorldRenderer.hpp"

#include <unordered_map>
#include <cstdint>

namespace Nuim::World {

    static inline uint64_t EntKey(Entity e)
    {
        return (uint64_t(e.id) << 32) | uint64_t(e.gen);
    }

    World::World()
    {
        m_hierarchy = new HierarchySystem(m_registry);
        m_transform = new TransformSystem(m_registry, *m_hierarchy);
        m_scripts = new ScriptSystem(m_registry, *this);
    }

    World::~World()
    {
        OnUnload();
        delete m_scripts;
        delete m_transform;
        delete m_hierarchy;
    }

    void World::OnLoad()
    {
        if (m_transform)
            m_transform->FlushDirtyAll();
    }

    void World::Clear()
    {

        Stop();


        m_registry.Clear();

    }

    void World::OnUnload()
    {
        Clear();
    }

    Entity World::CreateEntity(const std::string& name)
    {
        Entity e = m_registry.CreateEntity();

        m_registry.Emplace<UUIDComponent>(e);
        m_registry.Emplace<NameComponent>(e).name = name;
        m_registry.Emplace<ActiveComponent>(e).active = true;

        // Scene data
        m_registry.Emplace<TransformComponent>(e);

        // Runtime
        m_registry.Emplace<HierarchyComponent>(e);
        m_registry.Emplace<TransformCacheComponent>(e);

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

    bool World::IsActive(Entity e) const
    {
        if (!m_registry.IsAlive(e)) return false;
        if (!m_registry.Has<ActiveComponent>(e)) return true;
        return m_registry.Get<ActiveComponent>(e).active;
    }

    void World::SetActive(Entity e, bool active, bool recursive)
    {
        if (!m_registry.IsAlive(e)) return;

        std::vector<Entity> nodes;
        if (recursive && m_registry.Has<TransformComponent>(e))
            m_hierarchy->CollectSubtree(e, nodes);
        else
            nodes = { e };

        for (Entity x : nodes)
        {
            if (!m_registry.Has<ActiveComponent>(x))
                m_registry.Emplace<ActiveComponent>(x);

            auto& ac = m_registry.Get<ActiveComponent>(x);
            const bool was = ac.active;
            ac.active = active;

            if (m_running && was != active)
                m_scripts->OnEntityActiveChanged(x, active);
        }
    }

    void World::DestroyEntity(Entity e, bool recursive)
    {
        if (!m_registry.IsAlive(e)) return;

        if (recursive && m_registry.Has<HierarchyComponent>(e))
        {
            std::vector<Entity> nodes;
            m_hierarchy->CollectSubtree(e, nodes);

            // delete children first
            for (size_t i = nodes.size(); i-- > 0; )
            {
                Entity x = nodes[i];
                m_scripts->OnEntityDestroy(x);
                m_hierarchy->Detach(x);
                m_registry.DestroyEntity(x);
            }
            return;
        }

        // non-recursive: reparent children to root keeping world
        if (m_registry.Has<HierarchyComponent>(e))
        {
            auto& h = m_registry.Get<HierarchyComponent>(e);

            Entity ch = h.firstChild;
            while (ch != NullEntity)
            {
                Entity next = m_registry.Get<HierarchyComponent>(ch).nextSibling;
                m_transform->SetParent(ch, NullEntity, true);
                ch = next;
            }

            m_hierarchy->Detach(e);
        }

        m_scripts->OnEntityDestroy(e);
        m_registry.DestroyEntity(e);
    }


    Entity World::DuplicateEntity(Entity src, bool recursive)
    {
        if (!m_registry.IsAlive(src)) return NullEntity;

        // 1) collect subtree
        std::vector<Entity> nodes;
        if (recursive && m_registry.Has<HierarchyComponent>(src))
            m_hierarchy->CollectSubtree(src, nodes);
        else
            nodes = { src };

        // 2) create mapping old->new
        std::unordered_map<uint64_t, Entity> map;
        map.reserve(nodes.size() * 2);

        auto EntKey = [](Entity e) -> uint64_t
            {
                return (uint64_t(e.id) << 32) | uint64_t(e.gen);
            };

        for (Entity oldE : nodes)
        {

            const std::string oldName =
                m_registry.Has<NameComponent>(oldE) ? m_registry.Get<NameComponent>(oldE).name : "Entity";

            Entity newE = CreateEntity(oldName + " Copy");
            map.emplace(EntKey(oldE), newE);


            if (m_registry.Has<ActiveComponent>(oldE))
                m_registry.Get<ActiveComponent>(newE).active = m_registry.Get<ActiveComponent>(oldE).active;


            if (m_registry.Has<NameComponent>(oldE))
                m_registry.Get<NameComponent>(newE).name = m_registry.Get<NameComponent>(oldE).name + " Copy";


            if (m_registry.Has<TransformComponent>(oldE))
            {
                const auto& ot = m_registry.Get<TransformComponent>(oldE);
                auto& nt = m_registry.Get<TransformComponent>(newE);

                nt.localPos = ot.localPos;
                nt.localRot = ot.localRot;
                nt.localScale = ot.localScale;
                nt.shear = ot.shear;
            }

            if (m_registry.Has<TransformCacheComponent>(newE))
            {
                auto& cc = m_registry.Get<TransformCacheComponent>(newE);
                cc.dirtyLocal = true;
                cc.dirtyWorld = true;
            }

        }

        for (Entity oldE : nodes)
        {
            Entity newE = map[EntKey(oldE)];

            Entity oldParent = m_hierarchy->GetParent(oldE);
            Entity newParent = oldParent;

            if (oldParent != NullEntity)
            {
                auto it = map.find(EntKey(oldParent));
                if (it != map.end())
                    newParent = it->second;
            }

            m_transform->SetParent(newE, newParent, false);
        }

        return map[EntKey(src)];
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

        m_transform->FlushDirtyAll();

        m_scripts->Update(dt);

        m_transform->FlushDirtyAll();
    }

    void World::FixedUpdate(float fixedDt)
    {
        if (!m_running) return;

        m_transform->FlushDirtyAll();
        m_scripts->FixedUpdate(fixedDt);
        m_transform->FlushDirtyAll();
    }

    void World::Render(IWorldRenderer& renderer)
    {
        m_transform->FlushDirtyAll();
        renderer.Render(*this);
    }

    void World::DispatchEvent(Nuim::Event& e)
    {
        if (!m_running) return;
        m_scripts->DispatchEvent(e);
    }

} 
