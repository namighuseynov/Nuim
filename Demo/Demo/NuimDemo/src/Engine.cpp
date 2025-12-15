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

		m_mode = EngineMode::Editor;

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

	void Engine::Update(float dt)
	{
		bool isEditor = (m_mode == EngineMode::Editor);
		m_scene.Update(dt, isEditor);
	}

	void Engine::LateUpdate(float dt)
	{
		bool isEditor = (m_mode == EngineMode::Editor);
		m_scene.LateUpdate(dt, isEditor);
	}

	void Engine::Render()
	{
		float clearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

		m_imgui->BeginFrame();

		m_editor.OnGui(*this);

		m_renderer->BeginFrame(clearColor);
		m_scene.Render(m_renderer.get());

		m_imgui->EndFrame(m_renderer->GetContext());

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