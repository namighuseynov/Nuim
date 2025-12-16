workspace "Nuim"
    architecture "x86_64"
    configurations { "Debug", "Release" }
    startproject "Sandbox"
    location "build"

outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

newoption {
    trigger     = "gfx",
    value       = "API",
    description = "Graphics backend: dx11 (default), dx12, vulkan"
}

local GFX = _OPTIONS["gfx"] or "dx11"

function NuimCommon()
    language "C++"
    cppdialect "C++20"
    staticruntime "off" -- /MD, /MDd
    warnings "Extra"

    systemversion "latest"
    characterset "Unicode"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir    ("bin-int/" .. outputdir .. "/%{prj.name}")

    filter "configurations:Debug"
        defines { "NUIM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NUIM_RELEASE", "NDEBUG" }
        optimize "On"

    filter "system:windows"
        defines { "NUIM_PLATFORM_WINDOWS" }

    filter {}
end

function LinkForWinApp()
    filter "system:windows"
        links { "user32", "gdi32", "shell32", "ole32", "uuid" }
    filter {}

    if GFX == "dx11" then
        defines { "NUIM_GFX_DX11" }
        filter "system:windows"
            links { "d3d11", "dxgi", "d3dcompiler" }
        filter {}

    elseif GFX == "dx12" then
        defines { "NUIM_GFX_DX12" }
        filter "system:windows"
            links { "d3d12", "dxgi", "d3dcompiler", "dxguid" }
        filter {}

    elseif GFX == "vulkan" then
        defines { "NUIM_GFX_VULKAN" }
        filter "system:windows"
            links { "vulkan-1" }
        filter {}

        local vulkanSDK = os.getenv("VULKAN_SDK")
        if vulkanSDK then
            includedirs { vulkanSDK .. "/Include" }
            libdirs     { vulkanSDK .. "/Lib" }
        else
            print("WARNING: VULKAN_SDK not found. Install Vulkan SDK and set env var.")
        end
    end
end

project "NuimEngine"
    kind "StaticLib"
    NuimCommon()

    includedirs { "NuimEngine/include", "NuimEngine/src" }

    files {
        "NuimEngine/include/**.h", "NuimEngine/include/**.hpp",
        "NuimEngine/src/**.c", "NuimEngine/src/**.cpp",
        "NuimEngine/src/**.h", "NuimEngine/src/**.hpp"
    }

    if GFX == "dx11" then
        defines { "NUIM_GFX_DX11" }
        excludes {
            "NuimEngine/src/Renderer/Backends/DX12/**",
            "NuimEngine/src/Renderer/Backends/Vulkan/**"
        }
    elseif GFX == "dx12" then
        defines { "NUIM_GFX_DX12" }
        excludes {
            "NuimEngine/src/Renderer/Backends/DX11/**",
            "NuimEngine/src/Renderer/Backends/Vulkan/**"
        }
    elseif GFX == "vulkan" then
        defines { "NUIM_GFX_VULKAN" }
        excludes {
            "NuimEngine/src/Renderer/Backends/DX11/**",
            "NuimEngine/src/Renderer/Backends/DX12/**"
        }
    end

project "Sandbox"
    kind "WindowedApp"
    NuimCommon()

    dependson { "NuimEngine" }
    links     { "NuimEngine" }

    includedirs { "NuimEngine/include", "Sandbox/src" }

    files {
        "Sandbox/src/**.h",
        "Sandbox/src/**.hpp",
        "Sandbox/src/**.cpp"
    }

    LinkForWinApp()
