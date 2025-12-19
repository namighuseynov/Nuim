#pragma once
#include "Core/Base.hpp"
#include "Core/Events/Event.hpp"


namespace Nuim {
	class KeyPressEvent :
		public Event 
	{
	public:
		KeyPressEvent(U32 keyCode) : keyCode(keyCode) {}
	public:
		std::string GetName() const override {
			return "KeyPress";
		}
		EventType GetType() const override {
			return EventType::KeyPressEvent;
		}
		const U32& GetKeyCode() {
			return keyCode;
		}
	private:
		U32 keyCode = 0;
	};

	class KeyReleaseEvent :
		public Event
	{
	public:
		KeyReleaseEvent(U32 keyCode) : keyCode(keyCode) {};
	public:
		std::string GetName() const override {
			return "KeyRelease";
		}
		EventType GetType() const override {
			return EventType::KeyReleaseEvent;
		}
		const U32& GetKeyCode() {
			return keyCode;
		}
	private:
		U32 keyCode = 0;
	};
}