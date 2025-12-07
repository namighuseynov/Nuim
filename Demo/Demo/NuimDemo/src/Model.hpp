#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "Mesh.hpp"
#include "Material.hpp"


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

    void Draw(ID3D11DeviceContext* ctx) const
    {
        m_material->Bind(ctx);
        m_mesh->Draw(ctx);
    }

private:
    Mesh* m_mesh;
    Material* m_material;
    DirectX::XMMATRIX m_world;
};