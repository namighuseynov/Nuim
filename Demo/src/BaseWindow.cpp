#include "NuimDemoPCH.h"
#include "BaseWindow.h"

namespace NuimDemo {

	LRESULT CALLBACK WndProcess(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam
	) {
		switch (msg) {
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}

	bool BaseWindow::Show() {
		ShowWindow(this->hWnd, this->nCmdShow);
		return 1;
	}


	BaseWindow::BaseWindow(HINSTANCE hInstance, LPWSTR className, UINT16 width, UINT16 height, UINT16 posX, UINT16 posY, int nCmdShow) {
		this->hInstance		= hInstance;
		this->className		= className;
		this->Width			= width;
		this->Height		= height;
		this->PosX			= posX;
		this->PosY			= posY;
		this->nCmdShow		= nCmdShow;
		wc = {};
		wc.hInstance = this->hInstance;
		wc.lpszClassName = this->className;
		wc.lpfnWndProc = WndProcess;
		RegisterClass(&wc);

		this->hWnd = CreateWindowEx(0, className, className, WS_OVERLAPPEDWINDOW, posX, posY, width, height, nullptr, nullptr, hInstance, nullptr);
		if (!this->hWnd) {
			MessageBox(nullptr, L"Failed to register window class", L"Error", 0);
		}
	}
}