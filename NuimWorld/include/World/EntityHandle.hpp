#pragma once
#include <cassert>
#include "World/ECS/Entity.hpp"

namespace Nuim::World {

    class Scene; // forward

    class EntityHandle
    {
    public:
        EntityHandle() = default;
        EntityHandle(Scene* scene, Entity e) : m_scene(scene), m_e(e) {}

        Entity Raw() const { return m_e; }
        bool IsValid() const;

        template<typename T>
        bool Has() const;

        template<typename T>
        T& Get();

        template<typename T, typename... Args>
        T& Add(Args&&... args);

        template<typename T>
        void Remove();

    private:
        Scene* m_scene = nullptr;
        Entity m_e = NullEntity;
    };

} // namespace Nuim::World


// ---- inline + template impl ----
#include "World/Scene.hpp"
#include <utility>

namespace Nuim::World {

    inline bool EntityHandle::IsValid() const
    {
        return m_scene && m_scene->IsAlive(m_e);
    }

    template<typename T>
    bool EntityHandle::Has() const
    {
        return IsValid() && m_scene->GetRegistry().Has<T>(m_e);
    }

    template<typename T>
    T& EntityHandle::Get()
    {
        assert(IsValid() && "EntityHandle::Get on invalid handle");
        return m_scene->GetRegistry().Get<T>(m_e);
    }

    template<typename T, typename... Args>
    T& EntityHandle::Add(Args&&... args)
    {
        assert(IsValid() && "EntityHandle::Add on invalid handle");
        return m_scene->GetRegistry().Emplace<T>(m_e, std::forward<Args>(args)...);
    }

    template<typename T>
    void EntityHandle::Remove()
    {
        assert(IsValid() && "EntityHandle::Remove on invalid handle");
        m_scene->GetRegistry().Remove<T>(m_e);
    }

} 
