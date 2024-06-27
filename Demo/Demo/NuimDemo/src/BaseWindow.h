#pragma once
#include <Windows.h>

namespace NuimDemo {
	class BaseWindow
	{
	public:
		BaseWindow(HINSTANCE hInstance, LPWSTR className, UINT16 width, UINT16 height, UINT16 posX, UINT16 posY, int nCmdShow);
	public:
		bool Show();
		HWND GetHWND();
	public:
		UINT16 Width;
		UINT16 Height;
		UINT16 PosX;
		UINT16 PosY;
	private:
		WNDCLASS wc;
		HWND hWnd;
		HINSTANCE hInstance;
		LPWSTR className;
		int nCmdShow;
	};
}