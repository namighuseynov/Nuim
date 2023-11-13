#include "WindowsWindow.hpp"

namespace Nuim {
	WindowsWindow::WindowsWindow(USTRING title, U32 width, U32 height, HINSTANCE hInstance, I32 nCmdShow) : instance(NULL), title(u"") {
		this->Title = title;
		this->Width = width;
		this->Height = height;

		WNDCLASSEX wcex{};
		MSG Msg;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = 0;
		wcex.lpfnWndProc = WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = (LPCWSTR)this->Title;
		wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL, L"Window Registration Failed!", L"Error!",
				MB_ICONEXCLAMATION | MB_OK);
			return;
		}

		this->instance = CreateWindowEx(
			0,
			(LPCWSTR)this->Title,
			(LPCWSTR)this->Title,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			this->Width,
			this->Height,
			NULL,
			NULL,
			hInstance,
			NULL
		);

		if (this->instance == NULL)
		{
			std::cout << "error" << std::endl;
		}
		else {
			ShowWindow(this->instance, nCmdShow);
			UpdateWindow(this->instance);

			while (GetMessage(&Msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&Msg);
				DispatchMessage(&Msg);
			}
			return;
		}

		

	}

	WindowsWindow::~WindowsWindow() {
		
	}

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return 0;
	}
}