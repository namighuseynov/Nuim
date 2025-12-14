#pragma once
#include <vector>
#include <memory>

#include "GameObject.hpp"
#include "RenderQueue.hpp"
#include "Renderer/Renderer.hpp"

namespace Nuim {

	class CameraComponent;

	class Scene
	{
	public:
		Scene() = default;
		~Scene() = default;

		GameObject& CreateObject();

		void Update(float dt);

		void LateUpdate(float dt);
		
		void Draw();

		void Clear();

		void Render(Renderer* renderer);

		size_t GetObjectCount() const { return m_objects.size(); }

		void SetMainCamera(CameraComponent* cam) { m_mainCamera = cam; }

		CameraComponent* GetMainCamera() const { return m_mainCamera; }

		const std::vector<std::unique_ptr<GameObject>>& GetObjects() const { return m_objects; }

	private:
		std::vector<std::unique_ptr<GameObject>> m_objects;
		CameraComponent* m_mainCamera = nullptr;
		RenderQueue m_renderQueue;

	};
}