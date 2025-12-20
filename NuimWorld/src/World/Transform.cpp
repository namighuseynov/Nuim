#include "World/Transform.hpp"
#include <algorithm>

using namespace DirectX;

namespace Nuim::World {

    Transform::Transform()
        : m_localPosition(0, 0, 0),
        m_localRotation(0, 0, 0, 1),
        m_localScale(1, 1, 1)
    {
        m_cachedLocal = XMMatrixIdentity();
        m_cachedWorld = XMMatrixIdentity();
        m_dirtyLocal = true;
        m_dirtyWorld = true;
    }

    void Transform::MarkDirty()
    {
        m_dirtyLocal = true;
        m_dirtyWorld = true;
        MarkDirtyWorldRecursive();
    }

    void Transform::MarkDirtyWorldRecursive()
    {
        for (auto* ch : m_children)
        {
            if (!ch) continue;
            ch->m_dirtyWorld = true;
            ch->MarkDirtyWorldRecursive();
        }
    }

    void Transform::RebuildLocalMatrixIfNeeded()
    {
        if (!m_dirtyLocal) return;

        XMMATRIX S = XMMatrixScaling(m_localScale.x, m_localScale.y, m_localScale.z);
        XMVECTOR q = XMLoadFloat4(&m_localRotation);
        q = XMQuaternionNormalize(q);
        XMMATRIX R = XMMatrixRotationQuaternion(q);
        XMMATRIX T = XMMatrixTranslation(m_localPosition.x, m_localPosition.y, m_localPosition.z);

        m_cachedLocal = S * R * T;

        m_dirtyLocal = false;
    }

    void Transform::RebuildWorldMatrixIfNeeded()
    {
        if (!m_dirtyWorld) return;

        RebuildLocalMatrixIfNeeded();

        if (m_parent)
        {
            m_cachedWorld = m_cachedLocal * m_parent->GetWorldMatrix();
        }
        else
        {
            m_cachedWorld = m_cachedLocal;
        }

        m_dirtyWorld = false;
    }

    XMMATRIX Transform::GetLocalMatrix()
    {
        RebuildLocalMatrixIfNeeded();
        return m_cachedLocal;
    }

    XMMATRIX Transform::GetWorldMatrix()
    {
        RebuildWorldMatrixIfNeeded();
        return m_cachedWorld;
    }

    void Transform::SetLocalPosition(const XMFLOAT3& p)
    {
        m_localPosition = p;
        MarkDirty();
    }

    void Transform::SetLocalRotation(const XMFLOAT4& q)
    {
        XMVECTOR vq = XMLoadFloat4(&q);
        vq = XMQuaternionNormalize(vq);
        XMStoreFloat4(&m_localRotation, vq);
        MarkDirty();
    }

    void Transform::SetLocalScale(const XMFLOAT3& s)
    {
        m_localScale = s;
        MarkDirty();
    }

    void Transform::AddChild(Transform* t)
    {
        if (!t) return;
        m_children.push_back(t);
    }

    void Transform::RemoveChild(Transform* t)
    {
        auto it = std::find(m_children.begin(), m_children.end(), t);
        if (it != m_children.end())
            m_children.erase(it);
    }

    void Transform::DetachFromParent()
    {
        if (!m_parent) return;
        m_parent->RemoveChild(this);
        m_parent = nullptr;
        MarkDirty();
    }

    void Transform::SetParent(Transform* newParent, bool keepWorld)
    {
        if (newParent == this) return;
        if (newParent == m_parent) return;

        for (Transform* p = newParent; p; p = p->m_parent)
            if (p == this) return;

        XMMATRIX oldWorld = keepWorld ? GetWorldMatrix() : XMMatrixIdentity();

        // detach from old
        if (m_parent)
            m_parent->RemoveChild(this);

        m_parent = newParent;

        if (m_parent)
            m_parent->AddChild(this);

        if (keepWorld)
        {
            XMMATRIX parentWorld = m_parent ? m_parent->GetWorldMatrix() : XMMatrixIdentity();
            XMMATRIX invParent = XMMatrixInverse(nullptr, parentWorld);

            // local = world * inv(parentWorld)
            XMMATRIX newLocal = oldWorld * invParent;

            XMVECTOR S, R, T;
            if (XMMatrixDecompose(&S, &R, &T, newLocal))
            {
                XMStoreFloat3(&m_localScale, S);
                XMStoreFloat4(&m_localRotation, R);
                XMStoreFloat3(&m_localPosition, T);
            }
        }

        MarkDirty();
    }

    void Transform::TranslateLocal(float dx, float dy, float dz)
    {
        XMVECTOR localOffset = XMVectorSet(dx, dy, dz, 0.0f);
        XMVECTOR q = XMLoadFloat4(&m_localRotation);
        XMVECTOR worldOffset = XMVector3Rotate(localOffset, q);

        XMVECTOR pos = XMLoadFloat3(&m_localPosition);
        pos = XMVectorAdd(pos, worldOffset);
        XMStoreFloat3(&m_localPosition, pos);

        MarkDirty();
    }

    void Transform::TranslateWorld(float dx, float dy, float dz)
    {
        XMVECTOR worldOffset = XMVectorSet(dx, dy, dz, 0.0f);

        if (m_parent)
        {
            XMMATRIX parentWorld = m_parent->GetWorldMatrix();
            XMMATRIX invParent = XMMatrixInverse(nullptr, parentWorld);

            XMVECTOR localOffset = XMVector3TransformNormal(worldOffset, invParent);

            XMVECTOR pos = XMLoadFloat3(&m_localPosition);
            pos = XMVectorAdd(pos, localOffset);
            XMStoreFloat3(&m_localPosition, pos);
        }
        else
        {
            XMVECTOR pos = XMLoadFloat3(&m_localPosition);
            pos = XMVectorAdd(pos, worldOffset);
            XMStoreFloat3(&m_localPosition, pos);
        }

        MarkDirty();
    }

    void Transform::RotateAxisAngleLocal(const XMFLOAT3& axis, float radians)
    {
        XMVECTOR qCurrent = XMLoadFloat4(&m_localRotation);

        XMVECTOR vAxis = XMLoadFloat3(&axis);
        vAxis = XMVector3Normalize(vAxis);

        XMVECTOR qDelta = XMQuaternionRotationAxis(vAxis, radians);

        XMVECTOR qNew = XMQuaternionMultiply(qCurrent, qDelta);
        qNew = XMQuaternionNormalize(qNew);

        XMStoreFloat4(&m_localRotation, qNew);
        MarkDirty();
    }

}
