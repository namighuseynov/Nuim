#include "NuimDemoPCH.h"
#include "Input.hpp"
#include <cstring>

namespace NuimDemo {

	bool Input::s_keysDown[s_maxKeys];
	bool Input::s_keysPressed[s_maxKeys];
	bool Input::s_keysReleased[s_maxKeys];

	bool Input::s_mouseButtonDown[3];
	bool Input::s_mouseButtonPressed[3];
	bool Input::s_mouseButtonUp[3];

	int  Input::s_mouseX		= 0;
	int  Input::s_mouseY		= 0;
	int  Input::s_mouseDeltaX	= 0;
	int  Input::s_mouseDeltaY	= 0;

	bool Input::s_initialized = false;

	static int MouseButtonToIndex(MouseButton button)
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

	void Input::OnKeyDown(int keycode)
	{
		if (keycode >= s_maxKeys || keycode < 0)
			return;

		if (!s_keysDown[keycode]) {
			s_keysDown[keycode]		= true;
			s_keysPressed[keycode]	= true;
		}
	}

	void Input::OnKeyUp(int keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return;

		if (s_keysDown[keycode]) {
			s_keysDown[keycode] = false;
			s_keysReleased[keycode] = true;
		}
	}

	void Input::OnMouseMove(int x, int y)
	{
		int dx = x - s_mouseX;
		int dy = y - s_mouseY;

		s_mouseX = x;
		s_mouseY = y;

		s_mouseDeltaX += dx;
		s_mouseDeltaY += dy;
	}

	void Input::OnMouseButtonDown(MouseButton button)
	{
		int index = MouseButtonToIndex(button);
		if (index < 0)
			return;

		if (!s_mouseButtonDown[index]) {
			s_mouseButtonDown[index] = true;
			s_mouseButtonPressed[index] = true;
		}
	}

	void Input::OnMouseButtonUp(MouseButton button)
	{
		int index = MouseButtonToIndex(button);
		if (index < 0) return;

		if (s_mouseButtonDown[index]) {
			s_mouseButtonDown[index] = false;
			s_mouseButtonUp[index] = true;
		}
	}

	bool Input::IsKeyDown(int keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return false;

		return s_keysDown[keycode];
	}

	bool Input::IsKeyPressed(int keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return false;
		return s_keysPressed[keycode];
	}

	bool Input::IsKeyReleased(int keycode)
	{
		if (keycode < 0 || keycode >= s_maxKeys)
			return false;

		return s_keysReleased[keycode];
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		int index = MouseButtonToIndex(button);
		if (index < 0) return false;
		return s_mouseButtonDown[index];
	}

	bool Input::IsMouseButtonPressed(MouseButton button)
	{
		int index = MouseButtonToIndex(button);
		if (index < 0) return false;
		return s_mouseButtonPressed[index];
	}

	bool Input::IsMouseButtonReleased(MouseButton button)
	{
		int index = MouseButtonToIndex(button);
		if (index < 0) return false;
		return s_mouseButtonUp[index];
	}

	void Input::GetMousePosition(int& x, int& y)
	{
		x = s_mouseX;
		y = s_mouseY;
	}

	void Input::GetMouseDelta(int& dx, int& dy)
	{
		dx = s_mouseDeltaX;
		dy = s_mouseDeltaY;
	}
}