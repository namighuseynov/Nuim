#pragma once
#include "GameObject.hpp"

namespace NuimDemo {

	class Component
	{
	public:
		virtual ~Component() = default;

		GameObject* GetOwner() const { return m_owner; }

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(bool v) { m_enabled = v; }

		virtual void OnCreate() {}
		virtual void Update(float dt) {}
		virtual void Draw() {}

	protected:
		GameObject* m_owner = nullptr;

	private:
		bool m_enabled = true;

		friend class GameObject;
	};
}