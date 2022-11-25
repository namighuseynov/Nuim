workspace "Nuim"
    system "windows"
    systemversion "latest"
    configurations "Release"
    architecture "x64"
    startproject "Nuim"
    defines "SYSTEM_WINDOWS"

project "Nuim"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    location "Nuim"
    targetdir "bin"
    objdir "bin-obj"
    links "Engine"
    includedirs "Engine/src/"

    files {
        "Nuim/src/*.cpp",
        "Nuim/src/*.h",
        "Nuim/src/*.c",
    }

project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    location "Engine"
    targetdir "bin"
    objdir "bin-obj"
    links "opengl32.lib"

    pchheader "nmpch.h"
    pchsource "Engine/src/nmpch.cpp"
    -- Includes
    includedirs {
        "Engine/src",
        "Linking/include",
        "vendor"
    }


    libdirs {
        "Linking/libs/GLFW",
        "Linking/libs/GLAD",
    }
    links {
        "GLFW/glfw3.lib",
        "GLAD/glad.lib",
    }

    files {
        "Engine/src/*.cpp",
        "Engine/src/*.h",
        "Engine/src/*.c",

        "Engine/src/Nuim/Core/*.cpp",
        "Engine/src/Nuim/Core/*.h",
        "Engine/src/Nuim/Core/*.c",

        "Engine/src/Nuim/Math/*.cpp",
        "Engine/src/Nuim/Math/*.h",
        "Engine/src/Nuim/Math/*.c",

        "Engine/src/Nuim/Renderer/*.cpp",
        "Engine/src/Nuim/Renderer/*.h",
        "Engine/src/Nuim/Renderer/*.c",

        "Engine/src/Nuim/ImGui/*.cpp",
        "Engine/src/Nuim/ImGui/*.h",
        "Engine/src/Nuim/ImGui/*.c",

        "Engine/src/Nuim/Scene/*.cpp",
        "Engine/src/Nuim/Scene/*.h",
        "Engine/src/Nuim/Scene/*.c",

        "Engine/src/Nuim/EventSystem/*.cpp",
        "Engine/src/Nuim/EventSystem/*.h",
        "Engine/src/Nuim/EventSystem/*.c",

    }




