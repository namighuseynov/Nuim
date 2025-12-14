#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace Nuim {
    class FrameBuffers {
    public:
        bool Init(ID3D11Device* device, int w, int h);
        bool Resize(ID3D11Device* device, int w, int h);

        void Bind(ID3D11DeviceContext* ctx, ID3D11RenderTargetView* rtv);
        void Clear(ID3D11DeviceContext* ctx, ID3D11RenderTargetView* rtv, const float color[4]);

    private:
        void CreateDepth(ID3D11Device* device, int w, int h);

    private:
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depth;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_dsv;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_dss;
        D3D11_VIEWPORT m_viewport{};
    };
}

