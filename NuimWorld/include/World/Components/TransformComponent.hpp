#pragma once
#include <DirectXMath.h>
#include "World/ECS/Entity.hpp"

namespace Nuim::World {

    struct TransformComponent
    {
        DirectX::XMFLOAT3 localPos{ 0,0,0 };
        DirectX::XMFLOAT4 localRot{ 0,0,0,1 }; // quaternion
        DirectX::XMFLOAT3 localScale{ 1,1,1 };

        // hierarchy (professional, no vectors)
        Entity parent = NullEntity;
        Entity firstChild = NullEntity;
        Entity nextSibling = NullEntity;
        Entity prevSibling = NullEntity;

        bool dirtyLocal = true;
        bool dirtyWorld = true;

        DirectX::XMMATRIX cachedLocal = DirectX::XMMatrixIdentity();
        DirectX::XMMATRIX cachedWorld = DirectX::XMMatrixIdentity();
    };

} // namespace Nuim::World
