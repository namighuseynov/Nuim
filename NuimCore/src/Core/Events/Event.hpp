#pragma once

namespace EventSystem {

	enum class EventType {
		KeyPressEvent, KeyReleaseEvent,
		MousePressEvent, MouseReleaseEvent, MouseMoveEvent, MouseMiddleButtonDownEvent, MouseMiddleButtonReleaseEvent, MouseScrollEvent,
		ApplicationStartEvent, ApplicationCloseEvent,
		WindowSizeEvent, WindowMinimizeEvent, WindowMaxmimizeEvent, WindowRestoreEvent, WindowGotFocus, WindowLostFocus

	};

	class Event
	{
	public:
		virtual std::string GetName() const = 0;
		virtual EventType GetType() const = 0;
	};
}