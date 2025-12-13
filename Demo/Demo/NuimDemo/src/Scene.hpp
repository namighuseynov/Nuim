#pragma once
#include <vector>
#include <memory>

#include "GameObject.hpp"

namespace NuimDemo {

	class CameraComponent;

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

		void SetMainCamera(CameraComponent* cam) { m_mainCamera = cam; }

		CameraComponent* GetMainCamera() const { return m_mainCamera; }

	private:
		std::vector<std::unique_ptr<GameObject>> m_objects;
		CameraComponent* m_mainCamera = nullptr;

	};
}