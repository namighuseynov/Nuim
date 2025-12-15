#include "NuimDemoPCH.h"
#include "Texture2D.hpp"

namespace Nuim {

    bool Texture2D::CreateCheckerboard(ID3D11Device* dev, int w, int h, int cellSize)
    {
        if (!dev || w <= 0 || h <= 0) return false;

        std::vector<uint32_t> pixels((size_t)w * (size_t)h);

        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                int cx = x / cellSize;
                int cy = y / cellSize;
                bool odd = ((cx + cy) & 1) != 0;

                uint8_t v = odd ? 30 : 220;
                uint32_t rgba = (0xFFu << 24) | (v << 16) | (v << 8) | (v);
                pixels[(size_t)y * (size_t)w + (size_t)x] = rgba;
            }
        }

        D3D11_TEXTURE2D_DESC td = {};
        td.Width = (UINT)w;
        td.Height = (UINT)h;
        td.MipLevels = 1;
        td.ArraySize = 1;
        td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        td.SampleDesc.Count = 1;
        td.Usage = D3D11_USAGE_IMMUTABLE;
        td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA init = {};
        init.pSysMem = pixels.data();
        init.SysMemPitch = (UINT)(w * sizeof(uint32_t));

        HRESULT hr = dev->CreateTexture2D(&td, &init, &m_tex);
        if (FAILED(hr)) return false;

        hr = dev->CreateShaderResourceView(m_tex.Get(), nullptr, &m_srv);
        if (FAILED(hr)) return false;

        D3D11_SAMPLER_DESC sd = {};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.MaxLOD = D3D11_FLOAT32_MAX;

        hr = dev->CreateSamplerState(&sd, &m_samp);
        if (FAILED(hr)) return false;

        return true;
    }

}
