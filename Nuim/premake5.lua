project "Nuim"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "../bin"
    objdir "../bin-obj"
    links "Engine"
    includedirs "../Engine/src/"

    files {
        "../Nuim/src/*.cpp",
        "../Nuim/src/*.h",
        "../Nuim/src/*.c",
    }