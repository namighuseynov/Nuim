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
        "NuimCore/include/**.hpp",

        "NuimCore/src/**.h",
        "NuimCore/src/**.cpp"
    }

    includedirs {
        "NuimCore",
        "NuimCore/include",
        "NuimCore/include/Core/",
        "NuimCore/include/Core/Events",

        "NuimCore/src/Core", -- for NuimPCH.h
        
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



