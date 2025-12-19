#pragma once
#include "Base.hpp"

namespace Nuim {

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

		static void OnKeyDown(U32 keycode);
		static void OnKeyUp(U32 keycode);
		
		static void OnMouseMove(U32 x, U32 y);
		static void OnMouseButtonDown(MouseButton button);
		static void OnMouseButtonUp(MouseButton button);

		static bool IsKeyDown(U32 keycode);
		static bool IsKeyPressed(U32 keycode);
		static bool IsKeyReleased(U32 keycode);

		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		static void GetMousePosition(U32& x, U32& y);
		static void GetMouseDelta(U32& dx, U32& dy);

	private:
		static constexpr U32 s_maxKeys = 256;

		static bool s_keysDown[s_maxKeys];
		static bool s_keysPressed[s_maxKeys];
		static bool s_keysReleased[s_maxKeys];

		static bool s_mouseButtonDown[3];
		static bool s_mouseButtonPressed[3];
		static bool s_mouseButtonUp[3];

		static U32 s_mouseX;
		static U32 s_mouseY;
		static U32 s_mouseDeltaX;
		static U32 s_mouseDeltaY;

		static bool s_initialized;
	};
}