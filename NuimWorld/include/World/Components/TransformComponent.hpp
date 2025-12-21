#pragma once
#include <DirectXMath.h>
#include "World/ECS/Entity.hpp"

namespace Nuim::World {

    struct TransformComponent
    {
        DirectX::XMFLOAT3 localPos{ 0,0,0 };
        DirectX::XMFLOAT4 localRot{ 0,0,0,1 }; // quaternion
        DirectX::XMFLOAT3 localScale{ 1,1,1 };

        DirectX::XMFLOAT3 shear{ 0,0,0 };
    };

}
