workspace "Nuim"
    system "windows"
    systemversion "latest"
    configurations "Release"
    architecture "x64"
    location "Nuim"
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
    pchsource "Engine/src/Nuim/nmpch.cpp"
    -- Includes
    includedirs {
        "Engine/src/Nuim",
        "Linking/include"
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
        "Engine/src/Nuim/*.cpp",
        "Engine/src/Nuim/*.h",
        "Engine/src/Nuim*.c",
        "Engine/src/Nuim/Components/*.cpp",
        "Engine/src/Nuim/Components/*.h",
        "Engine/src/Nuim/Components/*.c",
        "Engine/src/Nuim/InputSystem/*.cpp",
        "Engine/src/Nuim/InputSystem/*.h",
        "Engine/src/Nuim/InputSystem/*.c"
    }




