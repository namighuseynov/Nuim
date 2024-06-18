#include "NuimDemoPCH.h"
#include "Application.hpp"


namespace NuimDemo {
	Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
		this->hInstance = hInstance;
		this->nCmdShow = nCmdShow;
		if (CreateAppWindow()) {
			this->renderer = new Renderer(this->baseWindow);
		}
		else {
			MessageBox(nullptr, L"Error creating renderer", L"Error", 0);
		}
		
	};
	Application::~Application() {
		if (this->baseWindow != nullptr) {
			delete baseWindow;
		}
		if (this->renderer != nullptr) {
			delete renderer;
		}
	}
	bool Application::CreateAppWindow() {
		this->baseWindow = new BaseWindow(
			this->hInstance,
			L"NuimDemo",
			600,
			600,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			this->nCmdShow
		);
		this->baseWindow->Show();
		return 1;
	};
	void Application::Run() {
		MSG msg;
		while (GetMessage(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	};
	void Application::Close()
	{
	}
}