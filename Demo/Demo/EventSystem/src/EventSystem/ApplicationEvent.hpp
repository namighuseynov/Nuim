#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EVENTSYSTEMLIBRARY_API ApplicationCloseEvent :
		public Event
	{
	public:
		std::string GetName() const override {
			return "ApplicationClose";
		}
		EventType GetType() {
			return EventType::ApplicationCloseEvent;
		}
	};

	class EVENTSYSTEMLIBRARY_API ApplicationStartEvent :
		public Event
	{
		std::string GetName() const override {
			return "ApplicationStart";
		}
		EventType GetType() {
			return EventType::ApplicationStartEvent;
		}
	};
}