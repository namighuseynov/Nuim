#pragma once
#include "Core/Base.hpp"

namespace Nuim::World {

    struct Entity
    {
        Nuim::U32 id = 0;
        Nuim::U32 gen = 0;

        bool operator==(const Entity& o) const { return id == o.id && gen == o.gen; }
        bool operator!=(const Entity& o) const { return !(*this == o); }
    };

    static constexpr Entity NullEntity{ 0, 0 };
}
