#pragma once
#include <string>
#include "Base.hpp"

namespace Nuim {

	enum class EventType {
		KeyPressEvent, KeyReleaseEvent,
		MousePressEvent, MouseReleaseEvent, MouseMoveEvent, MouseMiddleButtonDownEvent, MouseMiddleButtonReleaseEvent, MouseScrollEvent,
		ApplicationStartEvent, ApplicationCloseEvent,
		WindowSizeEvent, WindowMinimizeEvent, WindowMaxmimizeEvent, WindowRestoreEvent, WindowGotFocus, WindowLostFocus

	};

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual std::string GetName() const = 0;
		virtual EventType GetType() const = 0;

		bool Handled = false;
	};
}