#pragma once
#include "World/ECS/Registry.hpp"
#include "World/Components/HierarchyComponent.hpp"
#include "World/Components/TransformCacheComponent.hpp"
#include <vector>

namespace Nuim::World {

    class HierarchySystem
    {
    public:
        explicit HierarchySystem(Registry& r) : m_r(r) {}

        void Detach(Entity child);
        void SetParentRaw(Entity child, Entity newParent);

        Entity GetParent(Entity e) const;
        Entity GetFirstChild(Entity e) const;
        Entity GetNextSibling(Entity e) const;

        void MarkWorldDirtySubtree(Entity root);
        void CollectSubtree(Entity root, std::vector<Entity>& outNodes) const;

    private:
        void DetachNoRecalc(Entity child);
        void AttachChild(Entity parent, Entity child);

    private:
        Registry& m_r;
    };

}
