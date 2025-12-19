#pragma once
#include "Nuim/Render/Renderer.hpp"
#include "Nuim/DX11/DX11Context.hpp"

namespace Nuim {

    class DX11Renderer final : public Renderer {
    public:
        explicit DX11Renderer(const RendererDesc& desc);
        ~DX11Renderer() override;

        void BeginFrame() override;
        void Clear(float r, float g, float b, float a) override;
        void EndFrame() override;

        void Resize(U32 w, U32 h) override;

    private:
        DX11Context m_ctx;
        bool m_vsync = true;
    };

}
