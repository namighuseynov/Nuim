project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "../bin"
    objdir "../bin-obj"
    links "opengl32.lib"

    pchheader "nmpch.h"
    pchsource "../Engine/src/nmpch.cpp"
--     -- Includes
    includedirs {
        "../Engine/src",
        "../Linking/include",
        "../vendor"
    }


    libdirs {
        "../Linking/libs/GLFW",
        "../Linking/libs/GLAD",
    }
    links {
        "../GLFW/glfw3.lib",
        "../GLAD/glad.lib",
    }

    files {
        "../Engine/src/*.cpp",
        "../Engine/src/*.h",
        "../Engine/src/*.c",

        "../Engine/src/Nuim/Core/*.cpp",
        "../Engine/src/Nuim/Core/*.h",
        "../Engine/src/Nuim/Core/*.c",

        "../Engine/src/Nuim/Events/*.cpp",
        "../Engine/src/Nuim/Events/*.h",
        "../Engine/src/Nuim/Events/*.c",

        "../Engine/src/Nuim/ImGui/*.cpp",
        "../Engine/src/Nuim/ImGui/*.h",
        "../Engine/src/Nuim/ImGui/*.c",

        "../Engine/src/Nuim/Math/*.cpp",
        "../Engine/src/Nuim/Math/*.h",
        "../Engine/src/Nuim/Math/*.c",

        "../Engine/src/Nuim/Renderer/*.cpp",
        "../Engine/src/Nuim/Renderer/*.h",
        "../Engine/src/Nuim/Renderer/*.c",

        "../Engine/src/Nuim/Scene/*.cpp",
        "../Engine/src/Nuim/Scene/*.h",
        "../Engine/src/Nuim/Scene/*.c",

    }







