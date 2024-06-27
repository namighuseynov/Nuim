#pragma once
#include "Event.hpp"
#include <functional>

#ifdef EVENTSYSTEMLIBRARY_EXPORTS
#define EVENTSYSTEMLIBRARY_API __declspec(dllimport)
#else
#define EVENTSYSTEMLIBRARY_API __declspec(dllexport)
#endif

namespace EventSystem {
	class EVENTSYSTEMLIBRARY_API EventDispatcher
	{
	public:
		using Listener = std::function<void(Event&)>;
		void RegisterListener(const std::string& eventType, Listener listener);
		void Dispatch(Event& e);
	private:
		std::unordered_map<std::string, std::vector<Listener>> listeners;
	};

}