#pragma once
#include "Event.hpp"

namespace Nuim {

	class WindowSizeEvent :
		public Event {
	public:
		WindowSizeEvent(U32 width, U32 height) : width(width), height(height) {};
	public:
		std::string GetName() const override {
			return "WindowSize";
		}
		EventType GetType() const override {
			return EventType::WindowSizeEvent;
		}
		U32 GetWidth() const {
			return width;
		}
		U32 GetHeight() const {
			return height;
		}
	private:
		U32 width;
		U32 height;

	};

	class WindowMinimizeEvent :
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

	class WindowRestoreEvent :
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

	class WindowMaxmimizeEvent :
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

	class WindowGotFocusEvent :
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

	class WindowLostFocusEvent :
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