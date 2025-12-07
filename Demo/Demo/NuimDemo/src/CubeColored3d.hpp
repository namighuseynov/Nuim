#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

struct VertexColored {
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 color;
};

class CubeColored3d
{
public:
	CubeColored3d(ID3D11Device* device) : vertexBuffer(nullptr), indexBuffer(nullptr) {
		

		VertexColored vertices[] = {
			{ { -0.5f, -0.5f, -0.5f }, {1.0f, 0.0f, 0.0f, 1.0f} },
			{ { -0.5f,  0.5f, -0.5f }, {0.0f, 1.0f, 0.0f, 1.0f} },
			{ {  0.5f,  0.5f, -0.5f }, {0.0f, 0.0f, 1.0f, 1.0f} },
			{ {  0.5f, -0.5f, -0.5f }, {1.0f, 1.0f, 0.0f, 1.0f} },
			{ { -0.5f, -0.5f,  0.5f }, {1.0f, 0.0f, 1.0f, 1.0f} },
			{ { -0.5f,  0.5f,  0.5f }, {0.0f, 1.0f, 1.0f, 1.0f} },
			{ {  0.5f,  0.5f,  0.5f }, {1.0f, 1.0f, 1.0f, 1.0f} },
			{ {  0.5f, -0.5f,  0.5f }, {0.0f, 0.0f, 0.0f, 1.0f} },
		};

		D3D11_BUFFER_DESC  bd = {};
		bd.ByteWidth = sizeof(vertices);
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;

		HRESULT hr = device->CreateBuffer(&bd, &initData, &vertexBuffer);
		if (FAILED(hr)) {
			std::cout << "Failed to create vertex buffer" << std::endl;
		}

		UINT indices[] = {
			0,1,2,  0,2,3,
			4,6,5,  4,7,6,
			4,5,1,  4,1,0,
			3,2,6,  3,6,7,
			1,5,6,  1,6,2,
			4,0,3,  4,3,7
		};

		D3D11_BUFFER_DESC ibd = {};
		ibd.ByteWidth = sizeof(indices);
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		D3D11_SUBRESOURCE_DATA iinitData = {};
		iinitData.pSysMem = indices;	
		hr = device->CreateBuffer(&ibd, &iinitData, &indexBuffer);
		if (FAILED(hr)) {
			std::cout << "Failed to create index buffer" << std::endl;
		}
	}


	~CubeColored3d() {
		if (vertexBuffer) vertexBuffer->Release();
		if (indexBuffer) indexBuffer->Release();
	}

	void Draw(ID3D11DeviceContext* context) {
		UINT stride = sizeof(VertexColored);
		UINT offset = 0;

		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexed(36, 0, 0);
	}

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};