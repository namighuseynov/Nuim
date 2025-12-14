#pragma once
#include <vector>
#include <DirectXMath.h>

#include "Mesh.hpp"
#include "Material.hpp"

namespace Nuim
{
    struct RenderItem
    {
        Mesh* mesh = nullptr;                 
        Material* material = nullptr;        
        DirectX::XMFLOAT4X4 world;
    };

    class RenderQueue
    {
    public:
        void Clear() { m_items.clear(); }

        void Add(const RenderItem& item) { m_items.push_back(item); }

        const std::vector<RenderItem>& Items() const { return m_items; }

    private:
        std::vector<RenderItem> m_items;
    };
}
