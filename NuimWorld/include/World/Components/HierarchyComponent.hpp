#pragma once
#include "World/ECS/Entity.hpp"

namespace Nuim::World {

    struct HierarchyComponent
    {
        Entity parent = NullEntity;
        Entity firstChild = NullEntity;
        Entity nextSibling = NullEntity;
        Entity prevSibling = NullEntity;
    };

}
