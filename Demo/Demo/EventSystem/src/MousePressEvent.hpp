#pragma once
#include "Event.hpp"

namespace EventSystem {
	class MousePressEvent :
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