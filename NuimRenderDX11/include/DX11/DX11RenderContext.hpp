#pragma once
#include "Render/IRenderContext.hpp"
#include "DX11/DX11Common.hpp"

namespace Nuim::DX11 {

    class DX11RenderContext final : public Nuim::Render::IRenderContext
    {
    public:
        DX11RenderContext();
        ~DX11RenderContext() override = default;

        Nuim::Render::Backend GetBackend() const override { return Nuim::Render::Backend::DX11; }

        void* GetNativeDevice() const override { return (void*)m_device.Get(); }
        void* GetNativeContext() const override { return (void*)m_context.Get(); }

        ID3D11Device* GetDevice() const { return m_device.Get(); }
        ID3D11DeviceContext* GetContext() const { return m_context.Get(); }

    private:
        void CreateDevice();

    private:
        ComPtr<ID3D11Device>        m_device;
        ComPtr<ID3D11DeviceContext> m_context;
    };

}
