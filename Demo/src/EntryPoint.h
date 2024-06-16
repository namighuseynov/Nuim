#pragma once
#include <Windows.h>

LRESULT CALLBACK WinProc(
	HWND hwnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (Msg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, Msg, wParam, lParam);
	}
		
	
}


int WINAPI WinMain(
	HINSTANCE	hInstance,
	HINSTANCE	hPrevInstance,
	LPSTR		nCmdLine,
	int			nCmdShow
) {
	WNDCLASS wc = {};
	const wchar_t* class_name = L"NuimDemo";
	wc.lpszClassName = class_name;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WinProc;
	RegisterClass(&wc);
	HWND hwnd = CreateWindowEx(0, class_name, class_name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 600, 600, NULL, NULL, hInstance, NULL);
	
	if (hwnd == NULL) {
		MessageBox(NULL, L"Error creating window", L"Error", 0);
	}
	ShowWindow(hwnd, nCmdShow);
	MSG Msg;
	while (GetMessage(&Msg, 0, 0, 0) > 0) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return 0;
}