#pragma once
#include "World/ECS/Registry.hpp"
#include "World/Components/TransformComponent.hpp"
#include "World/Components/HierarchyComponent.hpp"
#include "World/Components/TransformCacheComponent.hpp"
#include "World/Systems/HierarchySystem.hpp"
#include <DirectXMath.h>

namespace Nuim::World {

    class TransformSystem
    {
    public:
        TransformSystem(Registry& r, HierarchySystem& h) : m_r(r), m_h(h) {}

        DirectX::XMMATRIX GetLocalMatrix(Entity e);
        DirectX::XMMATRIX GetWorldMatrix(Entity e);

        void MarkDirty(Entity e);
        void FlushDirtyAll();

        // keepWorld: keep child's WORLD transform unchanged when reparenting
        void SetParent(Entity child, Entity newParent, bool keepWorld);

    private:
        void RebuildLocalIfNeeded(Entity e);

        // NEW: non-recursive world rebuild for one entity (and required parents)
        void EnsureWorldUpToDate_NoRecursion(Entity e);

#if defined(_DEBUG) || defined(DEBUG)
        void DebugValidateKeepWorld(Entity child, Entity newParent, const DirectX::XMMATRIX& oldWorld);
#endif

    private:
        Registry& m_r;
        HierarchySystem& m_h;
    };

} 
