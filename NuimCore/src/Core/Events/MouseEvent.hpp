#pragma once
#include "Event.hpp"

namespace EventSystem {
	
	class MousePressEvent :
		public Event {
	public:
		MousePressEvent(const int& mouseButton, const int& coordX, const int&   coordY) : mouseButton(mouseButton), coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MousePress";
		}
		EventType GetType() const  override {
			return EventType::MousePressEvent; 
		}
		int GetMouseButton() {
			return mouseButton;
		}
		const int& GetX() {
			return coordX;
		}
		const int& GetY() {
			return coordY;
		}
	private:
		int mouseButton;
		int coordX;
		int coordY;
	};

	class MouseReleaseEvent :
		public Event {
	public:
		MouseReleaseEvent(const int& mouseButton) : mouseButton(mouseButton) {};
	public:
		std::string GetName() const override {
			return "MouseRelease";
		}
		EventType GetType() const  override {
			return EventType::MouseReleaseEvent;
		}
		int GetMouseButton() {
			return mouseButton;
		}
	private:
		int mouseButton;

	};

	class MouseMoveEvent :
		public Event {
	public:
		MouseMoveEvent(const UINT& coordX, const UINT& coordY) : coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MouseMove";
		}
		EventType GetType() const  override {
			return EventType::MouseMoveEvent;
		}
		const UINT& GetX() {
			return this->coordX;
		}
		const UINT& GetY() {
			return this->coordY;
		}
	private:
		UINT coordX = 0;
		UINT coordY = 0;
	};

	class MouseMiddleButtonDown :
		public Event {
	public:
		MouseMiddleButtonDown(const UINT& coordX, const UINT& coordY) : coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MouseMiddleButtonDown";
		}
		EventType GetType() const  override {
			return EventType::MouseMiddleButtonDownEvent;
		}
		const UINT& GetX() {
			return this->coordX;
		}
		const UINT& GetY() {
			return this->coordY;
		}
	private:
		UINT coordX = 0;
		UINT coordY = 0;
	};

	class MouseMiddleButtonRelease :
		public Event {
	public:
		MouseMiddleButtonRelease(const UINT& coordX, const UINT& coordY) : coordX(coordX), coordY(coordY) {};
	public:
		std::string GetName() const override {
			return "MouseMiddleButtonRelease";
		}
		EventType GetType() const  override {
			return EventType::MouseMiddleButtonReleaseEvent;
		}
		const UINT& GetX() {
			return this->coordX;
		}
		const UINT& GetY() {
			return this->coordY;
		}
	private:
		UINT coordX = 0;
		UINT coordY = 0;
	};

	class MouseScrollEvent :
		public Event {

	};
}




