#include "NuimDemoPCH.h"
#include "Scene.hpp"

namespace NuimDemo {
	GameObject& NuimDemo::Scene::CreateObject()
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

	void Scene::Update(float dt)
	{
		for (auto& obj : m_objects) {
			obj->Update(dt);
		}
	}

	void Scene::Clear()
	{
		m_objects.clear();
	}
}