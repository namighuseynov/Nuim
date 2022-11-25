#pragma once
#include "Event.h"

#include <sstream>

namespace Nuim {
	class WindowResizeEvent : public Event { 
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: Width(width), Height(height) {}
		unsigned int GetWidth() const { return Width; }
		unsigned int GetHeight() const { return Height; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "WindowResizeEvent: " << Width << ", " << Height;
			return ss.str();
		}
	private:
		unsigned int Width, Height;
	};

	class WindowCloseEvent : public Event {

	};

	class ApplicationTickEvent : public Event {

	};

	class ApplicationUpdateEvent : public Event {

	};

	class ApplicationRenderEvent : public Event {

	};
}
