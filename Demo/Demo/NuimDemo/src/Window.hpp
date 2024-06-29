#pragma once

namespace NuimDemo {
	class Window
	{
	public:
		static Window* Create(HINSTANCE hInstance);
		void OnUpdate();
	public:
		UINT Width;
		UINT Height;

	private:
		HWND hwnd;

	};
}