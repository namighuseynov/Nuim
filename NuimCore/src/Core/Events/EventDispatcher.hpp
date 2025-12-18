#pragma once
#include "Event.hpp"
#include <functional>

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EventDispatcher
	{
	public:
		using Listener = std::function<void(Event&)>;
		void EVENTSYSTEMLIBRARY_API RegisterListener(const std::string& eventType, Listener listener);
		void EVENTSYSTEMLIBRARY_API Dispatch(Event& e);
	private:
		std::unordered_map<std::string, std::vector<Listener>> listeners;
	};
}