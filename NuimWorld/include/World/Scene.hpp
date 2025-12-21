#pragma once
#include "World/ECS/Registry.hpp"
#include "World/ECS/View.hpp"
#include "World/ECS/Entity.hpp"

#include "World/Components/UUIDComponent.hpp"
#include "World/Components/NameComponent.hpp"
#include "World/Components/TransformComponent.hpp"
#include "World/Components/ScriptComponent.hpp"

#include "Core/Events/Event.hpp"
#include <DirectXMath.h>

#include <string>
#include <type_traits>
#include <stdexcept>

namespace Nuim::World {

    class EntityHandle; // forward

    class Scene
    {
    public:
        Scene() = default;

        // ---- Entities ----
        EntityHandle CreateEntity(const std::string& name = "Entity");
        void DestroyEntity(Entity e, bool recursive = true);

        bool IsAlive(Entity e) const { return m_registry.IsAlive(e); }

        Entity FindByUUID(const Nuim::UUID& uuid) const;

        Registry& GetRegistry() { return m_registry; }
        const Registry& GetRegistry() const { return m_registry; }

        // ---- Hierarchy API ----
        void SetParent(Entity child, Entity newParent, bool keepWorld = true);
        void Detach(Entity child, bool keepWorld = true);

        Entity GetParent(Entity e) const;
        Entity GetFirstChild(Entity e) const;
        Entity GetNextSibling(Entity e) const;

        // ---- Transform API ----
        DirectX::XMMATRIX GetLocalMatrix(Entity e);
        DirectX::XMMATRIX GetWorldMatrix(Entity e);

        void MarkTransformDirty(Entity e);

        // ---- Scripts API ----
        template<typename T, typename... Args>
        T& AddScript(Entity e, Args&&... args)
        {
            static_assert(std::is_base_of_v<ScriptBase, T>, "T must derive from ScriptBase");
            if (!m_registry.IsAlive(e))
                throw std::runtime_error("AddScript: entity not alive");

            ScriptComponent* sc = nullptr;
            if (!m_registry.Has<ScriptComponent>(e))
                sc = &m_registry.Emplace<ScriptComponent>(e);
            else
                sc = &m_registry.Get<ScriptComponent>(e);

            auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
            T& ref = *ptr;

            if (m_running)
            {
                ptr->_Bind(this, e);
                ptr->_SetAttached(true);
                ptr->OnAttach();
            }

            sc->scripts.emplace_back(std::move(ptr));
            return ref;
        }

        // ---- Runtime ----
        void OnRuntimeStart();
        void OnRuntimeStop();

        bool IsRunning() const { return m_running; }

        void Update(float dt);
        void DispatchEvent(Nuim::Event& e);

    private:
        // hierarchy internals
        void _DetachNoRecalc(Entity child);
        void _AttachChild(Entity parent, Entity child);

        void _MarkWorldDirtyRecursive(Entity e);

        void _RebuildLocalIfNeeded(Entity e);
        void _RebuildWorldIfNeeded(Entity e);

        void _DestroyRecursive(Entity e);
        void _DetachScriptsIfRunning(Entity e);

    private:
        Registry m_registry;
        bool m_running = false;
    };

}
