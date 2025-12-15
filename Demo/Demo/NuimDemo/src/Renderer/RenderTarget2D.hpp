#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include "DepthStencil.hpp"

namespace Nuim {

    class RenderTarget2D {
    public:
        bool Create(ID3D11Device* dev, int w, int h, DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM);
        bool Resize(ID3D11Device* dev, int w, int h);

        void Bind(ID3D11DeviceContext* ctx);
        void Clear(ID3D11DeviceContext* ctx, const float color[4], bool clearDepth = true, bool clearStencil = true);

        ID3D11RenderTargetView* RTV() const { return m_rtv.Get(); }
        ID3D11ShaderResourceView* SRV() const { return m_srv.Get(); }
        DepthStencil& Depth() { return m_depth; }

        int Width() const { return m_w; }
        int Height() const { return m_h; }

    private:
        bool CreateColor(ID3D11Device* dev);

    private:
        int m_w = 0, m_h = 0;
        DXGI_FORMAT m_fmt = DXGI_FORMAT_R8G8B8A8_UNORM;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_color;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

        DepthStencil m_depth;
        D3D11_VIEWPORT m_vp{};
    };

}
