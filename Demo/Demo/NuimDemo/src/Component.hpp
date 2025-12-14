#pragma once

namespace Nuim {
	class GameObject;

	class RenderQueue; 

	class Component
	{
	public:
		virtual ~Component() = default;

		virtual const char* GetTypeName() const { return "Component"; }

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(bool v) { m_enabled = v; }

		virtual void OnCreate() {}
		virtual void Update(float dt) {}
		virtual void LateUpdate(float dt) {}
		virtual void Draw() {}
		virtual void Submit(RenderQueue& q) {}


	protected:
		GameObject* m_owner = nullptr;

	private:
		bool m_enabled = true;
		friend class GameObject;
	};
}