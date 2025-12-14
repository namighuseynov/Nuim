#pragma once
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>
#include <string>
#include "Transform.hpp"
#include "Component.hpp"
#include "RenderQueue.hpp"

namespace Nuim {
	class GameObject
	{
	public:
		GameObject(const std::string& name = "GameObject")
			: m_id(s_nextId++), m_name(name) {}

		const std::vector<std::unique_ptr<Component>>& GetComponents() const { return m_components; }

		template<typename T>
		T* GetComponent()
		{
			static_assert(std::is_base_of_v<Component, T>);
			for (auto& c : m_components)
				if (auto casted = dynamic_cast<T*>(c.get()))
					return casted;
			return nullptr;
		}

		uint64_t GetId() const { return m_id; }

		const std::string& GetName() const { return m_name; }

		void SetName(const std::string& n) { m_name = n; }

		~GameObject() = default;

	public:
		Transform transform;

	public:
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

		void Update(float dt, bool isEditor);

		void LateUpdate(float dt, bool isEditor);

		void Submit(RenderQueue& q);

		void Draw();
	private:
		uint64_t m_id = 0;
		std::string m_name;

		static uint64_t s_nextId;

		std::vector<std::unique_ptr<Component>> m_components;

	};
}


