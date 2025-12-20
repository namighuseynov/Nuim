#include "EditorLayer.hpp"

#include "imgui.h"

namespace NuimEditor {

    EditorLayer::EditorLayer()
        : Nuim::Layer("EditorLayer") {}

    void EditorLayer::OnAttach() {
        m_scene = std::make_unique<Nuim::World::Scene>();
    }
    void EditorLayer::OnDetach() {
        if (m_scene && m_scene->IsRunning())
            m_scene->OnRuntimeStop();

        m_scene.reset();
    }

    void EditorLayer::TogglePlayStop()
    {
        if (!m_scene)
            return;

        m_playMode = !m_playMode;
        if (m_playMode) m_scene->OnRuntimeStart();
        else            m_scene->OnRuntimeStop();
    }


    void EditorLayer::OnUpdate(float dt) {
        if (m_scene)
            m_scene->Update(dt);
    }

    void EditorLayer::OnEvent(Nuim::Event& e) {
        if (m_scene)
            m_scene->DispatchEvent(e);

        (void)e;
    }

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

        ImGui::Begin("Runtime");

        if (!m_scene)
        {
            ImGui::Text("Scene: null");
            ImGui::End();
        }
        else
        {
            if (!m_playMode)
            {
                if (ImGui::Button("Play"))
                    TogglePlayStop();
            }
            else
            {
                if (ImGui::Button("Stop"))
                    TogglePlayStop();
            }

            ImGui::SameLine();
            ImGui::Text("Running: %s", m_scene->IsRunning() ? "true" : "false");

            ImGui::End();
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
