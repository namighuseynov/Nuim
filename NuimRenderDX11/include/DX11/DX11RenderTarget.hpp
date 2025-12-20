#pragma once
#include "Render/IRenderTarget.hpp"
#include "Render/RenderTypes.hpp"
#include "DX11/DX11Common.hpp"

namespace Nuim::DX11 {

    class DX11RenderContext;

    class DX11RenderTarget final : public Nuim::Render::IRenderTarget
    {
    public:
        DX11RenderTarget(DX11RenderContext& ctx, const Nuim::Render::RenderTargetDesc& desc);
        ~DX11RenderTarget() override = default;

        void Resize(Nuim::U32 width, Nuim::U32 height) override;

        Nuim::U32 GetWidth() const override { return m_width; }
        Nuim::U32 GetHeight() const override { return m_height; }

        void Bind() override;
        void Clear(float r, float g, float b, float a) override;

        void* GetImGuiTextureID() const override { return (void*)m_srv.Get(); }

    private:
        void CreateResources();
        void ReleaseResources();

    private:
        DX11RenderContext& m_ctx;

        Nuim::U32 m_width = 1;
        Nuim::U32 m_height = 1;
        bool m_hasDepth = true;

        ComPtr<ID3D11Texture2D> m_colorTex;
        ComPtr<ID3D11RenderTargetView> m_rtv;
        ComPtr<ID3D11ShaderResourceView> m_srv;

        ComPtr<ID3D11Texture2D> m_depthTex;
        ComPtr<ID3D11DepthStencilView> m_dsv;
    };

}
