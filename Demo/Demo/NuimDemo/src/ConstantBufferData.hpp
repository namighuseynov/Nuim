#pragma once
#include <DirectXMath.h>

struct ConstantBufferData
{
    DirectX::XMFLOAT4X4 world;
    DirectX::XMFLOAT4X4 view;
    DirectX::XMFLOAT4X4 proj;

    DirectX::XMFLOAT2 uvTiling = { 1.0f, 1.0f };
    DirectX::XMFLOAT2 uvOffset = { 0.0f, 0.0f };
};