#include "World/Systems/TransformSystem.hpp"
#include <vector>
#include <cmath>
#include <cassert>

using namespace DirectX;

namespace Nuim::World {

    static inline float AbsF(float x) { return x < 0.0f ? -x : x; }

    static bool DecomposeAffine_RowScaleShearRotTrans(
        const XMMATRIX& M,
        XMFLOAT3& outPos,
        XMFLOAT4& outRot,
        XMFLOAT3& outScale,
        XMFLOAT3& outShear)
    {
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

        float det = XMVectorGetX(XMVector3Dot(r0, XMVector3Cross(r1, r2)));
        if (det < 0.0f)
        {
            float ax = AbsF(sx), ay = AbsF(sy), az = AbsF(sz);

            if (ax >= ay && ax >= az)
            {
                sx = -sx; r0 = XMVectorNegate(r0);
                shXY = -shXY; shXZ = -shXZ;
            }
            else if (ay >= ax && ay >= az)
            {
                sy = -sy; r1 = XMVectorNegate(r1);
                shYZ = -shYZ;
            }
            else
            {
                sz = -sz; r2 = XMVectorNegate(r2);
            }
        }

        XMMATRIX R = XMMatrixIdentity();
        R.r[0] = XMVectorSetW(r0, 0.0f);
        R.r[1] = XMVectorSetW(r1, 0.0f);
        R.r[2] = XMVectorSetW(r2, 0.0f);

        XMVECTOR q = XMQuaternionNormalize(XMQuaternionRotationMatrix(R));
        XMStoreFloat4(&outRot, q);

        outScale = XMFLOAT3(sx, sy, sz);
        outShear = XMFLOAT3(shXY, shXZ, shYZ);
        return true;
    }

    static XMMATRIX BuildScaleShearMatrix_Row(const XMFLOAT3& scale, const XMFLOAT3& shear)
    {
        const float sx = scale.x, sy = scale.y, sz = scale.z;
        const float shXY = shear.x;
        const float shXZ = shear.y;
        const float shYZ = shear.z;

        XMMATRIX SS = XMMatrixIdentity();
        SS.r[0] = XMVectorSet(sx, 0, 0, 0);
        SS.r[1] = XMVectorSet(shXY * sy, sy, 0, 0);
        SS.r[2] = XMVectorSet(shXZ * sz, shYZ * sz, sz, 0);
        SS.r[3] = XMVectorSet(0, 0, 0, 1);
        return SS;
    }

#if defined(_DEBUG) || defined(DEBUG)
    static float MaxAbsDiff4x4(const XMMATRIX& A, const XMMATRIX& B)
    {
        XMFLOAT4X4 a, b;
        XMStoreFloat4x4(&a, A);
        XMStoreFloat4x4(&b, B);

        float m = 0.0f;
        const float* pa = &a.m[0][0];
        const float* pb = &b.m[0][0];
        for (int i = 0; i < 16; ++i)
        {
            float d = AbsF(pa[i] - pb[i]);
            if (d > m) m = d;
        }
        return m;
    }

    void TransformSystem::DebugValidateKeepWorld(Entity child, Entity newParent, const XMMATRIX& oldWorld)
    {
        auto& tr = m_r.Get<TransformComponent>(child);

        XMMATRIX SS = BuildScaleShearMatrix_Row(tr.localScale, tr.shear);
        XMVECTOR q = XMQuaternionNormalize(XMLoadFloat4(&tr.localRot));
        XMMATRIX R = XMMatrixRotationQuaternion(q);
        XMMATRIX T = XMMatrixTranslation(tr.localPos.x, tr.localPos.y, tr.localPos.z);
        XMMATRIX local = SS * R * T;

        XMMATRIX parentWorld = XMMatrixIdentity();
        if (newParent != NullEntity)
            parentWorld = GetWorldMatrix(newParent);

        XMMATRIX recomposed = local * parentWorld;

        // tolerance depends on your scale; keep it reasonable
        const float err = MaxAbsDiff4x4(recomposed, oldWorld);
        // If this trips, it usually means order/inverse mismatch or decomposition instability
        assert(err < 1e-3f && "keepWorld validation failed: recomposed world differs from oldWorld");
    }
#endif

    void TransformSystem::RebuildLocalIfNeeded(Entity e)
    {
        auto& tr = m_r.Get<TransformComponent>(e);
        if (!tr.dirtyLocal) return;

        XMMATRIX SS = BuildScaleShearMatrix_Row(tr.localScale, tr.shear);

        XMVECTOR q = XMQuaternionNormalize(XMLoadFloat4(&tr.localRot));
        XMMATRIX R = XMMatrixRotationQuaternion(q);

        XMMATRIX T = XMMatrixTranslation(tr.localPos.x, tr.localPos.y, tr.localPos.z);

        XMMATRIX local = SS * R * T;
        XMStoreFloat4x4(&tr.cachedLocal, local);

        tr.dirtyLocal = false;
    }

    // NEW: non-recursive ensure world for entity + required parents
    void TransformSystem::EnsureWorldUpToDate_NoRecursion(Entity e)
    {
        if (!m_r.Has<TransformComponent>(e)) return;

        // Build chain from e up to root
        std::vector<Entity> chain;
        chain.reserve(64);

        Entity cur = e;
        while (cur != NullEntity && m_r.Has<TransformComponent>(cur))
        {
            chain.push_back(cur);
            const auto& tr = m_r.Get<TransformComponent>(cur);
            cur = tr.parent;
        }

        // Walk from root to child, rebuilding only dirty nodes
        for (size_t i = chain.size(); i-- > 0; )
        {
            Entity node = chain[i];
            auto& tr = m_r.Get<TransformComponent>(node);

            if (!tr.dirtyWorld && !tr.dirtyLocal)
                continue;

            RebuildLocalIfNeeded(node);
            XMMATRIX local = XMLoadFloat4x4(&tr.cachedLocal);

            if (tr.parent != NullEntity && m_r.Has<TransformComponent>(tr.parent))
            {
                // Parent is earlier in chain (closer to root); it will be processed already
                auto& p = m_r.Get<TransformComponent>(tr.parent);
                // ensure parent's cachedWorld is valid; if parent was not dirty, it remains valid
                XMMATRIX parentWorld = XMLoadFloat4x4(&p.cachedWorld);
                XMStoreFloat4x4(&tr.cachedWorld, local * parentWorld);
            }
            else
            {
                XMStoreFloat4x4(&tr.cachedWorld, local);
            }

            tr.dirtyWorld = false;
        }
    }

    XMMATRIX TransformSystem::GetLocalMatrix(Entity e)
    {
        RebuildLocalIfNeeded(e);
        return XMLoadFloat4x4(&m_r.Get<TransformComponent>(e).cachedLocal);
    }

    XMMATRIX TransformSystem::GetWorldMatrix(Entity e)
    {
        EnsureWorldUpToDate_NoRecursion(e);
        return XMLoadFloat4x4(&m_r.Get<TransformComponent>(e).cachedWorld);
    }

    void TransformSystem::MarkDirty(Entity e)
    {
        if (!m_r.Has<TransformComponent>(e)) return;
        auto& tr = m_r.Get<TransformComponent>(e);
        tr.dirtyLocal = true;
        m_h.MarkWorldDirtySubtree(e);
    }

    void TransformSystem::SetParent(Entity child, Entity newParent, bool keepWorld)
    {
        if (!m_r.IsAlive(child)) return;
        if (!m_r.Has<TransformComponent>(child)) return;

        if (newParent != NullEntity && !m_r.IsAlive(newParent)) return;
        if (newParent != NullEntity && !m_r.Has<TransformComponent>(newParent)) return;

        if (child == newParent) return;

        // save old parent for rollback
        Entity oldParent = m_r.Get<TransformComponent>(child).parent;

        XMMATRIX oldWorld = XMMatrixIdentity();
        if (keepWorld)
            oldWorld = GetWorldMatrix(child);

        // change hierarchy first (your design)
        m_h.SetParentRaw(child, newParent);

        if (!keepWorld) return;

        XMMATRIX parentWorld = XMMatrixIdentity();
        if (newParent != NullEntity)
            parentWorld = GetWorldMatrix(newParent);

        XMVECTOR det;
        XMMATRIX invParent = XMMatrixInverse(&det, parentWorld);
        const float detX = XMVectorGetX(det);

        // If parent is singular -> exact keepWorld impossible => rollback
        if (AbsF(detX) <= 1e-8f)
        {
            m_h.SetParentRaw(child, oldParent);
            return;
        }

        // Convention: world = local * parentWorld  => local = world * inverse(parentWorld)
        XMMATRIX newLocal = oldWorld * invParent;

        XMFLOAT3 pos;
        XMFLOAT4 rot;
        XMFLOAT3 scl;
        XMFLOAT3 shr;

        if (!DecomposeAffine_RowScaleShearRotTrans(newLocal, pos, rot, scl, shr))
        {
            // decomposition failed -> rollback
            m_h.SetParentRaw(child, oldParent);
            return;
        }

        auto& tr = m_r.Get<TransformComponent>(child);
        tr.localPos = pos;
        tr.localRot = rot;
        tr.localScale = scl;
        tr.shear = shr;

        tr.dirtyLocal = true;
        m_h.MarkWorldDirtySubtree(child);

#if defined(_DEBUG) || defined(DEBUG)
        DebugValidateKeepWorld(child, newParent, oldWorld);
#endif
    }

}
