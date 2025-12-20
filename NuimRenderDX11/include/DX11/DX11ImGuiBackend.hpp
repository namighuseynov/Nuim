#pragma once
#include <memory>

#include "Render/IImGuiBackend.hpp"

namespace Nuim::DX11 {

    class DX11ImGuiBackend final : public Render::IImGuiBackend {
    public:
        DX11ImGuiBackend() = default;
        ~DX11ImGuiBackend() override = default;

        void Init(void* nativeWindowHandle, void* renderContextNative) override;
        void Shutdown() override;

        void BeginFrame() override;
        void EndFrame() override;

        bool HandleNativeMessage(void* hwnd, uint32_t msg, uint64_t wparam, int64_t lparam) override;

        bool IsInitialized() const override { return m_initialized; }

    private:
        bool m_initialized = false;
    };

    std::unique_ptr<Render::IImGuiBackend> CreateImGuiBackendDX11();

} 
