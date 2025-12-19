#pragma once
#include "Base.hpp"
#include "Event.hpp"

namespace Nuim {

	enum KeyCode {
		NM_KEY_Q = 81,
		NM_KEY_W = 87,
		NM_KEY_E = 69,
		NM_KEY_R = 82,
		NM_KEY_T = 84,
		NM_KEY_Y = 89,
		NM_KEY_U = 85,
		NM_KEY_I = 73,
		NM_KEY_O = 79,
		NM_KEY_P = 80,
		NM_KEY_A = 65,
		NM_KEY_S = 83,
		NM_KEY_D = 68,
		NM_KEY_F = 70,
		NM_KEY_G = 71,
		NM_KEY_H = 72,
		NM_KEY_J = 74,
		NM_KEY_K = 75,
		NM_KEY_L = 76,
		NM_KEY_Z = 90,
		NM_KEY_X = 88,
		NM_KEY_C = 67,
		NM_KEY_V = 86,
		NM_KEY_B = 66,
		NM_KEY_N = 78,
		NM_KEY_M = 77,
	};

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