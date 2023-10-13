#include "Core.h"
#include "Application.h"

namespace Nuim {
	Application::Application(HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow) {
		this->wcex.cbSize = sizeof(WNDCLASSEX);
		this->wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		this->wcex.lpfnWndProc = WndProc;
		this->wcex.cbClsExtra = 0;
		this->wcex.cbWndExtra = 0;
		this->wcex.hInstance = hInstance;
		this->wcex.hIcon = LoadIcon(this->wcex.hInstance, IDI_APPLICATION);
		this->wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		this->wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		this->wcex.lpszMenuName = NULL;
		this->wcex.lpszClassName = (LPWSTR)szWindowClass;
		this->wcex.hIconSm = LoadIcon(this->wcex.hInstance, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			MessageBox(NULL,
				UNICODE_TEXT("Call to RegisterClassEx failed!"),
				UNICODE_TEXT("Windows Desktop Guided Tour"),
				NULL); 
			return;
		}
		this->hInstance = hInstance;
		HWND hwnd = CreateWindowEx(
			WS_EX_OVERLAPPEDWINDOW,
			(LPWSTR)(szWindowClass),
			(LPWSTR)szTitle,
			WS_TILEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			800,
			600,
			NULL,
			NULL,
			this->hInstance,
			NULL
		);

		if (!hwnd) {
			MessageBox(NULL, UNICODE_TEXT("Call to create"), UNICODE_TEXT("Message"), NULL);
			return;
		}
		ShowWindow(hwnd, SW_SHOWMAXIMIZED);
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
		//PAINTSTRUCT ps;
		//HDC hdc;
		//TCHAR greeting[] = L"Hello from Nuim Engine";
		//UINT16 cmnd{ 0 };
		switch (message)
		{
		//case WM_CHAR: {
		//	CHAR16 chr = static_cast<CHAR16>(wParam);
		//	CHAR16 buffer[2] = { chr, u'\0' };

		//	ShowMessage(buffer);


		//	int keyCode = static_cast<int>(lParam);

		//	break;
		//}
		//	
		//case WM_APPCOMMAND:
		//	cmnd = GET_APPCOMMAND_LPARAM(lParam);
		//	ShowMessage(u"Media");

		//case WM_ACTIVATE:
		//	if (wParam == WA_CLICKACTIVE) {
		//		ShowMessage(u"Activate");
		//	}
		//	
		//	
		//case WM_KEYDOWN:
		//	/*if (wParam == KEY) {
		//		ShowMessage(u"Pressed");
		//	}*/
		//	break;

		//case WM_KEYUP:
		//	
		//	break;
		//case WM_PAINT:
		//	hdc = BeginPaint(hWnd, &ps);

		//	// Here your application is laid out.
		//	// For this introduction, we just print out "Hello, Windows desktop!"
		//	// in the top left corner.
		//	TextOut(hdc,
		//		5, 5,
		//		greeting, (int)_tcslen(greeting));
		//	// End application-specific layout section.

		//	EndPaint(hWnd, &ps);
		//	break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
		}

		return 0;
	}

	void ShowMessage(USTRING msg) {
		MessageBox(NULL, (LPWSTR)msg, UNICODE_TEXT("Message"), NULL);
	}

}
