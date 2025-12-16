#include "NuimDemoPCH.h"
#include "RenderContext.hpp"

namespace Nuim {
    using namespace DirectX;

    bool RenderContext::Init(ID3D11Device* device)
    {
        D3D11_BUFFER_DESC cbd = {};
        cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbd.Usage = D3D11_USAGE_DEFAULT;
        cbd.CPUAccessFlags = 0;
        cbd.ByteWidth = (sizeof(ConstantBufferData) + 15) & ~15;

        HRESULT hr = device->CreateBuffer(&cbd, nullptr, &m_perObjectCB);
        return SUCCEEDED(hr);
    }

    void RenderContext::SetCamera(
        const XMMATRIX& view,
        const XMMATRIX& proj
    )
    {
        XMStoreFloat4x4(&m_view, XMMatrixTranspose(view));
        XMStoreFloat4x4(&m_proj, XMMatrixTranspose(proj));
    }

    void RenderContext::UpdateConstantBuffer(
        ID3D11DeviceContext* ctx,
        const RenderItem& item
    )
    {
        ConstantBufferData data{};

        XMMATRIX W = XMLoadFloat4x4(&item.world);

        XMStoreFloat4x4(&data.world, XMMatrixTranspose(W));
        data.view = m_view;
        data.proj = m_proj;
        data.uvTiling = item.material->GetUVTiling();
        data.uvOffset = item.material->GetUVOffset();

        ctx->UpdateSubresource(
            m_perObjectCB.Get(),
            0,
            nullptr,
            &data,
            0,
            0
        );

        ctx->VSSetConstantBuffers(0, 1, m_perObjectCB.GetAddressOf());
    }

    void RenderContext::DrawItem(
        ID3D11DeviceContext* ctx,
        const RenderItem& item
    )
    {
        if (!item.mesh || !item.material)
            return;

        UpdateConstantBuffer(ctx, item);

        item.material->Bind(ctx);
        item.mesh->Draw(ctx);
    }

}
