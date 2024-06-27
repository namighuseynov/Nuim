#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EVENTSYSTEMLIBRARY_API MousePressEvent :
		public Event
	{
	public:
		MousePressEvent(const int& mouseButton) : mouseButton(mouseButton) {};
	public:
		std::string GetName() const override {
			return "MousePress";
		}
		int GetMouseButton() {
			return mouseButton;
		}
	private:
		int mouseButton;
	};
}