#pragma once

namespace Nuim::World {

    class World;

    class IWorldRenderer
    {
    public:
        virtual ~IWorldRenderer() = default;
        virtual void Render(World& world) = 0;
    };

}
