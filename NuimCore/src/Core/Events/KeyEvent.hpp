#pragma once
#include "Event.hpp"

namespace EventSystem {

	class KeyPressEvent :
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

	class KeyReleaseEvent :
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