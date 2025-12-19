#include "NuimPCH.h"
#include "Input.hpp"
#include <cstring>

namespace Nuim {

	bool Input::s_keysDown[s_maxKeys];
	bool Input::s_keysPressed[s_maxKeys];
	bool Input::s_keysReleased[s_maxKeys];

	bool Input::s_mouseButtonDown[3];
	bool Input::s_mouseButtonPressed[3];
	bool Input::s_mouseButtonUp[3];

	U32  Input::s_mouseX		= 0;
	U32  Input::s_mouseY		= 0;
	U32  Input::s_mouseDeltaX	= 0;
	U32  Input::s_mouseDeltaY	= 0;

	bool Input::s_initialized = false;

	static U32 MouseButtonToIndex(MouseButton button)
	{
		switch (button)
		{
		case MouseButton::Left:   return 0;
		case MouseButton::Right:  return 1;
		case MouseButton::Middle: return 2;
		default:                  return -1;
		}
	}

	void Input::Init()
	{
		if (s_initialized)
			return;

		std::memset(s_keysDown,			0, sizeof(s_keysDown));
		std::memset(s_keysPressed,		0, sizeof(s_keysPressed));
		std::memset(s_keysReleased,		0, sizeof(s_keysReleased));

		std::memset(s_mouseButtonDown,		0, sizeof(s_mouseButtonDown));
		std::memset(s_mouseButtonPressed,	0, sizeof(s_mouseButtonPressed));
		std::memset(s_mouseButtonUp,		0, sizeof(s_mouseButtonUp));

		s_mouseX = s_mouseY = 0;
		s_mouseDeltaX = s_mouseDeltaY = 0;

		s_initialized = true;
	}
	void Input::NewFrame()
	{
		std::memset(s_keysPressed, 0, sizeof(s_keysPressed));
		std::memset(s_keysReleased, 0, sizeof(s_keysReleased));

		std::memset(s_mouseButtonPressed, 0, sizeof(s_mouseButtonPressed));
		std::memset(s_mouseButtonUp, 0, sizeof(s_mouseButtonUp));

		s_mouseDeltaX = 0;
		s_mouseDeltaY = 0;
	}

	void Input::OnKeyDown(U32 keycode)
	{
		if (keycode >= s_maxKeys || keycode < 0)
			return;

		if (!s_keysDown[keycode]) {
			s_keysDown[keycode]		= true;
			s_keysPressed[keycode]	= true;
		}
	}

	void Input::OnKeyUp(U32 keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return;

		if (s_keysDown[keycode]) {
			s_keysDown[keycode] = false;
			s_keysReleased[keycode] = true;
		}
	}

	void Input::OnMouseMove(U32 x, U32 y)
	{
		U32 dx = x - s_mouseX;
		U32 dy = y - s_mouseY;

		s_mouseX = x;
		s_mouseY = y;

		s_mouseDeltaX += dx;
		s_mouseDeltaY += dy;
	}

	void Input::OnMouseButtonDown(MouseButton button)
	{
		U32 index = MouseButtonToIndex(button);
		if (index < 0)
			return;

		if (!s_mouseButtonDown[index]) {
			s_mouseButtonDown[index] = true;
			s_mouseButtonPressed[index] = true;
		}
	}

	void Input::OnMouseButtonUp(MouseButton button)
	{
		U32 index = MouseButtonToIndex(button);
		if (index < 0) return;

		if (s_mouseButtonDown[index]) {
			s_mouseButtonDown[index] = false;
			s_mouseButtonUp[index] = true;
		}
	}

	bool Input::IsKeyDown(U32 keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return false;

		return s_keysDown[keycode];
	}

	bool Input::IsKeyPressed(U32 keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return false;
		return s_keysPressed[keycode];
	}

	bool Input::IsKeyReleased(U32 keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return false;

		return s_keysReleased[keycode];
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		U32 index = MouseButtonToIndex(button);
		if (index < 0) return false;
		return s_mouseButtonDown[index];
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		U32 index = MouseButtonToIndex(button);
		if (index < 0) return false;
		return s_mouseButtonPressed[index];
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		U32 index = MouseButtonToIndex(button);
		if (index < 0) return false;
		return s_mouseButtonUp[index];
	}

	void Input::GetMousePosition(U32& x, U32& y)
	{
		x = s_mouseX;
		y = s_mouseY;
	}

	void Input::GetMouseDelta(U32& dx, U32& dy)
	{
		dx = s_mouseDeltaX;
		dy = s_mouseDeltaY;
	}
}