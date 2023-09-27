project "V_Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"


    libdirs {
        "%{wks.location}/Linking/libs/GLFW",
        "%{wks.location}/Linking/libs/GLAD",
        "%{wks.location}/Linking/libs/Vulkan",
    }

    includedirs {
        "%{wks.location}/Linking/include",
    }

    links {
        "opengl32.lib",
        "glfw3.lib",
        "vulkan-1.lib",
    }

    files {
        "%{wks.location}/V_Engine/src/*.cpp",
        "%{wks.location}/V_Engine/src/*.h",
        "%{wks.location}/V_Engine/src/*.c",

        "%{wks.location}/V_Engine/src/V_Nuim/Core/*.cpp",
        "%{wks.location}/V_Engine/src/V_Nuim/Core/*.h",
        "%{wks.location}/V_Engine/src/V_Nuim/Core/*.c",

        -- "../Vulkan/src/Nuim/Core/*.cpp",
        -- "../Vulkan/src/Nuim/Core/*.h",
        -- "../Vulkan/src/Nuim/Core/*.c",

        -- "../Vulkan/src/Nuim/Events/*.cpp",
        -- "../Vulkan/src/Nuim/Events/*.h",
        -- "../Vulkan/src/Nuim/Events/*.c",

        -- "../Vulkan/src/Nuim/Imgui/*.cpp",
        -- "../Vulkan/src/Nuim/Imgui/*.h",
        -- "../Vulkan/src/Nuim/Imgui/*.c",

        -- "../Vulkan/src/Nuim/Math/*.cpp",
        -- "../Vulkan/src/Nuim/Math/*.h",
        -- "../Vulkan/src/Nuim/Math/*.c",

        -- "../Vulkan/src/Nuim/Renderer/*.cpp",
        -- "../Vulkan/src/Nuim/Renderer/*.h",
        -- "../Vulkan/src/Nuim/Renderer/*.c",

        -- "../Vulkan/src/Nuim/Scene/*.cpp",
        -- "../Vulkan/src/Nuim/Scene/*.h",
        -- "../Vulkan/src/Nuim/Scene/*.c",

    }