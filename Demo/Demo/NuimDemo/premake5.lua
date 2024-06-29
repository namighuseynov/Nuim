project "NuimDemo"
    system "windows"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"
    includedirs {
        "%{wks.location}/Demo/NuimDemo",
        "%{wks.location}/Demo/NuimDemo/src",
        "%{wks.location}/Demo/EventSystem/src",

    }

    pchheader "NuimDemoPCH.h"
    pchsource "%{wks.location}/Demo/NuimDemo/src/NuimDemoPCH.cpp"
    links {
        "EventSystem"
    }
    files {
        "%{wks.location}/Demo/NuimDemo/src/*.cpp",
        "%{wks.location}/Demo/NuimDemo/src/*.hpp",
        "%{wks.location}/Demo/NuimDemo/src/*.h",
        "%{wks.location}/Demo/NuimDemo/src/*.c",
    }
    files {
        "%{wks.location}/Demo/NuimDemo/src/ImGui/*.cpp",
        "%{wks.location}/Demo/NuimDemo/src/ImGui/*.hpp",
        "%{wks.location}/Demo/NuimDemo/src/ImGui/*.h",
        "%{wks.location}/Demo/NuimDemo/src/ImGui/*.c",
    }
    filter "system:windows"
        links {"d3d11.lib"}

    filter "kind:WindowedApp"
        defines {"WINDOWED"}
    -- filter "kind:ConsoleApp"
    --     defines {"NUIM_SUBSYSTEM_CONSOLE"}
        