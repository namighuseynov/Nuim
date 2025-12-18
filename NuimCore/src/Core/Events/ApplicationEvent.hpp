#pragma once
#include "Event.hpp"

namespace EventSystem {
	class ApplicationCloseEvent :
		public Event
	{
	public:
		std::string GetName() const override {
			return "ApplicationClose";
		}
		EventType GetType() const override{
			return EventType::ApplicationCloseEvent;
		}
	};

	class ApplicationStartEvent :
		public Event
	{
		std::string GetName() const override {
			return "ApplicationStart";
		}
		EventType GetType() const override {
			return EventType::ApplicationStartEvent;
		}
	};
}