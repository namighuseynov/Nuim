#include "NuimPCH.h"
#include "DX11/DX11RenderContext.hpp"

#include <stdexcept>

namespace Nuim::DX11 {

    DX11RenderContext::DX11RenderContext()
    {
        CreateDevice();
    }

    void DX11RenderContext::CreateDevice()
    {
        UINT flags = 0;
#if defined(NUIM_DEBUG)
        flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        D3D_FEATURE_LEVEL levels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0
        };

        D3D_FEATURE_LEVEL created = D3D_FEATURE_LEVEL_11_0;

        HRESULT hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            flags,
            levels,
            (UINT)std::size(levels),
            D3D11_SDK_VERSION,
            m_device.GetAddressOf(),
            &created,
            m_context.GetAddressOf()
        );

        if (FAILED(hr))
        {
            hr = D3D11CreateDevice(
                nullptr,
                D3D_DRIVER_TYPE_WARP,
                nullptr,
                flags,
                levels,
                (UINT)std::size(levels),
                D3D11_SDK_VERSION,
                m_device.GetAddressOf(),
                &created,
                m_context.GetAddressOf()
            );
        }

        if (FAILED(hr))
            throw std::runtime_error("DX11RenderContext: D3D11CreateDevice failed");
    }

}
