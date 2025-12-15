#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "Renderer/Renderer.hpp"

namespace Nuim {
    class ImGuiRenderer {
    public:
        ImGuiRenderer(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) { Initialize(hwnd, device, context); }

        bool Initialize(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context) {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
            io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

            ImGui::StyleColorsDark();

            ImGuiStyle& style = ImGui::GetStyle();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                style.WindowRounding = 0.0f;
                style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            }

            ImGui_ImplWin32_Init(hwnd);
            ImGui_ImplDX11_Init(device, context);

            return true;
        }

        void BeginFrame() {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
        }

        void EndFrame(ID3D11DeviceContext* ctx) {
            ImGui::Render();
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                ID3D11RenderTargetView* oldRTV = nullptr;
                ID3D11DepthStencilView* oldDSV = nullptr;
                ctx->OMGetRenderTargets(1, &oldRTV, &oldDSV);

                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();

                ctx->OMSetRenderTargets(1, &oldRTV, oldDSV);
                if (oldRTV) oldRTV->Release();
                if (oldDSV) oldDSV->Release();
            }
        }

        void ShutDown() {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
    };
}