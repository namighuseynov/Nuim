#pragma once
#include "Core/Base.hpp"

#if NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#endif

namespace Nuim {

    class DX11Context {
    public:
        DX11Context() = default;
        ~DX11Context() = default;

        void Init(void* nativeWindowHandle, U32 width, U32 height, bool enableDebug);
        void Shutdown();

        void Resize(U32 width, U32 height);

        void Clear(float r, float g, float b, float a);
        void Present(bool vsync);

        ID3D11Device* GetDevice() const { return m_device.Get(); }
        ID3D11DeviceContext* GetContext() const { return m_context.Get(); }

    private:
        void CreateBackbufferRTV();
        void SetViewport(U32 width, U32 height);

    private:
        HWND m_hwnd = nullptr;
        U32  m_width = 0;
        U32  m_height = 0;

        Microsoft::WRL::ComPtr<ID3D11Device> m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context;
        Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;
    };

}
