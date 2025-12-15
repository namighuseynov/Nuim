#include "NuimDemoPCH.h"
#include "Application.hpp"

#include "Engine.hpp"
#include "TestScene.hpp"

namespace Nuim {

    Application::Application(HINSTANCE instance)
        : m_instance(instance) {
    }

    int Application::Run()
    {
        EngineConfig cfg{ 1280, 800, true };

        Engine engine;
        if (!engine.Init(cfg))
            return -1;

        engine.LoadScene(std::make_unique<TestScene>());
        engine.Run();

        return 0;
    }

} 
