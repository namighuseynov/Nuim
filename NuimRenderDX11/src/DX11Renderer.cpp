#include "NuimPCH.h"
#include "Core/Assert.hpp"
#include "Nuim/DX11/DX11Renderer.hpp"
#include "Core/Window.hpp"

namespace Nuim {

    DX11Renderer::DX11Renderer(const RendererDesc& desc)
    {
        NUIM_ASSERT(desc.WindowHandle != nullptr, "RendererDesc.WindowHandle is null");

        void* hwnd = desc.WindowHandle->GetNativeHandle();
        m_vsync = desc.WindowHandle->IsVSync();

        m_ctx.Init(hwnd, desc.WindowHandle->GetWidth(), desc.WindowHandle->GetHeight(), desc.EnableDebug);
    }

    DX11Renderer::~DX11Renderer()
    {
        m_ctx.Shutdown();
    }

    void DX11Renderer::BeginFrame()
    {
        
    }

    void DX11Renderer::Clear(float r, float g, float b, float a)
    {
        m_ctx.Clear(r, g, b, a);
    }

    void DX11Renderer::EndFrame()
    {
        m_ctx.Present(m_vsync);
    }

    void DX11Renderer::Resize(U32 w, U32 h)
    {
        m_ctx.Resize(w, h);
    }

}
