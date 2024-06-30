#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	
	class EVENTSYSTEMLIBRARY_API MousePressEvent :
		public Event {
	public:
		MousePressEvent(const int& mouseButton) : mouseButton(mouseButton) {};
	public:
		std::string GetName() const override {
			return "MousePress";
		}
		EventType GetType() const  override {
			return EventType::MousePressEvent; 
		}
		int GetMouseButton() {
			return mouseButton;
		}
	private:
		int mouseButton;
	};

	class EVENTSYSTEMLIBRARY_API MouseReleaseEvent :
		public Event {
	public:
		MouseReleaseEvent(const int& mouseButton) : mouseButton(mouseButton) {};
	public:
		std::string GetName() const override {
			return "MouseRelease";
		}
		EventType GetType() const  override {
			return EventType::MouseReleaseEvent;
		}
		int GetMouseButton() {
			return mouseButton;
		}
	private:
		int mouseButton;

	};

	class EVENTSYSTEMLIBRARY_API MouseMoveEvent :
		public Event {
	public:
		MouseMoveEvent() {};
	public:
		std::string GetName() const override {
			return "MouseMove";
		}
		EventType GetType() const  override {
			return EventType::MouseMoveEvent;
		}
	};

	class EVENTSYSTEMLIBRARY_API MouseScrollEvent :
		public Event {

	};
}




