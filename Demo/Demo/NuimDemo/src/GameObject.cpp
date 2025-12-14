#include "NuimDemoPCH.h"
#include "GameObject.hpp"

namespace Nuim {
	uint64_t GameObject::s_nextId = 1;

	void GameObject::Update(float dt, bool isEditor)
	{
		for (auto& c : m_components)
		{
			if (!c->IsEnabled()) continue;
			if (isEditor && !c->RunInEditor()) continue;
			c->Update(dt);
		}
	}

	void GameObject::LateUpdate(float dt, bool isEditor)
	{
		for (auto& c : m_components)
		{
			if (!c->IsEnabled()) continue;
			if (isEditor && !c->RunInEditor()) continue;
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