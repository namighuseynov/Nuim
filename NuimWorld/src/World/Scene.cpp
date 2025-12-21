#include "World/Scene.hpp"
#include "World/EntityHandle.hpp"

#include <DirectXMath.h>
#include <cmath>

using namespace DirectX;

namespace Nuim::World {

    // --------- small helpers ----------
    static inline float AbsF(float x) { return x < 0.0f ? -x : x; }

    // Decompose affine matrix into Translation + Rotation + (Scale+Shear)
    // Row-vector convention (consistent with your SS*R*T build).
    static bool DecomposeAffine_RowScaleShearRotTrans(
        const DirectX::XMMATRIX& M,
        DirectX::XMFLOAT3& outPos,
        DirectX::XMFLOAT4& outRot,
        DirectX::XMFLOAT3& outScale,
        DirectX::XMFLOAT3& outShear)
    {
        // translation in r[3]
        XMStoreFloat3(&outPos, M.r[3]);

        XMVECTOR row0 = XMVectorSetW(M.r[0], 0.0f);
        XMVECTOR row1 = XMVectorSetW(M.r[1], 0.0f);
        XMVECTOR row2 = XMVectorSetW(M.r[2], 0.0f);

        const float eps = 1e-8f;

        float sx = XMVectorGetX(XMVector3Length(row0));
        if (sx < eps) return false;
        XMVECTOR r0 = XMVectorScale(row0, 1.0f / sx);

        float shXY = XMVectorGetX(XMVector3Dot(r0, row1));
        row1 = XMVectorSubtract(row1, XMVectorScale(r0, shXY));

        float sy = XMVectorGetX(XMVector3Length(row1));
        if (sy < eps) return false;
        XMVECTOR r1 = XMVectorScale(row1, 1.0f / sy);
        shXY /= sy;

        float shXZ = XMVectorGetX(XMVector3Dot(r0, row2));
        row2 = XMVectorSubtract(row2, XMVectorScale(r0, shXZ));

        float shYZ = XMVectorGetX(XMVector3Dot(r1, row2));
        row2 = XMVectorSubtract(row2, XMVectorScale(r1, shYZ));

        float sz = XMVectorGetX(XMVector3Length(row2));
        if (sz < eps) return false;
        XMVECTOR r2 = XMVectorScale(row2, 1.0f / sz);
        shXZ /= sz;
        shYZ /= sz;

        // handedness fix
        float det = XMVectorGetX(XMVector3Dot(r0, XMVector3Cross(r1, r2)));
        if (det < 0.0f)
        {
            float ax = AbsF(sx), ay = AbsF(sy), az = AbsF(sz);

            if (ax >= ay && ax >= az)
            {
                sx = -sx;
                r0 = XMVectorNegate(r0);
                shXY = -shXY;
                shXZ = -shXZ;
            }
            else if (ay >= ax && ay >= az)
            {
                sy = -sy;
                r1 = XMVectorNegate(r1);
                shYZ = -shYZ;
            }
            else
            {
                sz = -sz;
                r2 = XMVectorNegate(r2);
            }
        }

        XMMATRIX R = XMMatrixIdentity();
        R.r[0] = XMVectorSetW(r0, 0.0f);
        R.r[1] = XMVectorSetW(r1, 0.0f);
        R.r[2] = XMVectorSetW(r2, 0.0f);

        XMVECTOR q = XMQuaternionRotationMatrix(R);
        q = XMQuaternionNormalize(q);
        XMStoreFloat4(&outRot, q);

        outScale = XMFLOAT3(sx, sy, sz);
        outShear = XMFLOAT3(shXY, shXZ, shYZ);
        return true;
    }

    static DirectX::XMMATRIX BuildScaleShearMatrix_Row(
        const DirectX::XMFLOAT3& scale,
        const DirectX::XMFLOAT3& shear)
    {
        const float sx = scale.x, sy = scale.y, sz = scale.z;
        const float shXY = shear.x;
        const float shXZ = shear.y;
        const float shYZ = shear.z;

        XMMATRIX SS = XMMatrixIdentity();
        SS.r[0] = XMVectorSet(sx, 0.0f, 0.0f, 0.0f);
        SS.r[1] = XMVectorSet(shXY * sy, sy, 0.0f, 0.0f);
        SS.r[2] = XMVectorSet(shXZ * sz, shYZ * sz, sz, 0.0f);
        SS.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
        return SS;
    }

    // ---------------- Scene ----------------

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

        if (trSet && trSet->Has(e))
            _DetachNoRecalc(e);

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

    // ---- hierarchy internals ----
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

        // IMPORTANT: whole subtree changes reference frame
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

        // IMPORTANT: whole subtree must update world
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

            XMVECTOR det;
            XMMATRIX invParent = XMMatrixInverse(&det, parentWorld);

            const float detX = XMVectorGetX(det);
            if (AbsF(detX) > 1e-8f)
            {
                XMMATRIX newLocal = oldWorld * invParent;

                XMFLOAT3 pos;
                XMFLOAT4 rot;
                XMFLOAT3 scl;
                XMFLOAT3 shr;

                if (DecomposeAffine_RowScaleShearRotTrans(newLocal, pos, rot, scl, shr))
                {
                    trChild.localPos = pos;
                    trChild.localRot = rot;
                    trChild.localScale = scl;
                    trChild.shear = shr;
                    trChild.dirtyLocal = true;
                }
            }
            // else parent is singular -> exact keepWorld impossible without full matrix storage
        }

        _MarkWorldDirtyRecursive(child);
    }

    // ---- transform rebuild ----
    void Scene::_RebuildLocalIfNeeded(Entity e)
    {
        auto& tr = m_registry.Get<TransformComponent>(e);
        if (!tr.dirtyLocal) return;

        DirectX::XMMATRIX SS = BuildScaleShearMatrix_Row(tr.localScale, tr.shear);

        DirectX::XMVECTOR q = DirectX::XMLoadFloat4(&tr.localRot);
        q = DirectX::XMQuaternionNormalize(q);
        DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(q);

        DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(tr.localPos.x, tr.localPos.y, tr.localPos.z);

        DirectX::XMMATRIX local = SS * R * T;
        DirectX::XMStoreFloat4x4(&tr.cachedLocal, local);

        tr.dirtyLocal = false;
    }

    void Scene::_RebuildWorldIfNeeded(Entity e)
    {
        auto& tr = m_registry.Get<TransformComponent>(e);
        if (!tr.dirtyWorld) return;

        _RebuildLocalIfNeeded(e);

        DirectX::XMMATRIX local = DirectX::XMLoadFloat4x4(&tr.cachedLocal);

        if (tr.parent != NullEntity)
        {
            _RebuildWorldIfNeeded(tr.parent);

            const auto& trParent = m_registry.Get<TransformComponent>(tr.parent);
            DirectX::XMMATRIX parentWorld = DirectX::XMLoadFloat4x4(&trParent.cachedWorld);

            DirectX::XMMATRIX world = local * parentWorld;
            DirectX::XMStoreFloat4x4(&tr.cachedWorld, world);
        }
        else
        {
            DirectX::XMStoreFloat4x4(&tr.cachedWorld, local);
        }

        tr.dirtyWorld = false;
    }

    DirectX::XMMATRIX Scene::GetLocalMatrix(Entity e)
    {
        _RebuildLocalIfNeeded(e);
        return DirectX::XMLoadFloat4x4(&m_registry.Get<TransformComponent>(e).cachedLocal);
    }

    DirectX::XMMATRIX Scene::GetWorldMatrix(Entity e)
    {
        _RebuildWorldIfNeeded(e);
        return DirectX::XMLoadFloat4x4(&m_registry.Get<TransformComponent>(e).cachedWorld);
    }

    void Scene::MarkTransformDirty(Entity e)
    {
        if (!m_registry.Has<TransformComponent>(e)) return;
        auto& tr = m_registry.Get<TransformComponent>(e);
        tr.dirtyLocal = true;
        _MarkWorldDirtyRecursive(e);
    }

    // ---- runtime ----
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
