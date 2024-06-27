#include "pch.h"
#include "EventDispatcher.hpp"

namespace EventSystem {

	void EventDispatcher::RegisterListener(const std::string& eventType, Listener listener) {
		listeners[eventType].push_back(listener);
	}
	void EventDispatcher::Dispatch(Event& e) {
		auto item = listeners.find(e.GetName());
		for (const auto& listener : listeners[e.GetName()]) {
			listener(e);
		}
	}
}