#include "NuimDemoPCH.h"
#include "Engine.hpp"
#include "RotatorComponent.hpp"
#include "FlyCameraController.hpp"
#include "MeshRenderer.hpp"

namespace Nuim {
	bool Engine::Init(const EngineConfig& config)
	{
		m_config = config;

		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		Time::Init();
		Input::Init();

		m_window = std::make_unique<Window>(m_config.width, m_config.height);
		m_window->SetEventCallback(
			std::bind(&Engine::OnEvent, this, std::placeholders::_1)
		);

		m_renderer = std::make_unique<Renderer>();
		if (!m_renderer->Init(
			m_window->GetHWND(),
			m_window->GetWidth(),
			m_window->GetHeight()
		))
		{
			return false;
		}

		m_imgui = std::make_unique<ImGuiRenderer>(
			m_window->GetHWND(),
			m_renderer->GetDevice(),
			m_renderer->GetContext()
		);

		m_running = true;
		return true;
	}

	void Engine::Run()
	{
		while (m_running) {
			Time::Tick();
			Input::NewFrame();

			float dt = Time::GetDeltaTime();

			ProcessEvents();
			Update(dt);
			LateUpdate(dt);
			Render();
		}

		Shutdown();
	}

	void Engine::LoadScene(std::unique_ptr<IScene> scene)
	{
		if (m_activeScene)
			m_activeScene->OnUnload();

		m_scene.Clear();

		m_activeScene = std::move(scene);

		if (m_activeScene)
			m_activeScene->OnLoad(*this);
	}

	void Engine::Shutdown()
	{
		m_imgui.reset();
		m_renderer.reset(); 
		m_window.reset();
	}

	void Engine::ProcessEvents()
	{
		MSG msg;
		while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				m_running = false;
		}
	}

	void Engine::DrawEditorUI()
	{
		ImGui::Begin("Engine Stats");

		float dt = Time::GetDeltaTime();
		float fps = (dt > 0.0f) ? (1.0f / dt) : 0.0f;

		ImGui::Text("FPS: %.1f", fps);
		ImGui::Text("dt: %.3f ms", dt * 1000.0f);

		const auto& st = m_renderer->GetStats();
		ImGui::Separator();
		ImGui::Text("Draw Calls: %u", st.drawCalls);
		ImGui::Text("Triangles:  %u", st.triangles);

		ImGui::End();

		// --- Hierarchy ---
		ImGui::Begin("Hierarchy");

		for (const auto& uptr : m_scene.GetObjects())
		{
			GameObject* obj = uptr.get();
			bool selected = (m_selected == obj);

			std::string label = obj->GetName() + "##" + std::to_string(obj->GetId());

			if (ImGui::Selectable(label.c_str(), selected))
				m_selected = obj;
		}

		ImGui::End();

		// --- Inspector ---
		ImGui::Begin("Inspector");

		if (!m_selected)
		{
			ImGui::Text("Select an object in Hierarchy.");
			ImGui::End();
			return;
		}

		// Name
		char nameBuf[128];
		std::strncpy(nameBuf, m_selected->GetName().c_str(), sizeof(nameBuf));
		nameBuf[sizeof(nameBuf) - 1] = '\0';

		if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf)))
			m_selected->SetName(nameBuf);

		ImGui::Separator();
		ImGui::Text("Transform");

		// Position
		auto pos = m_selected->transform.GetPosition();
		float p[3] = { pos.x, pos.y, pos.z };
		if (ImGui::DragFloat3("Position", p, 0.05f))
			m_selected->transform.SetPosition({ p[0], p[1], p[2] });

		// Rotation (Quaternion) 
		auto rot = m_selected->transform.GetRotation();
		float q[4] = { rot.x, rot.y, rot.z, rot.w };
		if (ImGui::DragFloat4("Rotation (quat)", q, 0.01f))
			m_selected->transform.SetRotation({ q[0], q[1], q[2], q[3] });

		// Scale
		auto sc = m_selected->transform.GetScale();
		float s[3] = { sc.x, sc.y, sc.z };
		if (ImGui::DragFloat3("Scale", s, 0.05f))
			m_selected->transform.SetScale({ s[0], s[1], s[2] });

		ImGui::Separator();
		ImGui::Text("Components");

		// List of components
		for (const auto& cptr : m_selected->GetComponents())
		{
			Component* c = cptr.get();
			bool enabled = c->IsEnabled();

			// Header of component
			std::string header = std::string(c->GetTypeName()) + "##" + std::to_string((uintptr_t)c);

			if (ImGui::CollapsingHeader(header.c_str()))
			{
				if (ImGui::Checkbox("Enabled", &enabled))
					c->SetEnabled(enabled);

				if (auto rot = dynamic_cast<RotatorComponent*>(c))
				{
					float sp = rot->GetSpeed();
					if (ImGui::DragFloat("Speed (rad/s)", &sp, 0.05f))
						rot->SetSpeed(sp);
				}

			}
		}

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
				auto mesh = m_resources.GetMesh("Cube");
				auto mat = m_resources.GetMaterial("VertexColor");
				if (mesh && mat)
					m_selected->AddComponent<MeshRenderer>(mesh, mat);
			}

			ImGui::EndPopup();
		}


		ImGui::End();
	}


	void Engine::Update(float dt)
	{
		m_scene.Update(dt);
	}

	void Engine::LateUpdate(float dt)
	{
		m_scene.LateUpdate(dt);
	}

	void Engine::Render()
	{
		float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		m_imgui->BeginFrame();

		DrawEditorUI();

		m_renderer->BeginFrame(clearColor);
		m_scene.Render(m_renderer.get());

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_renderer->EndFrame();
	}

	void Engine::OnEvent(EventSystem::Event& e)
	{
		using namespace EventSystem;

		switch (e.GetType())
		{
		case EventType::WindowSizeEvent:
		{
			auto& ws = static_cast<WindowSizeEvent&>(e);
			OnWindowResize(ws.GetWidth(), ws.GetHeight());
			break;
		}
		case EventType::MousePressEvent:
		{
			auto& me = static_cast<MousePressEvent&>(e);
			auto btn = me.GetMouseButton(); // int

			if (btn == (int)EventSystem::MouseButton::NM_LEFT)
				Nuim::Input::OnMouseButtonDown(Nuim::MouseButton::Left);
			else if (btn == (int)EventSystem::MouseButton::NM_RIGHT)
				Nuim::Input::OnMouseButtonDown(Nuim::MouseButton::Right);
			else if (btn == (int)EventSystem::MouseButton::NM_MIDDLE)
				Nuim::Input::OnMouseButtonDown(Nuim::MouseButton::Middle);

			break;
		}
		case EventType::KeyReleaseEvent:
		{
			auto& ke = static_cast<KeyReleaseEvent&>(e);
			int keyCode = ke.GetKeyCode();
			Nuim::Input::OnKeyUp(keyCode);
			break;
		}
		case EventType::KeyPressEvent:
		{
			auto& ke = static_cast<KeyPressEvent&>(e);
			int keyCode = ke.GetKeyCode();
			Nuim::Input::OnKeyDown(keyCode);
			break;
		}

		case EventType::MouseMoveEvent:
		{
			auto& me = static_cast<MouseMoveEvent&>(e);
			Nuim::Input::OnMouseMove(me.GetX(), me.GetY());
			break;
		}
		case EventType::MouseReleaseEvent:
		{
			auto& me = static_cast<MouseReleaseEvent&>(e);
			auto btn = me.GetMouseButton();

			if (btn == (int)EventSystem::MouseButton::NM_LEFT)
				Nuim::Input::OnMouseButtonUp(Nuim::MouseButton::Left);
			else if (btn == (int)EventSystem::MouseButton::NM_RIGHT)
				Nuim::Input::OnMouseButtonUp(Nuim::MouseButton::Right);
			else if (btn == (int)EventSystem::MouseButton::NM_MIDDLE)
				Nuim::Input::OnMouseButtonUp(Nuim::MouseButton::Middle);
			break;
		}
		case EventType::MouseMiddleButtonDownEvent:
		{
			Nuim::Input::OnMouseButtonDown(Nuim::MouseButton::Middle);
			break;
		}
		case EventType::MouseMiddleButtonReleaseEvent:
		{
			Nuim::Input::OnMouseButtonUp(Nuim::MouseButton::Middle);
			break;
		}
		default:
			break;
		}
	}

	void Engine::OnWindowResize(uint32_t w, uint32_t h)
	{
		if (w == 0 || h == 0) return;

		if (m_renderer)
			m_renderer->Resize(w, h);

		if (auto cam = m_scene.GetMainCamera())
			cam->OnResize(w, h);
	}
}