#include "NuimDemoPCH.h"
#include "GraphicsDevice.hpp"

namespace Nuim {
	bool GraphicsDevice::Init()
	{
        UINT flags = 0; 
        D3D_FEATURE_LEVEL fl; 

        HRESULT hr = D3D11CreateDevice( 
            nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 
            flags, nullptr, 0, D3D11_SDK_VERSION, 
            &m_device, &fl, &m_context 
        );

        return SUCCEEDED(hr);
	}
}