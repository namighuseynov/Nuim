project "NuimDemo"
    system "windows"
    kind "SharedLibs"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/../bin"
    objdir "%{wks.location}/../bin-obj"
    -- includedirs {
    --     "%{wks.location}",
    --     "%{wks.location}/src"
    -- }

    -- pchheader "NuimDemoPCH.h"
    -- pchsource "%{wks.location}/src/NuimDemoPCH.cpp"
    
    -- files {
    --     "%{wks.location}/src/*.cpp",
    --     "%{wks.location}/src/*.hpp",
    --     "%{wks.location}/src/*.h",
    --     "%{wks.location}/src/*.c",
    -- }
    -- files {
    --     "%{wks.location}/src/ImGui/*.cpp",
    --     "%{wks.location}/src/ImGui/*.hpp",
    --     "%{wks.location}/src/ImGui/*.h",
    --     "%{wks.location}/src/ImGui/*.c",
    -- }
    -- filter "kind:ConsoleApp"
    --     defines {"NUIM_SUBSYSTEM_CONSOLE"}
        