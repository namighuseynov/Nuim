project "EventSystem"
    system "windows"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"
    includedirs {
        "%{wks.location}/Demo/EventSystem/src",
        "%{wks.location}/Demo/EventSystem/src/EventSystem"  
    }

    pchheader "pch.h"
    pchsource "%{wks.location}/Demo/EventSystem/src/EventSystem/pch.cpp"
    
    files {
        "%{wks.location}/Demo/EventSystem/src/EventSystem/*.cpp",
        "%{wks.location}/Demo/EventSystem/src/EventSystem/*.hpp",
        "%{wks.location}/Demo/EventSystem/src/EventSystem/*.h",
        "%{wks.location}/Demo/EventSystem/src/EventSystem/*.c",
    }
    -- filter "kind:ConsoleApp"
    --     defines {"NUIM_SUBSYSTEM_CONSOLE"}
        