#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include <d3d11.h>

#include "Mesh.hpp"
#include "Material.hpp"

namespace Nuim {

    class ResourceManager {
    public:
        using MeshPtr = std::shared_ptr<Mesh>;
        using MaterialPtr = std::shared_ptr<Material>;

        MeshPtr CreateMesh(
            const std::string& name,
            ID3D11Device* device,
            const void* vertexData,
            UINT vertexStride,
            UINT vertexCount,
            const uint32_t* indices,
            UINT indexCount)
        {
            auto mesh = std::make_shared<Mesh>();
            if (!mesh->Init(device, vertexData, vertexStride, vertexCount, indices, indexCount))
                return nullptr;

            m_meshes[name] = mesh;
            return mesh;
        }

        MaterialPtr CreateMaterial(
            const std::string& name,
            ID3D11Device* device,
            const std::wstring& vsPath,
            const std::wstring& psPath,
            const D3D11_INPUT_ELEMENT_DESC* layout,
            UINT layoutCount)
        {
            auto mat = std::make_shared<Material>();
            if (!mat->Init(device, vsPath, psPath, layout, layoutCount))
                return nullptr;

            m_materials[name] = mat;
            return mat;
        }

        MeshPtr GetMesh(const std::string& name) const
        {
            auto it = m_meshes.find(name);
            return (it != m_meshes.end()) ? it->second : nullptr;
        }

        MaterialPtr GetMaterial(const std::string& name) const
        {
            auto it = m_materials.find(name);
            return (it != m_materials.end()) ? it->second : nullptr;
        }

        std::vector<std::string> GetMeshNames() const
        {
            std::vector<std::string> out;
            out.reserve(m_meshes.size());
            for (auto& [k, v] : m_meshes) out.push_back(k);
            return out;
        }

        std::vector<std::string> GetMaterialNames() const
        {
            std::vector<std::string> out;
            out.reserve(m_materials.size());
            for (auto& [k, v] : m_materials) out.push_back(k);
            return out;
        }

        void Clear()
        {
            m_meshes.clear();
            m_materials.clear();
        }

    private:
        std::unordered_map<std::string, MeshPtr> m_meshes;
        std::unordered_map<std::string, MaterialPtr> m_materials;
    };

}
