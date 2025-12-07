#pragma once
#include <DirectXMath.h>

struct ConstantBufferData
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 proj;
};