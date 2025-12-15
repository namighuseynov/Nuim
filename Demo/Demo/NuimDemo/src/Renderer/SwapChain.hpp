#pragma once
#include <dxgi.h>
#include <d3d11.h>
#include <wrl/client.h>

namespace Nuim {

    class SwapChain {
    public:
        bool Init(HWND hwnd, ID3D11Device* device, int w, int h);
        bool Resize(ID3D11Device* device, int w, int h);
        void Present(bool vsync);

        ID3D11RenderTargetView* RTV() const { return m_rtv.Get(); }
        IDXGISwapChain* DxgiSwapChain() const { return m_sc.Get(); }

    private:
        bool CreateRTV(ID3D11Device* device);

    private:
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_sc;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    };

}
