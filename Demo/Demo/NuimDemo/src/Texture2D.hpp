#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <vector>
#include <cstdint>

namespace Nuim {

    class Texture2D {
    public:
        bool CreateCheckerboard(ID3D11Device* dev, int w, int h, int cellSize = 32);

        ID3D11ShaderResourceView* SRV() const { return m_srv.Get(); }
        ID3D11SamplerState* Sampler() const { return m_samp.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;
        Microsoft::WRL::ComPtr<ID3D11SamplerState> m_samp;
    };

}
