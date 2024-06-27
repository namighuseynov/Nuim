#pragma once
#include "Event.hpp"

namespace EventSystem {
	class KeyReleaseEvent :
		public Event
	{
	public:
		KeyReleaseEvent(const int& keyCode) : keyCode(keyCode) {};
	public:
		std::string GetName() const override {
			return "KeyRelease";
		}
		int GetKeyCode() {
			return this->keyCode;
		}
	private:
		int keyCode;
	};

}