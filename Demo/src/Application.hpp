#pragma once
#include "BaseWindow.h"

namespace NuimDemo {
	class Application
	{
	public:
		Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow);
		~Application();
	public:
		void Run();
		void Close();
	private:
		bool CreateAppWindow();
	private:
		HINSTANCE hInstance;
		int nCmdShow;
		BaseWindow* baseWindow = nullptr;
	};
}


