#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.hpp"

class Cube {
public:
	Cube(ID3D11Device* device) : vertexBuffer(nullptr), indexBuffer(nullptr)
	{
		Vertex vertices[] = {
			{{-1, -1, -1}, {1, 0, 0}},
			{{-1, +1, -1}, {0, 1, 0}},
			{{+1, +1, -1}, {0, 0, 1}},
			{{+1, -1, -1}, {1, 1, 0}},
			{{-1, -1, +1}, {1, 0, 1}},
			{{-1, +1, +1}, {0, 1, 1}},
			{{+1, +1, +1}, {1, 1, 1}},
			{{+1, -1, +1}, {0, 0, 0}},
		};

		USHORT indices[] = {
		0,1,2, 0,2,3,
		4,6,5, 4,7,6,
		4,5,1, 4,1,0,
		3,2,6, 3,6,7,
		1,5,6, 1,6,2,
		4,0,3, 4,3,7
		};

		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(vertices);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vinitData = {};
		vinitData.pSysMem = vertices;

		HRESULT hr = device->CreateBuffer(&vbd, &vinitData, &vertexBuffer);

		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(indices);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData = {};
		iinitData.pSysMem = indices;

		HRESULT hr = device->CreateBuffer(&ibd, &iinitData, &indexBuffer);
	}
	~Cube() {
		if (vertexBuffer) vertexBuffer->Release();
		if (indexBuffer) indexBuffer->Release();
	}


	void Draw(ID3D11DeviceContext* context, const DirectX::XMMATRIX& viewProjMatrix) {

	}

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	
};