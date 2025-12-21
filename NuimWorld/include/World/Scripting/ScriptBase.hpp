#pragma once
#include "World/ECS/Entity.hpp"
#include "Core/Events/Event.hpp"

namespace Nuim::World {

    class World; 

    class ScriptBase
    {
    public:
        virtual ~ScriptBase() = default;

        Entity GetEntity() const { return m_entity; }
        World* GetWorld()  const { return m_world; }

        bool IsAttached() const { return m_attached; }

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnEvent(Nuim::Event& e) {}
        virtual void OnImGuiRender() {}

    private:
        void _Bind(World* world, Entity e) { m_world = world; m_entity = e; }
        void _SetAttached(bool a) { m_attached = a; }

    private:
        World* m_world = nullptr;
        Entity  m_entity = NullEntity;
        bool    m_attached = false;

        friend class ScriptSystem;
    };

} 
