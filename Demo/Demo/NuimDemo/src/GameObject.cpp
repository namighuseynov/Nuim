#include "NuimDemoPCH.h"
#include "GameObject.hpp"

namespace NuimDemo {
	void GameObject::Update(float dt)
	{
		for (auto& c : m_components) {
			if (c->IsEnabled())
				c->Update(dt);
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