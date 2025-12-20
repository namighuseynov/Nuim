#pragma once
#include "Render/ISwapChain.hpp"
#include "Render/IRenderContext.hpp"
#include "Render/RenderTypes.hpp"

#include <wrl/client.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct IDXGISwapChain;


namespace Nuim::DX11 {

    class DX11SwapChain final : public Render::ISwapChain
    {
    public:
        DX11SwapChain(Render::IRenderContext& ctx, const Render::SwapChainDesc& desc);
        ~DX11SwapChain() override = default;

        void Resize(U32 width, U32 height) override;
        void Present() override;

        U32 GetWidth() const override { return m_width; }
        U32 GetHeight() const override { return m_height; }

        void SetVSync(bool enabled) override { m_vsync = enabled; }
        bool IsVSync() const override { return m_vsync; }

        void BindBackbuffer() override;
        void ClearBackbuffer(float r, float g, float b, float a) override;

        void* GetNativeBackbufferRTV() const override { return (void*)m_rtv.Get(); }

    private:
        void CreateSwapChain(void* windowHandle);
        void CreateBackbufferRTV();
        void DestroyBackbufferRTV();

    private:
        Microsoft::WRL::ComPtr<ID3D11Device>           m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>    m_context;

        Microsoft::WRL::ComPtr<IDXGISwapChain>         m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_rtv;

        Nuim::U32 m_width = 0;
        Nuim::U32 m_height = 0;

        bool m_vsync = true;
        void* m_hwnd = nullptr;
    };

} 
