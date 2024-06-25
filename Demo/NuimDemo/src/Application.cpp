#include "NuimDemoPCH.h"
#include "Application.hpp"


namespace NuimDemo {
	Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) {
		this->hInstance = hInstance;
		this->nCmdShow = nCmdShow;
		if (CreateAppWindow()) {
			this->d3d_app = new D3DApp(this->baseWindow);
			d3d_app->Begin();
		}
		else {
			MessageBox(nullptr, L"Error creating renderer", L"Error", 0);
		}
	};
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
		return 1;
	};
	void Application::Run() {
		MSG msg;
		while (GetMessage(&msg, 0, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);

			d3d_app->Render();
		}
		d3d_app->ShutDown();
		delete d3d_app;
		delete baseWindow;
	};
}