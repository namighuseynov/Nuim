#pragma once
#include "Event.hpp"

namespace EventSystem {
	class ApplicationStartEvent :
		public Event
	{
		std::string GetName() const override {
			return "ApplicationStart";
		}
	};
}