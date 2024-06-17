#include "NuimDemoPCH.h"
#include "Window.h"

namespace NuimDemo {
	Window::Window(HINSTANCE hInstance, LPWSTR className, UINT16 width, UINT16 height, UINT16 posX, UINT16 posY) {
		this->hInstance = hInstance;
		this->className = className;
		this->Width = width;
		this->Height = height;
		this->PosX = posX;
		this->PosY = posY;
		wcex = {};
		wcex.hInstance = this->hInstance;
		wcex.lpszClassName = this->className;
		//wcex.lpfnWndProc = this->WndProc;
		this->hWnd = CreateWindowEx(0, className, className, WS_OVERLAPPEDWINDOW, posX, posY, width, height, nullptr, nullptr, hInstance, nullptr);

	}
}