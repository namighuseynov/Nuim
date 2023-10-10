#pragma once
#include <Windows.h>
#include <tchar.h>

namespace Nuim {
	void ShowMessage(USTRING msg);
	

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
		USTRING szWindowClass = u"NuimEngine";
		USTRING szTitle = u"Nuim";
		HINSTANCE hInstance{ NULL };
		WNDCLASSEX wcex{ NULL };
	};
}