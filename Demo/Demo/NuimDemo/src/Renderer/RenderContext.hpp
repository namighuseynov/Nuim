#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <DirectXMath.h>

#include "ConstantBufferData.hpp"
#include "RenderQueue.hpp"

namespace Nuim {

    class RenderContext {
    public:
        bool Init(ID3D11Device* device);

        void SetCamera(
            const DirectX::XMMATRIX& view,
            const DirectX::XMMATRIX& proj
        );

        void DrawItem(
            ID3D11DeviceContext* ctx,
            const Nuim::RenderItem& item
        );

    private:
        void UpdateConstantBuffer(
            ID3D11DeviceContext* ctx,
            const RenderItem& item
        );

    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_perObjectCB;

        DirectX::XMFLOAT4X4 m_view;
        DirectX::XMFLOAT4X4 m_proj;
    };
}