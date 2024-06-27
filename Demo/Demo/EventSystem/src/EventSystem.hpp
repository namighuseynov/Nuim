#pragma once

namespace EventSystem {
	enum MouseButton {
		NM_LEFT = 0,
		NM_RIGHT
	};
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
}

#include "Event.hpp"
#include "EventDispatcher.hpp"
#include "KeyPressEvent.hpp"
#include "KeyReleaseEvent.hpp"
#include "MousePressEvent.hpp"
#include "MouseMoveEvent.hpp"
#include "ApplicationStartEvent.hpp"
#include "ApplicationCloseEvent.hpp"