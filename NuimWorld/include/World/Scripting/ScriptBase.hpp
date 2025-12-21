#pragma once
#include "World/ECS/Entity.hpp"
#include "Core/Events/Event.hpp"

namespace Nuim::World {

    class Scene;

    class ScriptBase
    {
    public:
        virtual ~ScriptBase() = default;

        Entity GetEntity() const { return m_entity; }
        Scene* GetScene()  const { return m_scene; }

        bool IsAttached() const { return m_attached; }

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float dt) {}
        virtual void OnEvent(Nuim::Event& e) {}
        virtual void OnImGuiRender() {}

    private:
        void _Bind(Scene* scene, Entity e) { m_scene = scene; m_entity = e; }
        void _SetAttached(bool a) { m_attached = a; }

    private:
        Scene* m_scene = nullptr;
        Entity m_entity = NullEntity;
        bool m_attached = false;

        friend class Scene;
    };

} // namespace Nuim::World
