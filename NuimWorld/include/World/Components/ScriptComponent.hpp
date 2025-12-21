#pragma once
#include <memory>
#include <vector>
#include "World/Scripting/ScriptBase.hpp"

namespace Nuim::World {

    struct ScriptComponent
    {
        std::vector<std::unique_ptr<ScriptBase>> scripts;
    };

}
