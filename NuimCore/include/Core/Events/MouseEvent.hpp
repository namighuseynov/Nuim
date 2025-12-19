#pragma once
#include "Event.hpp"

namespace Nuim {

	class MousePressEvent :
		public Event {
	public:
		MousePressEvent(const U32& mouseButton, const U32& coordX, const U32&   coordY) : mouseButton(mouseButton), coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MousePress";
		}
		EventType GetType() const  override {
			return EventType::MousePressEvent; 
		}
		U32 GetMouseButton() {
			return mouseButton;
		}
		const U32& GetX() {
			return coordX;
		}
		const U32& GetY() {
			return coordY;
		}
	private:
		U32 mouseButton;
		U32 coordX;
		U32 coordY;
	};

	class MouseReleaseEvent :
		public Event {
	public:
		MouseReleaseEvent(const U32& mouseButton) : mouseButton(mouseButton) {};
	public:
		std::string GetName() const override {
			return "MouseRelease";
		}
		EventType GetType() const  override {
			return EventType::MouseReleaseEvent;
		}
		U32 GetMouseButton() {
			return mouseButton;
		}
	private:
		U32 mouseButton;

	};

	class MouseMoveEvent :
		public Event {
	public:
		MouseMoveEvent(const U32& coordX, const U32& coordY) : coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MouseMove";
		}
		EventType GetType() const  override {
			return EventType::MouseMoveEvent;
		}
		const U32& GetX() {
			return this->coordX;
		}
		const U32& GetY() {
			return this->coordY;
		}
	private:
		U32 coordX = 0;
		U32 coordY = 0;
	};

	class MouseMiddleButtonDown :
		public Event {
	public:
		MouseMiddleButtonDown(const U32& coordX, const U32& coordY) : coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MouseMiddleButtonDown";
		}
		EventType GetType() const  override {
			return EventType::MouseMiddleButtonDownEvent;
		}
		const U32& GetX() {
			return this->coordX;
		}
		const U32& GetY() {
			return this->coordY;
		}
	private:
		U32 coordX = 0;
		U32 coordY = 0;
	};

	class MouseMiddleButtonRelease :
		public Event {
	public:
		MouseMiddleButtonRelease(const U32& coordX, const U32& coordY) : coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MouseMiddleButtonRelease";
		}
		EventType GetType() const  override {
			return EventType::MouseMiddleButtonReleaseEvent;
		}
		const U32& GetX() {
			return this->coordX;
		}
		const U32& GetY() {
			return this->coordY;
		}
	private:
		U32 coordX = 0;
		U32 coordY = 0;
	};

	class MouseScrollEvent :
		public Event {

	};
}




