#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {

	class EVENTSYSTEMLIBRARY_API WindowSizeEvent :
		public Event {
	public:
		WindowSizeEvent(UINT width, UINT height) : width(width), height(height) {};
	public:
		std::string GetName() const override {
			return "WindowSize";
		}
		EventType GetType() const override {
			return EventType::WindowSizeEvent;
		}
		UINT GetWidth() {
			return width;
		}
		UINT GetHeight() {
			return height;
		}
	private:
		UINT width;
		UINT height;

	};

	class EVENTSYSTEMLIBRARY_API WindowMinimizeEvent :
		public Event {
	public:
		WindowMinimizeEvent() {};
	public:
		std::string GetName() const override {
			return "WindowMinimize";
		}
		EventType GetType() const override {
			return EventType::WindowMinimizeEvent;
		}
	private:
	};

	class EVENTSYSTEMLIBRARY_API WindowRestoreEvent :
		public Event {
	public:
		WindowRestoreEvent() {};
	public:
		std::string GetName() const override {
			return "WindowRestore";
		}
		EventType GetType() const override {
			return EventType::WindowRestoreEvent;
		}
	private:
	};

	class EVENTSYSTEMLIBRARY_API WindowMaxmimizeEvent :
		public Event {
	public:
		WindowMaxmimizeEvent() {};
	public:
		std::string GetName() const override {
			return "WindowMaxmimize";
		}
		EventType GetType() const override {
			return EventType::WindowMaxmimizeEvent;
		}
	private:
	};

	class EVENTSYSTEMLIBRARY_API WindowGotFocusEvent :
		public Event {
	public:
		WindowGotFocusEvent() {};
	public:
		std::string GetName() const override {
			return "WindowGotFocus";
		}
		EventType GetType() const override {
			return EventType::WindowGotFocus;
		}
	private:
	};

	class EVENTSYSTEMLIBRARY_API WindowLostFocusEvent :
		public Event {
	public:
		WindowLostFocusEvent() {};
	public:
		std::string GetName() const override {
			return "WindowLostFocus";
		}
		EventType GetType() const override {
			return EventType::WindowLostFocus;
		}
	private:
	};
}