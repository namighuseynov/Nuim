#include "NuimDemoPCH.h"
#include "EditorLayer.hpp"
#include "Engine.hpp"

#include "RotatorComponent.hpp"
#include "FlyCameraController.hpp"
#include "MeshRenderer.hpp"

namespace Nuim {

    static constexpr const char* kPayloadGO = "NUIM_GO_PTR";

    void EditorLayer::OnGui(Engine& engine)
    {
        DrawStats(engine.GetRenderer());
        DrawHierarchy(engine);
        DrawInspector(engine);
        DrawAssetBrowser(engine);
            
    }

    void EditorLayer::DrawHierarchy(Engine& engine) {
        Scene& scene = engine.GetScene();

        m_tfToObj.clear();
        m_roots.clear();

        for (const auto& uptr : scene.GetObjects())
            m_tfToObj[&uptr->transform] = uptr.get();

        for (const auto& uptr : scene.GetObjects())
        {
            auto* obj = uptr.get();
            if (obj->transform.GetParent() == nullptr)
                m_roots.push_back(obj);
        }

        ImGui::Begin("Hierarchy");

        // --- 3) Drop on empty space => unparent (make root) ---
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(kPayloadGO))
            {
                auto* dropped = *(GameObject**)payload->Data;
                if (dropped)
                    dropped->transform.SetParent(nullptr, true);
            }
            ImGui::EndDragDropTarget();
        }

        // --- 4) draw tree roots ---
        for (auto* root : m_roots)
            DrawHierarchyNode(engine, root);

        ImGui::End();
    }

    void EditorLayer::DrawHierarchyNode(Engine& engine, GameObject* obj) {
        if (!obj) return;

        bool selected = (m_selected == obj);

        auto& tr = obj->transform;
        const auto& children = tr.GetChildren();

        ImGuiTreeNodeFlags flags =
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_SpanAvailWidth;

        if (children.empty())
            flags |= ImGuiTreeNodeFlags_Leaf;

        if (selected)
            flags |= ImGuiTreeNodeFlags_Selected;

        // label with stable ID
        std::string label = obj->GetName() + "##" + std::to_string(obj->GetId());

        bool opened = ImGui::TreeNodeEx(label.c_str(), flags);

        // Select by click
        if (ImGui::IsItemClicked())
            m_selected = obj;

        // --- Drag source (drag this object) ---
        if (ImGui::BeginDragDropSource())
        {
            GameObject* ptr = obj;
            ImGui::SetDragDropPayload(kPayloadGO, &ptr, sizeof(GameObject*));
            ImGui::Text("%s", obj->GetName().c_str());
            ImGui::EndDragDropSource();
        }

        // --- Drop target (drop another object onto this => make it child) ---
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(kPayloadGO))
            {
                auto* dropped = *(GameObject**)payload->Data;
                if (dropped && dropped != obj)
                {
                    dropped->transform.SetParent(&obj->transform, true);
                }
            }
            ImGui::EndDragDropTarget();
        }

        // --- Context menu (optional but useful) ---
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Unparent"))
                obj->transform.SetParent(nullptr, true);

            ImGui::EndPopup();
        }

        // children recursion
        if (opened)
        {
            for (Transform* chTr : children)
            {
                auto it = m_tfToObj.find(chTr);
                if (it != m_tfToObj.end())
                    DrawHierarchyNode(engine, it->second);
            }

            ImGui::TreePop();
        }
    }

    void EditorLayer::DrawAssetBrowser(Engine& engine)
    {
        ImGui::Begin("Asset Browser");

        auto& rm = engine.GetResources();

        if (ImGui::CollapsingHeader("Meshes", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto names = rm.GetMeshNames();
            for (auto& n : names)
                ImGui::BulletText("%s", n.c_str());
        }

        if (ImGui::CollapsingHeader("Materials", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto names = rm.GetMaterialNames();
            for (auto& n : names)
                ImGui::BulletText("%s", n.c_str());
        }

        ImGui::End();
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


                if (auto mr = dynamic_cast<MeshRenderer*>(c))
                {
                    auto& rm = engine.GetResources();

                    // --- Mesh combo ---
                    auto meshNames = rm.GetMeshNames();
                    int meshIndex = -1;

                    std::string curMeshName = "(none)";
                    if (mr->GetMesh())
                    {
                        for (int i = 0; i < (int)meshNames.size(); ++i)
                            if (rm.GetMesh(meshNames[i]).get() == mr->GetMesh().get())
                                meshIndex = i;
                        if (meshIndex >= 0) curMeshName = meshNames[meshIndex];
                    }

                    if (ImGui::BeginCombo("Mesh", curMeshName.c_str()))
                    {
                        for (int i = 0; i < (int)meshNames.size(); ++i)
                        {
                            bool isSel = (i == meshIndex);
                            if (ImGui::Selectable(meshNames[i].c_str(), isSel))
                                mr->SetMesh(rm.GetMesh(meshNames[i]));
                            if (isSel) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    // --- Material combo ---
                    auto matNames = rm.GetMaterialNames();
                    int matIndex = -1;

                    std::string curMatName = "(none)";
                    if (mr->GetMaterial())
                    {
                        for (int i = 0; i < (int)matNames.size(); ++i)
                            if (rm.GetMaterial(matNames[i]).get() == mr->GetMaterial().get())
                                matIndex = i;
                        if (matIndex >= 0) curMatName = matNames[matIndex];
                    }

                    if (ImGui::BeginCombo("Material", curMatName.c_str()))
                    {
                        for (int i = 0; i < (int)matNames.size(); ++i)
                        {
                            bool isSel = (i == matIndex);
                            if (ImGui::Selectable(matNames[i].c_str(), isSel))
                                mr->SetMaterial(rm.GetMaterial(matNames[i]));
                            if (isSel) ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
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
