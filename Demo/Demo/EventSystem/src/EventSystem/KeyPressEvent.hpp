#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EVENTSYSTEMLIBRARY_API KeyPressEvent :
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
