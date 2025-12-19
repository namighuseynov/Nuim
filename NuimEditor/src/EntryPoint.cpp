#include "EditorApplication.hpp"

#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    try
    {
        NuimEditor::EditorApplicationSpecification spec;
        spec.Name = "NuimEditor";
        spec.Width = 1280;
        spec.Height = 720;
        spec.VSync = true;

        NuimEditor::EditorApplication app(spec);
        app.Run();
    }
    catch (const std::exception& e)
    {
        MessageBoxA(nullptr, e.what(), "NuimEditor - Fatal Error", MB_ICONERROR | MB_OK);
        return -1;
    }

    return 0;
}