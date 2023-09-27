workspace "Nuim"
    system "windows"
    systemversion "latest"
    configurations { "Debug", "Release" }
    architecture "x64"
    startproject "Nuim"
    defines "SYSTEM_WINDOWS"

include "Nuim/premake5.lua"
include "V_Engine/premake5.lua"
include "G_Engine/premake5.lua"
