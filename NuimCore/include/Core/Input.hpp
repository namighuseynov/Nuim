#pragma once
#include "Base.hpp"
#include "InputCodes.hpp"

namespace Nuim {
	class Input
	{
	public:
		static void Init();
		static void NewFrame();

		static void OnKeyDown(KeyCode key);
		static void OnKeyUp(KeyCode key);

		static bool IsKeyDown(KeyCode key);
		static bool IsKeyPressed(KeyCode key);
		static bool IsKeyReleased(KeyCode key);

		static void OnMouseMove(U32 x, U32 y);
		static void OnMouseButtonDown(MouseButton button);
		static void OnMouseButtonUp(MouseButton button);

		static bool IsMouseButtonDown(MouseButton button);
		static bool IsMouseButtonPressed(MouseButton button);
		static bool IsMouseButtonReleased(MouseButton button);

		static void GetMousePosition(U32& x, U32& y);
		static void GetMouseDelta(I32& dx, I32& dy);

	private:
		static constexpr U32 s_maxKeys = 1024;

		static U32 ToIndex(KeyCode key) { return (U16)key; }

		static bool s_keysDown[s_maxKeys];
		static bool s_keysPressed[s_maxKeys];
		static bool s_keysReleased[s_maxKeys];

		static bool s_mouseButtonDown[3];
		static bool s_mouseButtonPressed[3];
		static bool s_mouseButtonUp[3];

		static U32 s_mouseX;
		static U32 s_mouseY;
		static I32 s_mouseDeltaX;
		static I32 s_mouseDeltaY;

		static bool s_initialized;
	};
}