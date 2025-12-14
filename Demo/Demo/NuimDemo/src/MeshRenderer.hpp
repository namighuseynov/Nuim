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
		MeshRenderer(Renderer* renderer, Mesh* mesh, Material* material)
			: m_renderer(renderer), m_mesh(mesh), m_material(material)
		{}

		void Draw() override {
			if (!m_renderer || !m_mesh || !m_material) return;

			ID3D11DeviceContext* ctx = m_renderer->GetContext();
			ID3D11Buffer* cb = m_renderer->GetConstantBuffer();

			DirectX::XMMATRIX world = m_owner->transform.GetWorldMatrix();

			ConstantBufferData data{};
			DirectX::XMStoreFloat4x4(&data.world, DirectX::XMMatrixTranspose(world));
			data.view = m_renderer->GetView();
			data.proj = m_renderer->GetProj();

			ctx->UpdateSubresource(cb, 0, nullptr, &data, 0, 0);

			ctx->VSSetConstantBuffers(0, 1, &cb);

			m_material->Bind(ctx);
			m_mesh->Draw(ctx);
		}

		void OnCreate() override {
			std::cout << "MeshRenderer created for GameObject at " << m_owner << std::endl;
		}

		void Update(float dt) override {
			// Example update logic

		}

	private:
		Renderer* m_renderer;
		Mesh* m_mesh;
		Material* m_material; 
	};
}