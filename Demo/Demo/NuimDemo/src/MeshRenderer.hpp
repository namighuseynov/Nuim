#pragma once
#include "Component.hpp"
#include "Renderer.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "GameObject.hpp"

namespace NuimDemo {
	class MeshRenderer : 
		public Component
	{
	public:
		MeshRenderer(Mesh* mesh, Material* material)
			: m_mesh(mesh), m_material(material)
		{}

		void Submit(RenderQueue& q) override
		{
			if (!m_mesh || !m_material || !m_owner) return;

			RenderItem item;
			item.mesh = m_mesh;
			item.material = m_material;

			// Transform -> world matrix
			DirectX::XMStoreFloat4x4(&item.world, m_owner->transform.GetWorldMatrix());

			q.Add(item);
		}

		void OnCreate() override {
			std::cout << "MeshRenderer created for GameObject at " << m_owner << std::endl;
		}

		void Update(float dt) override {
			// Example update logic

		}

	private:
		Mesh* m_mesh;
		Material* m_material; 
	};
}