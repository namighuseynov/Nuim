#pragma once
#include <d3d11.h>

class Mesh
{
public:
	Mesh() :
		m_vertexBuffer(nullptr),
		m_indexBuffer(nullptr),
		m_vertexStride(0),
		m_indexCount(0),
		m_indexFormat(DXGI_FORMAT_R32_UINT) 
	{}

	~Mesh() {
		if (m_vertexBuffer) m_vertexBuffer->Release();
		if (m_indexBuffer) m_indexBuffer->Release();

	}

	bool Init(
		ID3D11Device* device,
		const void* vertexData,
		UINT vertexStride,
		UINT vertexCount,
		const uint32_t* indices,
		UINT indexCount) 
	{
		m_vertexStride = vertexStride;
		m_indexCount = indexCount;
		m_indexFormat = DXGI_FORMAT_R32_UINT;
		
		// Vertex buffer
		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = vertexStride * vertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vinit = {};
		vinit.pSysMem = vertexData;

		HRESULT hr = device->CreateBuffer(&vbd, &vinit, &m_vertexBuffer);
		if (FAILED(hr)) return false;

		// --- Index buffer ---
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.ByteWidth = sizeof(uint32_t) * indexCount;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA iinit = {};
		iinit.pSysMem = indices;

		hr = device->CreateBuffer(&ibd, &iinit, &m_indexBuffer);
		if (FAILED(hr)) return false;

		return true;
	}
	

	void Draw(ID3D11DeviceContext* context) {
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexStride, &offset);
		context->IASetIndexBuffer(m_indexBuffer, m_indexFormat, 0);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->DrawIndexed(m_indexCount, 0, 0);
	}

private:
	ID3D11Buffer*	m_vertexBuffer;
	ID3D11Buffer*	m_indexBuffer;
	UINT			m_indexCount;
	UINT			m_vertexStride;
	DXGI_FORMAT		m_indexFormat;
};