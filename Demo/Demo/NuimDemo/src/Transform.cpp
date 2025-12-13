#include "NuimDemoPCH.h"
#include "Transform.hpp"

namespace NuimDemo {
	Transform::Transform() : m_position(0.0f, 0.0f, 0.0f),
		m_rotation(0.0f, 0.0f, 0.0f, 1.0f),
		m_scale(1.0f, 1.0f, 1.0f)
	{
		m_dirty = true;
		m_cachedLocal = DirectX::XMMatrixIdentity();
	}

	void Transform::MarkDirty()
	{
		m_dirty = true;
	}

	void Transform::RebuildLocalMatrix()
	{
		if (!m_dirty)
			return;

		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&m_rotation));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

		m_cachedLocal = S * R * T;

		m_dirty = false;
	}

	void NuimDemo::Transform::SetPosition(const DirectX::XMFLOAT3& p)
	{
		m_position = p;
		MarkDirty();
	}

	void NuimDemo::Transform::SetRotation(const DirectX::XMFLOAT4& q)
	{
		DirectX::XMVECTOR vq = DirectX::XMLoadFloat4(&q);
		vq = DirectX::XMQuaternionNormalize(vq);
		DirectX::XMStoreFloat4(&m_rotation, vq);

		MarkDirty();
	}

	void NuimDemo::Transform::SetScale(const DirectX::XMFLOAT3& s)
	{
		m_scale = s;
		MarkDirty(); 
	}

	void Transform::TranslateWorld(float dx, float dy, float dz)
	{
		m_position.x += dx;
		m_position.y += dy;
		m_position.z += dz; 

		MarkDirty(); 
	}

	void Transform::TranslateLocal(float dx, float dy, float dz)
	{
		DirectX::XMVECTOR localOffset = DirectX::XMVectorSet(dx, dy, dz, 0.0f);

		DirectX::XMVECTOR q = DirectX::XMLoadFloat4(&m_rotation);

		DirectX::XMVECTOR worldOffset = DirectX::XMVector3Rotate(localOffset, q);

		DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&m_position);
		pos = DirectX::XMVectorAdd(pos, worldOffset); 
		XMStoreFloat3(&m_position, pos);

		MarkDirty();
	}

	void Transform::RotateAxisAngleLocal(const DirectX::XMFLOAT3& axis, float radians)
	{
		DirectX::XMVECTOR qCurrent = DirectX::XMLoadFloat4(&m_rotation);

		DirectX::XMVECTOR vAxis = DirectX::XMLoadFloat3(&axis);
		vAxis = DirectX::XMVector3Normalize(vAxis);

		DirectX::XMVECTOR qDelta = DirectX::XMQuaternionRotationAxis(vAxis, radians);

		DirectX::XMVECTOR qNew = DirectX::XMQuaternionMultiply(qCurrent, qDelta);
		qNew = DirectX::XMQuaternionNormalize(qNew);

		DirectX::XMStoreFloat4(&m_rotation, qNew);
		MarkDirty();
	}

	DirectX::XMMATRIX Transform::GetLocalMatrix()
	{
		RebuildLocalMatrix();
		return m_cachedLocal;
	}

	DirectX::XMMATRIX Transform::GetWorldMatrix()
	{
		RebuildLocalMatrix();
		return m_cachedLocal; 
	}

}

