workspace "NuimDemo"
    system "windows"
    systemversion "latest"
    configurations { "Debug", "Release" }
    architecture "x64"
    startproject "NuimDemo"
    characterset "Unicode"
    defines {"_CRT_SECURE_NO_WARNINGS", "NUIM_CHARACTERSET_UNICODE" }
    flags
	{
		"MultiProcessorCompile"
	}
    
    filter "configurations:Debug"
        defines { "DEBUG", "NUIM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG", "NUIM_NDEBUG" }
        optimize "On"

project "NuimDemo"
    -- kind "ConsoleApp"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"
    includedirs {
        "%{wks.location}",
        "%{wks.location}/src"
    }

    pchheader "NuimDemoPCH.h"
    pchsource "%{wks.location}/src/NuimDemoPCH.cpp"
    
    files {
        "%{wks.location}/src/*.cpp",
        "%{wks.location}/src/*.h",
        "%{wks.location}/src/*.c",
    }

    filter "kind:WindowedApp"
        defines {"WINDOWED"}
    -- filter "kind:ConsoleApp"
    --     defines {"NUIM_SUBSYSTEM_CONSOLE"}
        