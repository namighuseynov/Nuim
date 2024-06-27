#pragma once
#include <string>

namespace EventSystem {
	class Event
	{
	public:
		virtual std::string GetName() const = 0;
	};
}