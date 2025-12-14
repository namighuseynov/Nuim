#pragma once
#include <DirectXMath.h>

namespace Nuim {
	class Transform
	{
	public:
		Transform();
		void SetPosition(const DirectX::XMFLOAT3& position);
		void SetRotation(const DirectX::XMFLOAT4& rotation);
		void SetScale(const DirectX::XMFLOAT3& scale);

		void TranslateWorld(float dx, float dy, float dz);
		void TranslateLocal(float dx, float dy, float dz);

		void RotateAxisAngleLocal(const DirectX::XMFLOAT3& axis, float radians);

		// Matrices
		DirectX::XMMATRIX GetLocalMatrix();
		DirectX::XMMATRIX GetWorldMatrix();

		// Getters
		DirectX::XMFLOAT3 GetPosition() const { return m_position; }
		DirectX::XMFLOAT4 GetRotation() const { return m_rotation; }
		DirectX::XMFLOAT3 GetScale()	const { return m_scale; }
	private:
		void MarkDirty();
		void RebuildLocalMatrix();
	private:
		DirectX::XMFLOAT3 m_position;	// Vector
		DirectX::XMFLOAT4 m_rotation;	// Quaternions
		DirectX::XMFLOAT3 m_scale;		// Vector

		bool m_dirty = true;
		DirectX::XMMATRIX m_cachedLocal = DirectX::XMMatrixIdentity();
	};
}