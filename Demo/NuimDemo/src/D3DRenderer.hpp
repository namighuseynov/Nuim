#pragma once
#include "BaseWindow.h"
#include "D3DApp.hpp"

namespace NuimDemo {
	class D3DRenderer
	{
	public:
		D3DRenderer(BaseWindow* baseWindow);
	public:
		void Begin();
		void ShutDown();
		void Render();
	private:
		BaseWindow* window;
		D3DApp* d3dApp;
	};
}

