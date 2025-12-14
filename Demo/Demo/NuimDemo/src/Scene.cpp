#include "NuimDemoPCH.h"
#include "Scene.hpp"


namespace Nuim {
	GameObject& Scene::CreateObject()
	{
		auto obj = std::make_unique<GameObject>();

		GameObject& ref = *obj;

		m_objects.push_back(std::move(obj));

		return ref;
	}

	void Scene::Draw()
	{
		for (auto& o : m_objects)
			o->Draw();
	}

	void Scene::Update(float dt, bool isEditor)
	{
		for (auto& obj : m_objects) {
			obj->Update(dt, isEditor);
		}
	}

	void Scene::LateUpdate(float dt, bool isEditor)
	{
		for (auto& obj : m_objects)
			obj->LateUpdate(dt, isEditor);
	}

	void Scene::Clear()
	{
		m_objects.clear();
	}
	void Scene::Render(Renderer* renderer)
	{
		if (!renderer) return;

		m_renderQueue.Clear();
		for (auto& obj : m_objects)
			obj->Submit(m_renderQueue);

		for (const auto& item : m_renderQueue.Items())
			renderer->DrawItem(item);
	}
}