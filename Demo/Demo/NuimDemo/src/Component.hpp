#pragma once

namespace NuimDemo {
	class GameObject;

	class Component
	{
	public:

	protected:
		GameObject* m_owner = nullptr;

	private:
		bool m_enabled = true;

		friend class GameObject;
	};
}