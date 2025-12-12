#pragma once

namespace NuimDemo {

	enum class MouseButton {
		Left = 0,
		Right = 1,
		Middle = 2
	};

	class Input
	{
	public:
		static void Init();
		static void NewFrame();

		static void OnKeyDown(int keycode);
		static void OnKeyUp(int keycode);
		
		static void OnMouseMove(int x, int y);
		static void OnMouseButtonDown(MouseButton button);
		static void OnMouseButtonUp(MouseButton button);

		static bool IsKeyDown(int keycode);
		static bool IsKeyPressed(int keycode);
		static bool IsKeyReleased(int keycode);

		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		static void GetMousePosition(int& x, int& y);
		static void GetMouseDelta(int& dx, int& dy);

	private:
		static constexpr int s_maxKeys = 256;

		static bool s_keysDown[s_maxKeys];
		static bool s_keysPressed[s_maxKeys];
		static bool s_keysReleased[s_maxKeys];

		static bool s_mouseButtonDown[3];
		static bool s_mouseButtonPressed[3];
		static bool s_mouseButtonUp[3];

		static int s_mouseX;
		static int s_mouseY;
		static int s_mouseDeltaX;
		static int s_mouseDeltaY;

		static bool s_initialized;
	};
}