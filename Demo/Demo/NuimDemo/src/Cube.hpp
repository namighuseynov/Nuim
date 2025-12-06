#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.hpp"

class Cube {
public:
	Cube(ID3D11Device* device) : vertexBuffer(nullptr)
	{
		Vertex vertices[] = {
			{ {  0.0f,  0.5f, 0.0f }},
			{{  0.5f, -0.5f, 0.0f }},
			{{ -0.5f, -0.5f, 0.0f }},
		};

		D3D11_BUFFER_DESC bd = {};

		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertices);
		bd.CPUAccessFlags = 0;
		bd.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;

		HRESULT hr = device->CreateBuffer(&bd, &initData, &vertexBuffer);
	}
	~Cube() {
		if (vertexBuffer) vertexBuffer->Release();
	}


	void Draw(ID3D11DeviceContext* context) {
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->Draw(3, 0);
	}

private:
	ID3D11Buffer* vertexBuffer;
	
};