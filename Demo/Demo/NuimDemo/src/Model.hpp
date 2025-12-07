#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "Mesh.hpp"
#include "Material.hpp"
#include "ConstantBufferData.hpp"


class Model
{
public:
    Model(Mesh* mesh, Material* material)
        : m_mesh(mesh),
        m_material(material),
        m_world(DirectX::XMMatrixIdentity())
    {}

    void SetWorld(const DirectX::XMMATRIX& world)
    {
        m_world = world;
    }

    const DirectX::XMMATRIX& GetWorld() const { return m_world; }

    void Draw(ID3D11DeviceContext* ctx,
        ID3D11Buffer* constantBuffer,
        const DirectX::XMFLOAT4X4& view,
        const DirectX::XMFLOAT4X4& proj
    ) const
    {
        ConstantBufferData cbd;
        DirectX::XMStoreFloat4x4(&cbd.world, DirectX::XMMatrixTranspose(m_world));
        cbd.view = view;
        cbd.proj = proj;

        ctx->UpdateSubresource(constantBuffer, 0, nullptr, &cbd, 0, 0);
        ctx->VSSetConstantBuffers(0, 1, &constantBuffer);

        m_material->Bind(ctx);
        m_mesh->Draw(ctx);
    }

private:
    Mesh* m_mesh;
    Material* m_material;
    DirectX::XMMATRIX m_world;
};