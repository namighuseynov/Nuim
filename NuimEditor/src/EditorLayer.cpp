#include "EditorLayer.hpp"

#include "imgui.h"

namespace NuimEditor {

    EditorLayer::EditorLayer()
        : Nuim::Layer("EditorLayer") {}

    void EditorLayer::OnAttach() {}
    void EditorLayer::OnDetach() {}
    void EditorLayer::OnUpdate(float) {}
    void EditorLayer::OnEvent(Nuim::Event&) {}

    bool EditorLayer::ConsumeViewportResize(Nuim::U32& outW, Nuim::U32& outH)
    {
        if (!m_viewportResizeRequested)
            return false;

        m_viewportResizeRequested = false;
        outW = m_pendingViewportW;
        outH = m_pendingViewportH;
        return true;
    }

    void EditorLayer::OnImGuiRender()
    {
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

        const ImGuiViewport* vp = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(vp->WorkPos);
        ImGui::SetNextWindowSize(vp->WorkSize);
        ImGui::SetNextWindowViewport(vp->ID);

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

        // Panels
        ImGui::Begin("Hierarchy");
        ImGui::Text("Scene objects will be here");
        ImGui::End();

        ImGui::Begin("Inspector");
        ImGui::Text("Selected object properties");
        ImGui::End();

        // Viewport panel
        ImGui::Begin("Viewport");

        ImVec2 avail = ImGui::GetContentRegionAvail();

        Nuim::U32 newW = (Nuim::U32)((avail.x > 1.0f) ? avail.x : 1.0f);
        Nuim::U32 newH = (Nuim::U32)((avail.y > 1.0f) ? avail.y : 1.0f);

        if (newW != m_viewportW || newH != m_viewportH)
        {
            m_viewportW = newW;
            m_viewportH = newH;

            m_pendingViewportW = newW;
            m_pendingViewportH = newH;
            m_viewportResizeRequested = true;
        }

        if (m_viewportTarget)
        {
            ImTextureID id = (ImTextureID)m_viewportTarget->GetImGuiTextureID();
            ImGui::Image(id, avail, ImVec2(0, 0), ImVec2(1, 1));
        }
        else
        {
            ImGui::Text("Viewport target is null");
        }

        ImGui::End(); // Viewport

        ImGui::End(); // Dockspace
    }

} 
