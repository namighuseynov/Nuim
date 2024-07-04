#pragma once
#include "EventSystem.hpp"

namespace NuimDemo {
	class Window
	{
	using EventCallback = std::function<void(EventSystem::Event& e)>;
	public:
		Window(HINSTANCE hInstance);
	public:
		static LRESULT CALLBACK WndProcess(
			HWND hwnd,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam
		);
		void SetEventCallback(EventCallback callbackFn);
		HWND GetHWND() { return this->hwnd; }
	public:
		UINT Width;
		UINT Height;

	private:
		HWND hwnd;
		EventCallback eventCallbackFn;
	};
}