#include "World/Scene.hpp"
#include "World/EntityHandle.hpp"
#include <DirectXMath.h>

using namespace DirectX;

namespace Nuim::World {

    EntityHandle Scene::CreateEntity(const std::string& name)
    {
        Entity e = m_registry.CreateEntity();

        m_registry.Emplace<UUIDComponent>(e);
        m_registry.Emplace<NameComponent>(e).name = name;
        m_registry.Emplace<TransformComponent>(e);

        return EntityHandle(this, e);
    }

    void Scene::_DetachScriptsIfRunning(Entity e)
    {
        if (!m_running) return;
        if (!m_registry.Has<ScriptComponent>(e)) return;

        auto& sc = m_registry.Get<ScriptComponent>(e);
        for (auto& s : sc.scripts)
        {
            if (!s) continue;
            if (s->m_attached)
            {
                s->OnDetach();
                s->_SetAttached(false);
            }
        }
    }

    void Scene::_DestroyRecursive(Entity e)
    {
        _DetachScriptsIfRunning(e);

        // destroy children first
        auto* trSet = m_registry.TryGetSet<TransformComponent>();
        if (trSet && trSet->Has(e))
        {
            auto& tr = trSet->Get(e);
            Entity ch = tr.firstChild;
            while (ch != NullEntity)
            {
                Entity next = trSet->Get(ch).nextSibling;
                _DestroyRecursive(ch);
                ch = next;
            }
        }

        // detach from parent (safe)
        if (trSet && trSet->Has(e))
            _DetachNoRecalc(e);

        // kill entity
        m_registry.DestroyEntity(e);
    }

    void Scene::DestroyEntity(Entity e, bool recursive)
    {
        if (!m_registry.IsAlive(e)) return;

        if (recursive)
        {
            _DestroyRecursive(e);
            return;
        }

        auto* trSet = m_registry.TryGetSet<TransformComponent>();
        if (trSet && trSet->Has(e))
        {
            auto& tr = trSet->Get(e);

            Entity ch = tr.firstChild;
            while (ch != NullEntity)
            {
                Entity next = trSet->Get(ch).nextSibling;
                SetParent(ch, NullEntity, true);
                ch = next;
            }

            _DetachNoRecalc(e);
        }

        _DetachScriptsIfRunning(e);

        m_registry.DestroyEntity(e);
    }

    Entity Scene::FindByUUID(const Nuim::UUID& uuid) const
    {
        auto* set = m_registry.TryGetSet<UUIDComponent>();
        if (!set) return NullEntity;

        const auto& ents = set->DenseEntities();
        const auto& data = set->DenseData();
        for (size_t i = 0; i < ents.size(); ++i)
            if (data[i].uuid == uuid)
                return ents[i];

        return NullEntity;
    }

    // ---- Hierarchy internals ----
    void Scene::_DetachNoRecalc(Entity child)
    {
        auto& trChild = m_registry.Get<TransformComponent>(child);
        Entity parent = trChild.parent;
        if (parent == NullEntity) return;

        auto& trParent = m_registry.Get<TransformComponent>(parent);

        if (trChild.prevSibling != NullEntity)
            m_registry.Get<TransformComponent>(trChild.prevSibling).nextSibling = trChild.nextSibling;
        if (trChild.nextSibling != NullEntity)
            m_registry.Get<TransformComponent>(trChild.nextSibling).prevSibling = trChild.prevSibling;

        if (trParent.firstChild == child)
            trParent.firstChild = trChild.nextSibling;

        trChild.parent = NullEntity;
        trChild.prevSibling = NullEntity;
        trChild.nextSibling = NullEntity;

        trParent.dirtyWorld = true;

        _MarkWorldDirtyRecursive(child);
    }

    void Scene::_AttachChild(Entity parent, Entity child)
    {
        auto& trParent = m_registry.Get<TransformComponent>(parent);
        auto& trChild = m_registry.Get<TransformComponent>(child);

        Entity oldFirst = trParent.firstChild;
        trParent.firstChild = child;

        trChild.parent = parent;
        trChild.prevSibling = NullEntity;
        trChild.nextSibling = oldFirst;

        if (oldFirst != NullEntity)
            m_registry.Get<TransformComponent>(oldFirst).prevSibling = child;

        trParent.dirtyWorld = true;

        _MarkWorldDirtyRecursive(child);
    }

    void Scene::_MarkWorldDirtyRecursive(Entity e)
    {
        auto& tr = m_registry.Get<TransformComponent>(e);
        tr.dirtyWorld = true;

        Entity ch = tr.firstChild;
        while (ch != NullEntity)
        {
            _MarkWorldDirtyRecursive(ch);
            ch = m_registry.Get<TransformComponent>(ch).nextSibling;
        }
    }

    Entity Scene::GetParent(Entity e) const
    {
        if (!m_registry.Has<TransformComponent>(e)) return NullEntity;
        return m_registry.Get<TransformComponent>(e).parent;
    }
    Entity Scene::GetFirstChild(Entity e) const
    {
        if (!m_registry.Has<TransformComponent>(e)) return NullEntity;
        return m_registry.Get<TransformComponent>(e).firstChild;
    }
    Entity Scene::GetNextSibling(Entity e) const
    {
        if (!m_registry.Has<TransformComponent>(e)) return NullEntity;
        return m_registry.Get<TransformComponent>(e).nextSibling;
    }

    void Scene::Detach(Entity child, bool keepWorld)
    {
        SetParent(child, NullEntity, keepWorld);
    }

    void Scene::SetParent(Entity child, Entity newParent, bool keepWorld)
    {
        if (!m_registry.IsAlive(child)) return;
        if (newParent != NullEntity && !m_registry.IsAlive(newParent)) return;
        if (child == newParent) return;

        // prevent cycles
        if (newParent != NullEntity)
        {
            for (Entity p = newParent; p != NullEntity; p = m_registry.Get<TransformComponent>(p).parent)
                if (p == child) return;
        }

        auto& trChild = m_registry.Get<TransformComponent>(child);

        XMMATRIX oldWorld = XMMatrixIdentity();
        if (keepWorld)
            oldWorld = GetWorldMatrix(child);

        if (trChild.parent != NullEntity)
            _DetachNoRecalc(child);

        if (newParent != NullEntity)
            _AttachChild(newParent, child);

        if (keepWorld)
        {
            XMMATRIX parentWorld = XMMatrixIdentity();
            if (newParent != NullEntity)
                parentWorld = GetWorldMatrix(newParent);

            XMMATRIX invParent = XMMatrixInverse(nullptr, parentWorld);
            XMMATRIX newLocal = oldWorld * invParent;

            XMVECTOR S, R, T;
            if (XMMatrixDecompose(&S, &R, &T, newLocal))
            {
                XMStoreFloat3(&trChild.localScale, S);
                XMStoreFloat4(&trChild.localRot, XMQuaternionNormalize(R));
                XMStoreFloat3(&trChild.localPos, T);
            }

            trChild.dirtyLocal = true;
        }

        _MarkWorldDirtyRecursive(child);
    }

    void Scene::_RebuildLocalIfNeeded(Entity e)
    {
        auto& tr = m_registry.Get<TransformComponent>(e);
        if (!tr.dirtyLocal) return;

        XMMATRIX S = XMMatrixScaling(tr.localScale.x, tr.localScale.y, tr.localScale.z);
        XMVECTOR q = XMLoadFloat4(&tr.localRot);
        q = XMQuaternionNormalize(q);
        XMMATRIX R = XMMatrixRotationQuaternion(q);
        XMMATRIX T = XMMatrixTranslation(tr.localPos.x, tr.localPos.y, tr.localPos.z);

        tr.cachedLocal = S * R * T;
        tr.dirtyLocal = false;
    }

    void Scene::_RebuildWorldIfNeeded(Entity e)
    {
        auto& tr = m_registry.Get<TransformComponent>(e);
        if (!tr.dirtyWorld) return;

        _RebuildLocalIfNeeded(e);

        if (tr.parent != NullEntity)
        {
            _RebuildWorldIfNeeded(tr.parent);
            tr.cachedWorld = tr.cachedLocal * m_registry.Get<TransformComponent>(tr.parent).cachedWorld;
        }
        else
        {
            tr.cachedWorld = tr.cachedLocal;
        }

        tr.dirtyWorld = false;
    }

    DirectX::XMMATRIX Scene::GetLocalMatrix(Entity e)
    {
        _RebuildLocalIfNeeded(e);
        return m_registry.Get<TransformComponent>(e).cachedLocal;
    }

    DirectX::XMMATRIX Scene::GetWorldMatrix(Entity e)
    {
        _RebuildWorldIfNeeded(e);
        return m_registry.Get<TransformComponent>(e).cachedWorld;
    }

    void Scene::MarkTransformDirty(Entity e)
    {
        if (!m_registry.Has<TransformComponent>(e)) return;
        auto& tr = m_registry.Get<TransformComponent>(e);
        tr.dirtyLocal = true;
        _MarkWorldDirtyRecursive(e);
    }

    // ---- Runtime ----
    void Scene::OnRuntimeStart()
    {
        m_running = true;

        View<ScriptComponent> v(m_registry);
        v.Each([this](Entity e, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                {
                    if (!s) continue;
                    if (!s->m_attached)
                    {
                        s->_Bind(this, e);
                        s->_SetAttached(true);
                        s->OnAttach();
                    }
                }
            });
    }

    void Scene::OnRuntimeStop()
    {
        View<ScriptComponent> v(m_registry);
        v.Each([&](Entity, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                {
                    if (!s) continue;
                    if (s->m_attached)
                    {
                        s->OnDetach();
                        s->_SetAttached(false);
                    }
                }
            });

        m_running = false;
    }

    void Scene::Update(float dt)
    {
        if (!m_running) return;

        View<ScriptComponent> v(m_registry);
        v.Each([&](Entity, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnUpdate(dt);
            });
    }

    void Scene::DispatchEvent(Nuim::Event& e)
    {
        if (!m_running) return;

        View<ScriptComponent> v(m_registry);
        v.Each([&](Entity, ScriptComponent& sc)
            {
                for (auto& s : sc.scripts)
                    if (s && s->m_attached)
                        s->OnEvent(e);
            });
    }

}
