#include "NuimDemoPCH.h"
#include "EditorLayer.hpp"
#include "Engine.hpp"

#include "RotatorComponent.hpp"
#include "FlyCameraController.hpp"
#include "MeshRenderer.hpp"

namespace Nuim {

    void EditorLayer::OnGui(Engine& engine)
    {
        DrawStats(engine.GetRenderer());
        DrawHierarchy(engine.GetScene());
        DrawInspector(engine);
    }

    void EditorLayer::DrawStats(Renderer* renderer)
    {
        ImGui::Begin("Engine Stats");

        float dt = Time::GetDeltaTime();
        float fps = (dt > 0.0f) ? (1.0f / dt) : 0.0f;

        ImGui::Text("FPS: %.1f", fps);
        ImGui::Text("dt: %.3f ms", dt * 1000.0f);

        if (renderer) {
            const auto& st = renderer->GetStats();
            ImGui::Separator();
            ImGui::Text("Draw Calls: %u", st.drawCalls);
            ImGui::Text("Triangles:  %u", st.triangles);
        }

        ImGui::End();
    }

    void EditorLayer::DrawHierarchy(Scene& scene)
    {
        ImGui::Begin("Hierarchy");

        for (const auto& uptr : scene.GetObjects())
        {
            GameObject* obj = uptr.get();
            bool selected = (m_selected == obj);

            std::string label = obj->GetName() + "##" + std::to_string(obj->GetId());
            if (ImGui::Selectable(label.c_str(), selected))
                m_selected = obj;
        }

        ImGui::End();
    }

    void EditorLayer::DrawInspector(Engine& engine)
    {
        ImGui::Begin("Inspector");

        if (!m_selected)
        {
            ImGui::Text("Select an object in Hierarchy.");
            ImGui::End();
            return;
        }

        // --- Name ---
        char nameBuf[128];
        std::strncpy(nameBuf, m_selected->GetName().c_str(), sizeof(nameBuf));
        nameBuf[sizeof(nameBuf) - 1] = '\0';
        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf)))
            m_selected->SetName(nameBuf);

        // --- Transform ---
        ImGui::Separator();
        ImGui::Text("Transform");

        auto pos = m_selected->transform.GetPosition();
        float p[3] = { pos.x, pos.y, pos.z };
        if (ImGui::DragFloat3("Position", p, 0.05f))
            m_selected->transform.SetPosition({ p[0], p[1], p[2] });

        auto rot = m_selected->transform.GetRotation();
        float q[4] = { rot.x, rot.y, rot.z, rot.w };
        if (ImGui::DragFloat4("Rotation (quat)", q, 0.01f))
            m_selected->transform.SetRotation({ q[0], q[1], q[2], q[3] });

        auto sc = m_selected->transform.GetScale();
        float s[3] = { sc.x, sc.y, sc.z };
        if (ImGui::DragFloat3("Scale", s, 0.05f))
            m_selected->transform.SetScale({ s[0], s[1], s[2] });

        // --- Components ---
        ImGui::Separator();
        ImGui::Text("Components");

        for (const auto& cptr : m_selected->GetComponents())
        {
            Component* c = cptr.get();
            bool enabled = c->IsEnabled();

            std::string header = std::string(c->GetTypeName()) + "##" + std::to_string((uintptr_t)c);
            if (ImGui::CollapsingHeader(header.c_str()))
            {
                if (ImGui::Checkbox("Enabled", &enabled))
                    c->SetEnabled(enabled);

                if (auto rotC = dynamic_cast<RotatorComponent*>(c))
                {
                    float sp = rotC->GetSpeed();
                    if (ImGui::DragFloat("Speed (rad/s)", &sp, 0.05f))
                        rotC->SetSpeed(sp);
                }
            }
        }

        // --- Add Component popup ---
        if (ImGui::Button("Add Component..."))
            ImGui::OpenPopup("AddComponentPopup");

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            if (ImGui::MenuItem("RotatorComponent"))
                m_selected->AddComponent<RotatorComponent>(1.0f);

            if (ImGui::MenuItem("FlyCameraController"))
                m_selected->AddComponent<FlyCameraController>(4.0f, 6.0f);

            if (ImGui::MenuItem("MeshRenderer (Cube/VertexColor)"))
            {
                auto mesh = engine.GetResources().GetMesh("Cube");
                auto mat = engine.GetResources().GetMaterial("VertexColor");
                if (mesh && mat)
                    m_selected->AddComponent<MeshRenderer>(mesh, mat);
            }

            ImGui::EndPopup();
        }

        ImGui::End();
    }

}
