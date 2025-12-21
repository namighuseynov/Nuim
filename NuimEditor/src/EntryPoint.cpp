#include "EditorApp.hpp"
#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
#if defined(NUIM_DEBUG)
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
#endif

    try
    {
        Nuim::ApplicationSpecification spec;
        spec.Name = "NuimEditor";
        spec.Width = 1280;
        spec.Height = 720;
        spec.VSync = true;
        spec.EnableImGui = true;

        NuimEditor::EditorApp app(spec);
        app.Run();
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "NuimEditor - Fatal Error", MB_ICONERROR | MB_OK);
        return -1;
    }

    return 0;
}
