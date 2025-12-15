#pragma once
#include <Windows.h>

namespace Nuim {

    class Application {
    public:
        explicit Application(HINSTANCE instance);
        int Run();

    private:
        HINSTANCE m_instance = nullptr;
    };

}
