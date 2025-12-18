#include "NuimPCH.h"
#include "EventDispatcher.hpp"

namespace Nuim {

	void EventDispatcher::RegisterListener(const std::string& eventType, Listener listener) {
		listeners[eventType].push_back(listener);
	}
	void EventDispatcher::Dispatch(Event& e) {
		auto it = listeners.find(e.GetName());
		if (it != listeners.end()) {
			for (const auto& listener : it->second) {
				listener(e);
			}
		}
	}
}