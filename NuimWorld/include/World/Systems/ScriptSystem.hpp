#pragma once
#include "World/ECS/Registry.hpp"
#include "World/Components/ScriptComponent.hpp"
#include "World/ECS/View.hpp"
#include "World/World.hpp"

namespace Nuim::World {

    class ScriptSystem
    {
    public:
        ScriptSystem(Registry& r, World& w) : m_r(r), m_w(w) {}

        void OnRuntimeStart();
        void OnRuntimeStop();

        void Update(float dt);
        void FixedUpdate(float fixedDt);
        void DispatchEvent(Nuim::Event& e);

        void OnEntityDestroy(Entity e);

        void OnEntityActiveChanged(Entity e, bool active);

    private:
        void DetachIfRunning(Entity e);
        void AttachIfRunning(Entity e);

    private:
        Registry& m_r;
        World& m_w;
    };

}
