#pragma once
#include <DirectXMath.h>
#include <vector>

namespace Nuim::World {
	class Transform
	{
	public:
		Transform();

		Transform* GetParent() const { return m_parent; }

		const std::vector<Transform*>& GetChildren() const { return m_children; }

		void SetParent(Transform* newParent, bool keepWorld = true);

		void DetachFromParent();

		// --- Local getters/setters ---
		DirectX::XMFLOAT3 GetLocalPosition() const { return m_localPosition; }
		DirectX::XMFLOAT4 GetLocalRotation() const { return m_localRotation; }
		DirectX::XMFLOAT3 GetLocalScale()    const { return m_localScale; }


		void SetLocalPosition(const DirectX::XMFLOAT3& p);
		void SetLocalRotation(const DirectX::XMFLOAT4& q);
		void SetLocalScale(const DirectX::XMFLOAT3& s);

		DirectX::XMFLOAT3 GetPosition() const { return GetLocalPosition(); }
		DirectX::XMFLOAT4 GetRotation() const { return GetLocalRotation(); }
		DirectX::XMFLOAT3 GetScale()    const { return GetLocalScale(); }

		void SetPosition(const DirectX::XMFLOAT3& p) { SetLocalPosition(p); }
		void SetRotation(const DirectX::XMFLOAT4& q) { SetLocalRotation(q); }
		void SetScale(const DirectX::XMFLOAT3& s) { SetLocalScale(s); }

		// --- Matrices ---
		DirectX::XMMATRIX GetLocalMatrix();
		DirectX::XMMATRIX GetWorldMatrix();

		// --- Movement helpers ---
		void TranslateWorld(float dx, float dy, float dz);
		void TranslateLocal(float dx, float dy, float dz);
		void RotateAxisAngleLocal(const DirectX::XMFLOAT3& axis, float radians);

	private:
		void MarkDirty();
		void MarkDirtyWorldRecursive();

		void RebuildLocalMatrixIfNeeded();
		void RebuildWorldMatrixIfNeeded();

		void AddChild(Transform* t);
		void RemoveChild(Transform* t);

		void RebuildLocalMatrix();
	private:
		DirectX::XMFLOAT3 m_localPosition;
		DirectX::XMFLOAT4 m_localRotation; // quaternion
		DirectX::XMFLOAT3 m_localScale;

		Transform* m_parent = nullptr;
		std::vector<Transform*> m_children;

		bool m_dirtyLocal = true;
		bool m_dirtyWorld = true;

		DirectX::XMMATRIX m_cachedLocal;
		DirectX::XMMATRIX m_cachedWorld;
	};
}