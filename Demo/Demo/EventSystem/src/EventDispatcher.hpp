#pragma once
#include <functional>
#include <string>
#include <unordered_map>
#include "Event.hpp"

namespace EventSystem {
	class EventDispatcher
	{
	public:
		using Listener = std::function<void(Event&)>;
		void RegisterListener(const std::string& eventType, Listener listener);
		void Dispatch(Event& e);
	private:
		std::unordered_map<std::string, std::vector<Listener>> listeners;
	};

}