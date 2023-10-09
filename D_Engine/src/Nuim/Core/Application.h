#pragma once
#include <Windows.h>
#include <tchar.h>

namespace Nuim {

	static TCHAR szWindowClass[] = L"NuimEngine";

	static TCHAR szTitle[] = L"Nuim";

	LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


	class Application {
	public:
		Application(HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR lpCmdLine,
			int nCmdShow);
		~Application();
	public:
		void Run();

	private:
		HINSTANCE hInstance{ NULL };
		WNDCLASSEX wcex{ NULL };
	};
}