#pragma once
#include <Windows.h>

namespace NuimDemo {
	class Window
	{
	public:
		Window(HINSTANCE hInstance, LPWSTR className, UINT16 width, UINT16 height, UINT16 posX, UINT16 posY);
	public:
		UINT16 Width;
		UINT16 Height;
		UINT16 PosX;
		UINT16 PosY;
	private:
		LRESULT CALLBACK WndProc(
			HWND hwnd,
			UINT msg,
			WPARAM wParam,
			LPARAM lParam
		) {
			switch (msg) {
			case WM_CLOSE:
				DestroyWindow(this->hWnd);
				break;
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(this->hWnd, msg, wParam, lParam);
			}
		}
	private:
		WNDCLASSEX wcex;
		HWND hWnd;
		HINSTANCE hInstance;
		LPWSTR className;
	};
}