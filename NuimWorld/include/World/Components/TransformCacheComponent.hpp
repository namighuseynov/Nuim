#pragma once
#include <DirectXMath.h>

namespace Nuim::World {

    struct TransformCacheComponent
    {
        bool dirtyLocal = true;
        bool dirtyWorld = true;

        DirectX::XMFLOAT4X4 cachedLocal;
        DirectX::XMFLOAT4X4 cachedWorld;

        TransformCacheComponent()
        {
            DirectX::XMStoreFloat4x4(&cachedLocal, DirectX::XMMatrixIdentity());
            DirectX::XMStoreFloat4x4(&cachedWorld, DirectX::XMMatrixIdentity());
        }
    };

}
