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

        ImVec2 avail = ImGui::GetContentRegionAvail();
        Nuim::U32 vw = (Nuim::U32)(avail.x > 0 ? avail.x : 0);
        Nuim::U32 vh = (Nuim::U32)(avail.y > 0 ? avail.y : 0);

        if (vw > 0 && vh > 0 && (vw != m_lastViewportW || vh != m_lastViewportH))
        {
            m_lastViewportW = vw;
            m_lastViewportH = vh;
            if (m_requestViewportResize)
                m_requestViewportResize(vw, vh);
        }

        void* srv = m_getViewportSRV ? m_getViewportSRV() : nullptr;
        if (srv && vw > 0 && vh > 0)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
            ImGui::Image((ImTextureID)srv, ImVec2((float)vw, (float)vh));
            ImGui::PopStyleVar();
        }
        else
        {
            ImGui::Text("Viewport texture not ready...");
        }

        ImGui::End();


        ImGui::ShowDemoWindow();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Nuim::Event& /*e*/)
    {
        
    }

} 
