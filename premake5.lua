workspace "Nuim"
    system "windows"
    systemversion "latest"
    configurations { "Debug", "Release" }
    architecture "x64"
    startproject "Nuim"
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

include "Nuim/premake5.lua"
include "D_Engine/premake5.lua"

-- include "V_Engine/premake5.lua"
-- include "G_Engine/premake5.lua"
