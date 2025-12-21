#include "World/Systems/HierarchySystem.hpp"

namespace Nuim::World {

    Entity HierarchySystem::GetParent(Entity e) const
    {
        if (!m_r.Has<TransformComponent>(e)) return NullEntity;
        return m_r.Get<TransformComponent>(e).parent;
    }
    Entity HierarchySystem::GetFirstChild(Entity e) const
    {
        if (!m_r.Has<TransformComponent>(e)) return NullEntity;
        return m_r.Get<TransformComponent>(e).firstChild;
    }
    Entity HierarchySystem::GetNextSibling(Entity e) const
    {
        if (!m_r.Has<TransformComponent>(e)) return NullEntity;
        return m_r.Get<TransformComponent>(e).nextSibling;
    }

    void HierarchySystem::MarkWorldDirtySubtree(Entity root)
    {
        if (!m_r.Has<TransformComponent>(root)) return;

        std::vector<Entity> stack;
        stack.reserve(64);
        stack.push_back(root);

        while (!stack.empty())
        {
            Entity e = stack.back();
            stack.pop_back();

            auto& tr = m_r.Get<TransformComponent>(e);
            tr.dirtyWorld = true;

            Entity ch = tr.firstChild;
            while (ch != NullEntity)
            {
                stack.push_back(ch);
                ch = m_r.Get<TransformComponent>(ch).nextSibling;
            }
        }
    }

    void HierarchySystem::CollectSubtree(Entity root, std::vector<Entity>& outNodes) const
    {
        outNodes.clear();
        outNodes.reserve(64);

        if (!m_r.Has<TransformComponent>(root))
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

            const auto& tr = m_r.Get<TransformComponent>(e);
            Entity ch = tr.firstChild;
            while (ch != NullEntity)
            {
                stack.push_back(ch);
                ch = m_r.Get<TransformComponent>(ch).nextSibling;
            }
        }
    }

    void HierarchySystem::DetachNoRecalc(Entity child)
    {
        auto& trChild = m_r.Get<TransformComponent>(child);
        Entity parent = trChild.parent;
        if (parent == NullEntity) return;

        auto& trParent = m_r.Get<TransformComponent>(parent);

        if (trChild.prevSibling != NullEntity)
            m_r.Get<TransformComponent>(trChild.prevSibling).nextSibling = trChild.nextSibling;
        if (trChild.nextSibling != NullEntity)
            m_r.Get<TransformComponent>(trChild.nextSibling).prevSibling = trChild.prevSibling;

        if (trParent.firstChild == child)
            trParent.firstChild = trChild.nextSibling;

        trChild.parent = NullEntity;
        trChild.prevSibling = NullEntity;
        trChild.nextSibling = NullEntity;

        trParent.dirtyWorld = true;
        MarkWorldDirtySubtree(child);
    }

    void HierarchySystem::AttachChild(Entity parent, Entity child)
    {
        auto& trParent = m_r.Get<TransformComponent>(parent);
        auto& trChild = m_r.Get<TransformComponent>(child);

        Entity oldFirst = trParent.firstChild;
        trParent.firstChild = child;

        trChild.parent = parent;
        trChild.prevSibling = NullEntity;
        trChild.nextSibling = oldFirst;

        if (oldFirst != NullEntity)
            m_r.Get<TransformComponent>(oldFirst).prevSibling = child;

        trParent.dirtyWorld = true;
        MarkWorldDirtySubtree(child);
    }

    void HierarchySystem::Detach(Entity child)
    {
        if (!m_r.IsAlive(child)) return;
        if (!m_r.Has<TransformComponent>(child)) return;
        DetachNoRecalc(child);
    }

    void HierarchySystem::SetParentRaw(Entity child, Entity newParent)
    {
        if (!m_r.IsAlive(child)) return;
        if (newParent != NullEntity && !m_r.IsAlive(newParent)) return;
        if (child == newParent) return;

        // prevent cycles
        if (newParent != NullEntity)
        {
            for (Entity p = newParent; p != NullEntity; p = m_r.Get<TransformComponent>(p).parent)
                if (p == child) return;
        }

        auto& trChild = m_r.Get<TransformComponent>(child);

        if (trChild.parent != NullEntity)
            DetachNoRecalc(child);

        if (newParent != NullEntity)
            AttachChild(newParent, child);

        MarkWorldDirtySubtree(child);
    }

}
