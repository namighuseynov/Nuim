#pragma once
#include "Event.hpp"

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EVENTSYSTEMLIBRARY_API KeyReleaseEvent :
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