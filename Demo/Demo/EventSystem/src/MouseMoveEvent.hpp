#pragma once
#include "Event.hpp"

namespace EventSystem {
	class MouseMoveEvent :
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
