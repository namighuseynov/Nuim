#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

class Material
{
public:
    Material() = default;

    ~Material() = default;

	bool Init(
		ID3D11Device* device,
		const std::wstring& vsPath,
		const std::wstring& psPath,
		const D3D11_INPUT_ELEMENT_DESC* layout,
		UINT layoutCount)
	{
		HRESULT hr;
		ID3DBlob* vsBlob = nullptr;
		ID3DBlob* psBlob = nullptr;
		ID3DBlob* errorBlob = nullptr;

        // --- Vertex shader ---
        hr = D3DCompileFromFile(
            vsPath.c_str(),
            nullptr, nullptr,
            "main", "vs_5_0",
            0, 0,
            &vsBlob, &errorBlob
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                std::cout << "VS compile error: "
                    << (char*)errorBlob->GetBufferPointer() << std::endl;
                errorBlob->Release();
            }
            return false;
        }

        hr = device->CreateVertexShader(
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr,
            m_vs.GetAddressOf()
        );
        if (FAILED(hr)) return false;

        // --- Input layout ---
        hr = device->CreateInputLayout(
            layout,
            layoutCount,
            vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            m_inputLayout.GetAddressOf()
        );
        vsBlob->Release();
        if (FAILED(hr))
        {
            std::cout << "Failed to create input layout\n";
            return false;
        }

        // --- Pixel shader ---
        hr = D3DCompileFromFile(
            psPath.c_str(),
            nullptr, nullptr,
            "main", "ps_5_0",
            0, 0,
            &psBlob, &errorBlob
        );

        if (FAILED(hr))
        {
            if (errorBlob)
            {
                std::cout << "PS compile error: "
                    << (char*)errorBlob->GetBufferPointer() << std::endl;
                errorBlob->Release();
            }
            return false;
        }

        hr = device->CreatePixelShader(
            psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr,
            m_ps.GetAddressOf()
        );
        psBlob->Release();
        if (FAILED(hr)) return false;

        return true;
	}

    void Bind(ID3D11DeviceContext* ctx) const
    {
        ctx->IASetInputLayout(m_inputLayout.Get());
        ctx->VSSetShader(m_vs.Get(), nullptr, 0);
        ctx->PSSetShader(m_ps.Get(), nullptr, 0);
    }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>  m_vs;
    Microsoft::WRL::ComPtr<ID3D11PixelShader>   m_ps;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;
};