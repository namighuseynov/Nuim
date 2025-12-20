#pragma once
#include "Render/ISwapChain.hpp"
#include "Render/RenderTypes.hpp"
#include "DX11/DX11Common.hpp"

namespace Nuim::DX11 {

    class DX11RenderContext;

    class DX11SwapChain final : public Nuim::Render::ISwapChain
    {
    public:
        DX11SwapChain(DX11RenderContext& ctx, const Nuim::Render::SwapChainDesc& desc);
        ~DX11SwapChain() override = default;

        void Resize(Nuim::U32 width, Nuim::U32 height) override;
        void Present() override;

        Nuim::U32 GetWidth() const override { return m_width; }
        Nuim::U32 GetHeight() const override { return m_height; }

        void SetVSync(bool enabled) override { m_vsync = enabled; }
        bool IsVSync() const override { return m_vsync; }

        void* GetNativeBackbufferRTV() const override { return (void*)m_backbufferRTV.Get(); }

        IDXGISwapChain* GetSwapChain() const { return m_swapChain.Get(); }

    private:
        void CreateSwapChain(HWND hwnd);
        void CreateBackbufferRTV();
        void CleanupBackbuffer();

    private:
        DX11RenderContext& m_ctx;

        ComPtr<IDXGISwapChain> m_swapChain;
        ComPtr<ID3D11RenderTargetView> m_backbufferRTV;

        Nuim::U32 m_width = 0;
        Nuim::U32 m_height = 0;
        bool m_vsync = true;

        HWND m_hwnd = nullptr;
    };

} 
