#pragma once
#include "Event.hpp"

namespace EventSystem {
	class WindowSizeEvent :
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

	class WindowMinimizeEvent :
		public Event {
	public:
	public:
	private:
	};

	class WindowMaxmimizeEvent :
		public Event {
	public:
	public:
	private:
	};
}