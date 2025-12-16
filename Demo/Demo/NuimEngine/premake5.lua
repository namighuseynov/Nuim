project "NuimEngine"
    system "windows"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"
    includedirs {
        "%{wks.location}/Demo/NuimEngine/src",
        "%{wks.location}/Demo/NuimEngine/src/NuimEngine"  
    }

    pchheader "pch.h"
    pchsource "%{wks.location}/Demo/NuimEngine/src/NuimEngine/pch.cpp"
    
    files {
        "%{wks.location}/Demo/NuimEngine/src/NuimEngine/*.cpp",
        "%{wks.location}/Demo/NuimEngine/src/NuimEngine/*.hpp",
        "%{wks.location}/Demo/NuimEngine/src/NuimEngine/*.h",
        "%{wks.location}/Demo/NuimEngine/src/NuimEngine/*.c",
    }
    -- filter "kind:ConsoleApp"
    --     defines {"NUIM_SUBSYSTEM_CONSOLE"}
        