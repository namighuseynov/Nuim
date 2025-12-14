#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>
#include "Transform.hpp"
#include "Component.hpp"

namespace NuimDemo {
	class GameObject
	{
	public:
		Transform transform;

	public:
		GameObject() = default;
		~GameObject() = default;

		template<typename T, typename ... Args>
		T* AddComponent(Args&&... args) {
			static_assert(std::is_base_of_v<Component, T>, "T must be derived from Component");

			auto comp = std::make_unique<T>(std::forward<Args>(args)...);

			comp->m_owner = this;

			T& ref = *comp;

			m_components.emplace_back(std::move(comp));

			ref.OnCreate();

			return &ref;
		}

		void Update(float dt);
		
		void LateUpdate(float dt);

		void Draw();
	private:
		std::vector<std::unique_ptr<Component>> m_components;

	};
}


