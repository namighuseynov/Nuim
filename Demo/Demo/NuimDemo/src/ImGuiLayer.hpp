#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "Renderer.hpp"

namespace NuimDemo {
    class ImGuiRenderer {
    public:
        ImGuiRenderer(Renderer* renderer) : renderer(renderer) {}

        void Initialize() {
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();

            // Setup Platform/Renderer backends
            ImGui_ImplWin32_Init(renderer->GetWnd()->GetHWND());
            ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetContext());
        }

        void BeginFrame() {
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();
        }

        void EndFrame() {
            ImGui::Render();
            renderer->BeginRender();  // Clear + RTV
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
            renderer->EndRender();    // SwapChain Present
        }

        void ShutDown() {
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
    private:
        Renderer* renderer = nullptr;
    };
}