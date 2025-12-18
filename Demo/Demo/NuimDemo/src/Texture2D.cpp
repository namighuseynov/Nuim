#include "NuimDemoPCH.h"
#include <wincodec.h>
#pragma comment(lib, "windowscodecs.lib")

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

    static bool WICCreateFactory(Microsoft::WRL::ComPtr<IWICImagingFactory>& out)
    {
        HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&out));
        if (FAILED(hr))
            hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&out));
        return SUCCEEDED(hr);
    }

    static bool WICLoadRGBA8(
        const std::wstring& path,
        std::vector<uint8_t>& outRGBA,
        int& outW, int& outH
    )
    {
        Microsoft::WRL::ComPtr<IWICImagingFactory> factory;
        if (!WICCreateFactory(factory)) return false;


        if (!factory) return false;

        Microsoft::WRL::ComPtr<IWICBitmapDecoder> decoder;
        HRESULT hr = factory->CreateDecoderFromFilename(
            path.c_str(),
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnDemand,
            &decoder
        );
        if (FAILED(hr)) return false;

        Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> frame;
        hr = decoder->GetFrame(0, &frame);
        if (FAILED(hr)) return false;

        UINT w = 0, h = 0;
        hr = frame->GetSize(&w, &h);
        if (FAILED(hr) || w == 0 || h == 0) return false;

        Microsoft::WRL::ComPtr<IWICFormatConverter> conv;
        hr = factory->CreateFormatConverter(&conv);
        if (FAILED(hr)) return false;

        hr = conv->Initialize(
            frame.Get(),
            GUID_WICPixelFormat32bppRGBA, 
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeCustom
        );
        if (FAILED(hr)) return false;

        outW = (int)w;
        outH = (int)h;
        outRGBA.resize((size_t)outW * (size_t)outH * 4);

        const UINT stride = (UINT)outW * 4;
        const UINT imageSize = stride * (UINT)outH;

        hr = conv->CopyPixels(nullptr, stride, imageSize, outRGBA.data());
        if (FAILED(hr)) return false;

        return true;
    }

    bool Texture2D::CreateDefaultSampler(ID3D11Device* dev)
    {
        if (!dev) return false;

        D3D11_SAMPLER_DESC sd = {};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.MaxLOD = D3D11_FLOAT32_MAX;

        return SUCCEEDED(dev->CreateSamplerState(&sd, &m_samp));
    }

    void Texture2D::Reset()
    {
        m_tex.Reset();
        m_srv.Reset();
        m_samp.Reset();
        m_w = m_h = 0;
    }

    bool Texture2D::LoadFromFile(
        ID3D11Device* dev,
        ID3D11DeviceContext* ctx,
        const std::wstring& path,
        bool generateMips
    )
    {
        Reset();
        if (!dev) return false;
        if (generateMips && !ctx) return false;

        std::vector<uint8_t> rgba;
        if (!WICLoadRGBA8(path, rgba, m_w, m_h))
            return false;

        DXGI_FORMAT fmt = DXGI_FORMAT_R8G8B8A8_UNORM;

        if (!generateMips)
        {
            D3D11_TEXTURE2D_DESC td = {};
            td.Width = (UINT)m_w;
            td.Height = (UINT)m_h;
            td.MipLevels = 1;
            td.ArraySize = 1;
            td.Format = fmt;
            td.SampleDesc.Count = 1;
            td.Usage = D3D11_USAGE_IMMUTABLE;
            td.BindFlags = D3D11_BIND_SHADER_RESOURCE;

            D3D11_SUBRESOURCE_DATA init = {};
            init.pSysMem = rgba.data();
            init.SysMemPitch = (UINT)m_w * 4;

            HRESULT hr = dev->CreateTexture2D(&td, &init, &m_tex);
            if (FAILED(hr)) return false;

            hr = dev->CreateShaderResourceView(m_tex.Get(), nullptr, &m_srv);
            if (FAILED(hr)) return false;
        }
        else
        {
            D3D11_TEXTURE2D_DESC td = {};
            td.Width = (UINT)m_w;
            td.Height = (UINT)m_h;
            td.MipLevels = 0;
            td.ArraySize = 1;
            td.Format = fmt;
            td.SampleDesc.Count = 1;
            td.Usage = D3D11_USAGE_DEFAULT;
            td.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
            td.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

            HRESULT hr = dev->CreateTexture2D(&td, nullptr, &m_tex);
            if (FAILED(hr)) return false;

            hr = dev->CreateShaderResourceView(m_tex.Get(), nullptr, &m_srv);
            if (FAILED(hr)) return false;

            ctx->UpdateSubresource(
                m_tex.Get(),
                0,
                nullptr,
                rgba.data(),
                (UINT)m_w * 4,
                0
            );

            ctx->GenerateMips(m_srv.Get());
        }

        if (!CreateDefaultSampler(dev))
            return false;

        return true;
    }
}
