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

include ("Demo/EventSystem/premake5.lua")
include("Demo/NuimDemo/premake5.lua")