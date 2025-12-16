#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <cstdint>

namespace Nuim {

    class Texture2D {
    public:
        bool CreateCheckerboard(ID3D11Device* dev, int w, int h, int cellSize = 32);

        bool LoadFromFile(
            ID3D11Device* dev,
            ID3D11DeviceContext* ctx, 
            const std::wstring& path,
            bool generateMips = true
        );

        void Reset();

        int Width() const { return m_w; }
        int Height() const { return m_h; }

        ID3D11ShaderResourceView* SRV() const { return m_srv.Get(); }
        ID3D11SamplerState* Sampler() const { return m_samp.Get(); }

    private:
        bool CreateDefaultSampler(ID3D11Device* dev);

    private:
        int m_w = 0, m_h = 0;
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samp;
    };

}
