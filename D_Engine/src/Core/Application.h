#pragma once
#include <Windows.h>

HINSTANCE wInstance;
static TCHAR szWindowClass[] = L"NuimEngine";

static TCHAR szTitle[] = L"Nuim";

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.lpfnWndProc = nullptr;
	wc.lpszClassName = L"Nuim";
	wc.hCursor = NULL;
	wc.hInstance = hInstance;

	if (RegisterClass(&wc)) {
		MessageBox(NULL, L"Call to RegisterClassEx failed!",
			L"Windows Desktop Guided Tour",
			NULL);
		return 1;
	}

	wInstance = hInstance;
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
		wInstance,
		NULL
	);

	if (!hwnd) {
		MessageBox(NULL, L"Call to create", L"sdkf", NULL);
		return 1;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}