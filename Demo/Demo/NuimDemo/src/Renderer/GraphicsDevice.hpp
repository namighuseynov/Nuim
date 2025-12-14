#pragma once
#include <d3d11.h>
#include <wrl/client.h>

namespace Nuim {
	class GraphicsDevice
	{
	public:
		bool Init();
		ID3D11Device* Device() const { return m_device.Get(); }
		ID3D11DeviceContext* Context() const { return m_context.Get(); }

	private:
		Microsoft::WRL::ComPtr<ID3D11Device> m_device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
	};
}