#pragma once

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {

	enum class EVENTSYSTEMLIBRARY_API EventType {
		KeyPressEvent, KeyReleaseEvent,
		MousePressEvent, MouseReleaseEvent, MouseMoveEvent, MouseScrollEvent,
		ApplicationStartEvent, ApplicationCloseEvent,
		WindowSizeEvent, WindowMinimizeEvent, WindowMaxmimizeEvent, WindowRestoreEvent, WindowGotFocus, WindowLostFocus

	};

	class EVENTSYSTEMLIBRARY_API Event
	{
	public:
		virtual std::string GetName() const = 0;
		virtual EventType GetType() const = 0;
	};
}