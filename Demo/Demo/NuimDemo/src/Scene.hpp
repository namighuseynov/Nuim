#pragma once
#include <vector>
#include <memory>

#include "GameObject.hpp"

namespace NuimDemo {
	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		GameObject& CreateObject();

		void Update(float dt);
		
		void Draw();

		void Clear();

		size_t GetObjectCount() const { return m_objects.size(); }

	private:
		std::vector<std::unique_ptr<GameObject>> m_objects;

	};
}