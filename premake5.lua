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
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "NUIM_RELEASE" }
        runtime "Release"
        optimize "on"

project "NuimWorld"
    location "NuimWorld"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "NuimWorld/include/**.h",
        "NuimWorld/include/**.hpp",
        "NuimWorld/src/**.cpp"
    }

    includedirs {
        "NuimWorld/include",
        "NuimCore/include"
    }

    links { "NuimCore" }


project "NuimRender"
    location "NuimRender"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "NuimRender/include/**.hpp",
        "NuimRender/src/**.cpp",
        "NuimRender/**.cpp"
    }

    includedirs {
        "NuimRender/include",
        "NuimCore/include",
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    links { "NuimCore" } --"NuimWorld", "NuimAssets"-- }

project "NuimRenderDX11"
    location "NuimRenderDX11"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "NuimRenderDX11/include/**.h",
        "NuimRenderDX11/include/**.hpp",
        "NuimRenderDX11/src/**.h",
        "NuimRenderDX11/src/**.cpp"
    }

    includedirs {
        "NuimRenderDX11/include",
        "NuimRender/include",
        "NuimCore/include",
        "ImGui",
    }

    links {
        "NuimRender", "NuimCore", "ImGui",
        "d3d11", "dxgi", "d3dcompiler",
        "windowscodecs", -- WIC
        "ole32"          -- CoInitialize/COM
    }

    filter "system:windows"
        systemversion "latest"
        defines { "NUIM_RENDER_DX11" }

        filter "configurations:Debug"
    runtime "Debug"
    symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    pchheader "NuimPCH.h"
    pchsource "NuimRenderDX11/src/NuimPCH.cpp"

project "ImGui"
    location "ImGui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "ImGui/imgui.cpp",
        "ImGui/imgui_demo.cpp",
        "ImGui/imgui_draw.cpp",
        "ImGui/imgui_tables.cpp",
        "ImGui/imgui_widgets.cpp",
        "ImGui/imgui_impl_win32.cpp",
        "ImGui/imgui_impl_dx11.cpp",
        "ImGui/imgui.h",
        "ImGui/imgui_impl_win32.h",
        "ImGui/imgui_impl_dx11.h"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    includedirs { "%{IncludeDir.ImGui}" }
    defines { "IMGUI_IMPL_WIN32_DISABLE_GAMEPAD" }

project "NuimEditor"
    location "NuimEditor"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "NuimEditor/src/**.cpp",
        "NuimEditor/src/**.h",
        "NuimEditor/src/**.hpp"
    }

    includedirs {
        "NuimEditor/src",
        "NuimCore/include",
        "NuimWorld/include",
        "NuimAssets/include",
        "NuimRender/include",
        "NuimRenderDX11/include",
        "ImGui",
        "%{IncludeDir.ImGui}"
    }

    links {
        "NuimRenderDX11", "NuimRender", "NuimCore",
        "ImGui"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"




