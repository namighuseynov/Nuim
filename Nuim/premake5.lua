project "Nuim"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "../bin"
    objdir "../bin-obj"
    links {
        -- "Engine",
        "Vulkan"
    }
    includedirs "../Engine/src/"
    includedirs "../Vulkan/src/"

    files {
        "../Nuim/src/*.cpp",
        "../Nuim/src/*.h",
        "../Nuim/src/*.c",
    }