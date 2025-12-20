#pragma once

#include <Windows.h>
#include <d3d11.h>

namespace NuimEditor {

    class EditorImGuiLayer
    {
    public:
        void Init(HWND hwnd, ID3D11Device* device, ID3D11DeviceContext* context);
        void Shutdown();

        void BeginFrame();
        void EndFrame();

        bool HandleWin32Message(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    private:
        bool m_initialized = false;
    };

}
