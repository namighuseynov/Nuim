#include "World/Systems/HierarchySystem.hpp"

namespace Nuim::World {

    Entity HierarchySystem::GetParent(Entity e) const
    {
        if (!m_r.Has<HierarchyComponent>(e)) return NullEntity;
        return m_r.Get<HierarchyComponent>(e).parent;
    }

    Entity HierarchySystem::GetFirstChild(Entity e) const
    {
        if (!m_r.Has<HierarchyComponent>(e)) return NullEntity;
        return m_r.Get<HierarchyComponent>(e).firstChild;
    }

    Entity HierarchySystem::GetNextSibling(Entity e) const
    {
        if (!m_r.Has<HierarchyComponent>(e)) return NullEntity;
        return m_r.Get<HierarchyComponent>(e).nextSibling;
    }

    void HierarchySystem::MarkWorldDirtySubtree(Entity root)
    {
        if (!m_r.Has<HierarchyComponent>(root)) return;
        if (!m_r.Has<TransformCacheComponent>(root)) return;

        std::vector<Entity> stack;
        stack.reserve(64);
        stack.push_back(root);

        while (!stack.empty())
        {
            Entity e = stack.back();
            stack.pop_back();

            if (m_r.Has<TransformCacheComponent>(e))
                m_r.Get<TransformCacheComponent>(e).dirtyWorld = true;

            const auto& h = m_r.Get<HierarchyComponent>(e);
            Entity ch = h.firstChild;
            while (ch != NullEntity)
            {
                stack.push_back(ch);
                ch = m_r.Get<HierarchyComponent>(ch).nextSibling;
            }
        }
    }

    void HierarchySystem::CollectSubtree(Entity root, std::vector<Entity>& outNodes) const
    {
        outNodes.clear();
        outNodes.reserve(64);

        if (!m_r.Has<HierarchyComponent>(root))
        {
            outNodes.push_back(root);
            return;
        }

        std::vector<Entity> stack;
        stack.reserve(64);
        stack.push_back(root);

        while (!stack.empty())
        {
            Entity e = stack.back();
            stack.pop_back();
            outNodes.push_back(e);

            const auto& h = m_r.Get<HierarchyComponent>(e);
            Entity ch = h.firstChild;
            while (ch != NullEntity)
            {
                stack.push_back(ch);
                ch = m_r.Get<HierarchyComponent>(ch).nextSibling;
            }
        }
    }

    void HierarchySystem::DetachNoRecalc(Entity child)
    {
        auto& hc = m_r.Get<HierarchyComponent>(child);
        Entity parent = hc.parent;
        if (parent == NullEntity) return;

        auto& hp = m_r.Get<HierarchyComponent>(parent);

        if (hc.prevSibling != NullEntity)
            m_r.Get<HierarchyComponent>(hc.prevSibling).nextSibling = hc.nextSibling;
        if (hc.nextSibling != NullEntity)
            m_r.Get<HierarchyComponent>(hc.nextSibling).prevSibling = hc.prevSibling;

        if (hp.firstChild == child)
            hp.firstChild = hc.nextSibling;

        hc.parent = NullEntity;
        hc.prevSibling = NullEntity;
        hc.nextSibling = NullEntity;

        MarkWorldDirtySubtree(child);
    }

    void HierarchySystem::AttachChild(Entity parent, Entity child)
    {
        auto& hp = m_r.Get<HierarchyComponent>(parent);
        auto& hc = m_r.Get<HierarchyComponent>(child);

        Entity oldFirst = hp.firstChild;
        hp.firstChild = child;

        hc.parent = parent;
        hc.prevSibling = NullEntity;
        hc.nextSibling = oldFirst;

        if (oldFirst != NullEntity)
            m_r.Get<HierarchyComponent>(oldFirst).prevSibling = child;

        MarkWorldDirtySubtree(child);
    }

    void HierarchySystem::Detach(Entity child)
    {
        if (!m_r.IsAlive(child)) return;
        if (!m_r.Has<HierarchyComponent>(child)) return;
        DetachNoRecalc(child);
    }

    void HierarchySystem::SetParentRaw(Entity child, Entity newParent)
    {
        if (!m_r.IsAlive(child)) return;
        if (newParent != NullEntity && !m_r.IsAlive(newParent)) return;
        if (child == newParent) return;

        if (!m_r.Has<HierarchyComponent>(child)) return;
        if (newParent != NullEntity && !m_r.Has<HierarchyComponent>(newParent)) return;

        if (newParent != NullEntity)
        {
            for (Entity p = newParent; p != NullEntity; p = m_r.Get<HierarchyComponent>(p).parent)
                if (p == child) return;
        }

        auto& hc = m_r.Get<HierarchyComponent>(child);

        if (hc.parent != NullEntity)
            DetachNoRecalc(child);

        if (newParent != NullEntity)
            AttachChild(newParent, child);

        MarkWorldDirtySubtree(child);
    }

} 
