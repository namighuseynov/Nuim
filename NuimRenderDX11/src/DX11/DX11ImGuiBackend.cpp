#include "NuimPCH.h"
#include "DX11/DX11RenderContext.hpp"
#include "DX11/DX11ImGuiBackend.hpp"
#include "Core/Base.hpp"

#ifdef NUIM_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <stdexcept>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

namespace Nuim::DX11 {

    void DX11ImGuiBackend::Init(void* nativeWindowHandle, void* renderContextNative)
    {
        if (m_initialized)
            return;

        auto hwnd = (HWND)nativeWindowHandle;

        auto* rc = reinterpret_cast<Nuim::Render::IRenderContext*>(renderContextNative);
        if (!hwnd || !rc)
            throw std::runtime_error("DX11ImGuiBackend::Init: invalid hwnd/renderContext");

        auto* device = (ID3D11Device*)rc->GetNativeDevice();
        auto* ctx = (ID3D11DeviceContext*)rc->GetNativeContext();

        if (!device || !ctx)
            throw std::runtime_error("DX11ImGuiBackend::Init: invalid device/context");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Viewports better to turn of 
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(device, ctx);

        m_initialized = true;
    }


    void DX11ImGuiBackend::Shutdown()
    {
        if (!m_initialized)
            return;

        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();

        m_initialized = false;
    }

    void DX11ImGuiBackend::BeginFrame()
    {
        if (!m_initialized) return;

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void DX11ImGuiBackend::EndFrame()
    {
        if (!m_initialized) return;

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    bool DX11ImGuiBackend::HandleNativeMessage(void* hwnd, uint32_t msg, uint64_t wparam, int64_t lparam)
    {
        if (!m_initialized) return false;
        return ImGui_ImplWin32_WndProcHandler((HWND)hwnd, (UINT)msg, (WPARAM)wparam, (LPARAM)lparam) != 0;
    }

    std::unique_ptr<Render::IImGuiBackend> CreateImGuiBackendDX11()
    {
        return std::make_unique<DX11ImGuiBackend>();
    }

} 
