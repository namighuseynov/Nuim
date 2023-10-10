#include "Core.h"
#include "Application.h"

namespace Nuim {
	Application::Application(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow) {
		this->wcex.cbSize = sizeof(WNDCLASSEX);
		this->wcex.style = CS_HREDRAW | CS_VREDRAW;
		this->wcex.lpfnWndProc = WndProc;
		this->wcex.cbClsExtra = 0;
		this->wcex.cbWndExtra = 0;
		this->wcex.hInstance = hInstance;
		this->wcex.hIcon = LoadIcon(this->wcex.hInstance, IDI_APPLICATION);
		this->wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		this->wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		this->wcex.lpszMenuName = NULL;
		this->wcex.lpszClassName = szWindowClass;
		this->wcex.hIconSm = LoadIcon(this->wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				_T("Call to RegisterClassEx failed!"),
				_T("Windows Desktop Guided Tour"),
				NULL);
			return;
		}
		std::wstring text = std::to_wstring(sizeof("nnnəə"));
		MessageBox(NULL, text.c_str(), text.c_str(), NULL);
		this->hInstance = hInstance;
		HWND hwnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,
			szWindowClass,
			szTitle,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			800,
			600,
			NULL,
			NULL,
			this->hInstance,
			NULL
		);

		if (!hwnd) {
			MessageBox(NULL, L"Call to create", L"sdkf", NULL);
			return;
		}

		ShowWindow(hwnd, nCmdShow);
		UpdateWindow(hwnd);
	}

	Application::~Application() {

	}

	void Application::Run() {
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}


	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		PAINTSTRUCT ps;
		HDC hdc;
		TCHAR greeting[] = L"Hello from Nuim Engine";

		switch (message)
		{
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			// Here your application is laid out.
			// For this introduction, we just print out "Hello, Windows desktop!"
			// in the top left corner.
			TextOut(hdc,
				5, 5,
				greeting, (int)_tcslen(greeting));
			// End application-specific layout section.

			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}
}
