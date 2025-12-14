#include "NuimDemoPCH.h"
#include "GameObject.hpp"

namespace Nuim {
	void GameObject::Update(float dt)
	{
		for (auto& c : m_components) {
			if (c->IsEnabled())
				c->Update(dt);
		}
	}

	void GameObject::LateUpdate(float dt)
	{
		for (auto& c : m_components) {
			if (c->IsEnabled())
				c->LateUpdate(dt);
		}
	}

	void GameObject::Submit(RenderQueue& q)
	{
		for (auto& c : m_components)
		{
			if (c->IsEnabled())
				c->Submit(q);
		}
	}

	void GameObject::Draw()
	{
		for (auto& c : m_components)
		{
			if (c->IsEnabled())
				c->Draw();
		}
	}
}