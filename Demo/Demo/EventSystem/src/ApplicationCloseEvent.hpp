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
	};

}