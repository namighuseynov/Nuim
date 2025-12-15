#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace Nuim {

    class RenderTarget2D {
    public:
        bool Init(ID3D11Device* device, int w, int h, DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM) {
            m_format = fmt;
            return Resize(device, w, h);
        }

        bool Resize(ID3D11Device* device, int w, int h) {
            if (!device) return false;
            if (w <= 0 || h <= 0) return false;

            if (w == m_w && h == m_h && m_tex && m_rtv && m_srv)
                return true;

            m_w = w; m_h = h;

            m_tex.Reset();
            m_rtv.Reset();
            m_srv.Reset();

            D3D11_TEXTURE2D_DESC td = {};
            td.Width = (UINT)w;
            td.Height = (UINT)h;
            td.MipLevels = 1;
            td.ArraySize = 1;
            td.Format = m_format;
            td.SampleDesc.Count = 1;
            td.Usage = D3D11_USAGE_DEFAULT;
            td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

            HRESULT hr = device->CreateTexture2D(&td, nullptr, &m_tex);
            if (FAILED(hr)) return false;

            hr = device->CreateRenderTargetView(m_tex.Get(), nullptr, &m_rtv);
            if (FAILED(hr)) return false;

            hr = device->CreateShaderResourceView(m_tex.Get(), nullptr, &m_srv);
            if (FAILED(hr)) return false;

            return true;
        }

        ID3D11RenderTargetView* RTV() const { return m_rtv.Get(); }
        ID3D11ShaderResourceView* SRV() const { return m_srv.Get(); }

        int Width() const { return m_w; }
        int Height() const { return m_h; }

    private:
        int m_w = 0, m_h = 0;
        DXGI_FORMAT m_format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
    };

}
