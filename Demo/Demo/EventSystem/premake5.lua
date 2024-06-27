project "EventSystem"
    system "windows"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"
    includedirs {
        "%{wks.location}/Demo/EventSystem",
        "%{wks.location}/Demo/EventSystem/src"
    }

    pchheader "pch.h"
    pchsource "%{wks.location}/Demo/EventSystem/src/pch.cpp"
    
    files {
        "%{wks.location}/Demo/EventSystem/src/*.cpp",
        "%{wks.location}/Demo/EventSystem/src/*.hpp",
        "%{wks.location}/Demo/EventSystem/src/*.h",
        "%{wks.location}/Demo/EventSystem/src/*.c",
    }
    -- files {
    --     "%{wks.location}/src/ImGui/*.cpp",
    --     "%{wks.location}/src/ImGui/*.hpp",
    --     "%{wks.location}/src/ImGui/*.h",
    --     "%{wks.location}/src/ImGui/*.c",
    -- }
    -- filter "kind:ConsoleApp"
    --     defines {"NUIM_SUBSYSTEM_CONSOLE"}
        