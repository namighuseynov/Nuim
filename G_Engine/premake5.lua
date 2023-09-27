project "G_Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "../bin"
    objdir "../bin-obj"
    links "opengl32.lib"

    pchheader "nmpch.h"
    pchsource "../G_Engine/src/nmpch.cpp"
--     -- Includes
    includedirs {
        "../G_Engine/src",
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
        "../G_Engine/src/*.cpp",
        "../G_Engine/src/*.h",
        "../G_Engine/src/*.c",

        "../G_Engine/src/G_Nuim/Core/*.cpp",
        "../G_Engine/src/G_Nuim/Core/*.h",
        "../G_Engine/src/G_Nuim/Core/*.c",

        "../G_Engine/src/G_Nuim/Events/*.cpp",
        "../G_Engine/src/G_Nuim/Events/*.h",
        "../G_Engine/src/G_Nuim/Events/*.c",

        "../G_Engine/src/G_Nuim/Imgui/*.cpp",
        "../G_Engine/src/G_Nuim/Imgui/*.h",
        "../G_Engine/src/G_Nuim/Imgui/*.c",

        "../G_Engine/src/G_Nuim/Math/*.cpp",
        "../G_Engine/src/G_Nuim/Math/*.h",
        "../G_Engine/src/G_Nuim/Math/*.c",

        "../G_Engine/src/G_Nuim/Renderer/*.cpp",
        "../G_Engine/src/G_Nuim/Renderer/*.h",
        "../G_Engine/src/G_Nuim/Renderer/*.c",

        "../G_Engine/src/G_Nuim/Scene/*.cpp",
        "../G_Engine/src/G_Nuim/Scene/*.h",
        "../G_Engine/src/G_Nuim/Scene/*.c",

    }







