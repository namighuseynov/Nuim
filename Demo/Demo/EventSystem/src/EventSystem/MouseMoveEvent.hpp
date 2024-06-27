#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EVENTSYSTEMLIBRARY_API MouseMoveEvent :
		public Event
	{
	public:
		MouseMoveEvent() {}
	public:
		std::string GetName() const override {
			return "MouseMove";
		}
	};
}
