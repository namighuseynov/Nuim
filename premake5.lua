workspace "Nuim"
    architecture "x64"
    startproject "NuimEditor"

    configurations { "Debug", "Release" }
    flags { "MultiProcessorCompile" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["ImGui"] = "extern/imgui"

project "NuimCore"
    location "NuimCore"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "NuimPCH.h"
    pchsource "NuimCore/src/Core/NuimPCH.cpp"

    files {
        "NuimCore/include/**.h",
        "NuimCore/src/**.h",
        "NuimCore/src/**.hpp",
        "NuimCore/src/**.cpp"
    }

    includedirs {
        "NuimCore/include",
        "NuimCore/src/Core" -- for NuimPCH.h
    }

    filter "system:windows"
        systemversion "latest"
        defines { "NUIM_PLATFORM_WINDOWS" }

    filter "configurations:Debug"
        defines { "NUIM_DEBUG" }
        symbols "on"

    filter "configurations:Release"
        defines { "NUIM_RELEASE" }
        optimize "on"



