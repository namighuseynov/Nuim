#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace Nuim {

    class DepthStencil {
    public:
        bool Create(ID3D11Device* dev, int w, int h, DXGI_FORMAT fmt = DXGI_FORMAT_D24_UNORM_S8_UINT);
        void Reset();

        ID3D11DepthStencilView* DSV() const { return m_dsv.Get(); }
        ID3D11DepthStencilState* DSS() const { return m_dss.Get(); }

        int Width() const { return m_w; }
        int Height() const { return m_h; }

    private:
        int m_w = 0, m_h = 0;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dss;
    };

}