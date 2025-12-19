#include "EditorLayer.hpp"

#include "imgui.h"

namespace NuimEditor {

    EditorLayer::EditorLayer()
        : Nuim::Layer("EditorLayer")
    {
    }

    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(float /*dt*/)
    {
    }

    void EditorLayer::OnImGuiRender()
    {
        // DockSpace
        static bool dockspaceOpen = true;

        ImGuiWindowFlags windowFlags =
            ImGuiWindowFlags_MenuBar |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGui::Begin("NuimDockSpace", &dockspaceOpen, windowFlags);

        ImGui::PopStyleVar(2);

        ImGuiID dockspaceID = ImGui::GetID("NuimDockspaceID");
        ImGui::DockSpace(dockspaceID, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);

        // Menu
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ImGui::MenuItem("New");
                ImGui::MenuItem("Open");
                ImGui::MenuItem("Save");
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Test windows
        ImGui::Begin("Hierarchy");
        ImGui::Text("Scene objects will be here");
        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::Text("Selected object properties");
        ImGui::End();

        ImGui::Begin("Viewport");
        ImGui::Text("Render view will be here (later)");
        ImGui::End();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Nuim::Event& /*e*/)
    {
        
    }

} 
