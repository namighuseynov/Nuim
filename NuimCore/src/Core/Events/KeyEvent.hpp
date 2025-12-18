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
		KeyPressEvent(UINT keyCode) : keyCode(keyCode) {}
	public:
		std::string GetName() const override {
			return "KeyPress";
		}
		EventType GetType() const override {
			return EventType::KeyPressEvent;
		}
		const UINT& GetKeyCode() {
			return keyCode;
		}
	private:
		UINT keyCode = 0;
	};

	class EVENTSYSTEMLIBRARY_API KeyReleaseEvent :
		public Event
	{
	public:
		KeyReleaseEvent(UINT keyCode) : keyCode(keyCode) {};
	public:
		std::string GetName() const override {
			return "KeyRelease";
		}
		EventType GetType() const override {
			return EventType::KeyReleaseEvent;
		}
		const UINT& GetKeyCode() {
			return keyCode;
		}
	private:
		UINT keyCode = 0;
	};
}