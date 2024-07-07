#pragma once
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/imgui_impl_win32.h"
#include "Renderer.hpp"

namespace NuimDemo {
    class ImGuiRenderer {
    public:
        ImGuiRenderer(Renderer* renderer) : renderer(renderer) {};
    public:
        void Begin() {
            // Create application window
            //ImGui_ImplWin32_EnableDpiAwareness();


            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsLight();

            // Setup Platform/Renderer backends
            ImGui_ImplWin32_Init(renderer->GetWnd()->GetHWND());
            ImGui_ImplDX11_Init(renderer->GetDevice(), renderer->GetContext());
            bool done = false;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        }
        void End() {
            // Cleanup
            ImGui_ImplDX11_Shutdown();
            ImGui_ImplWin32_Shutdown();
            ImGui::DestroyContext();
        }
        void OnUpdate() {

            // Start the Dear ImGui frame
            ImGui_ImplDX11_NewFrame();
            ImGui_ImplWin32_NewFrame();
            ImGui::NewFrame();

            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).

            ImGui::ShowDemoWindow();


            // Rendering
            ImGui::Render();
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
            const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
            auto targetView = renderer->GetTargetView();
            renderer->GetContext()->OMSetRenderTargets(1, &targetView, nullptr);
            renderer->GetContext()->ClearRenderTargetView(targetView, clear_color_with_alpha);
            ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        }
    private:
        Renderer* renderer = nullptr;
    };
}