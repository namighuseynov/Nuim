#pragma once
#include "Event.hpp"

namespace EventSystem {
	class KeyPressEvent :
		public Event
	{
	public:
		KeyPressEvent(int keyCode) : keyCode(keyCode) {}
	public:
		std::string GetName() const override {
			return "KeyPress";
		}
		int GetKeyCode() {
			return keyCode;
		}
	private:
		int keyCode;
	};
}
