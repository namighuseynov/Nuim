#pragma once
#include "Component.hpp"
#include "Renderer/Renderer.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "GameObject.hpp"

namespace Nuim {
	class MeshRenderer : 
		public Component
	{
	public:
		MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
			: m_mesh(std::move(mesh)), m_material(std::move(material)) {}

		const char* GetTypeName() const override { return "MeshRenderer"; }

		void Submit(RenderQueue& q) override
		{
			if (!m_mesh || !m_material || !m_owner) return;

			RenderItem item;
			item.mesh = m_mesh.get();
			item.material = m_material.get();

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
		std::shared_ptr<Mesh> m_mesh;
		std::shared_ptr<Material> m_material;
	};
}