project "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"

--     -- Includes
    includedirs {
        "%{wks.location}/Linking/include",
        "%{wks.location}/Engine/src/"
    }

    files {
        "%{wks.location}/Engine/src/*.cpp",
        "%{wks.location}/Engine/src/*.h",
        "%{wks.location}/Engine/src/*.hpp",
        "%{wks.location}/Engine/src/*.c",

        "%{wks.location}/Engine/src/Nuim/Core/*.cpp",
        "%{wks.location}/Engine/src/Nuim/Core/*.h",
        "%{wks.location}/Engine/src/Nuim/Core/*.hpp",
        "%{wks.location}/Engine/src/Nuim/Core/*.c",

        "%{wks.location}/Engine/src/Nuim/Renderer/*.cpp",
        "%{wks.location}/Engine/src/Nuim/Renderer/*.h",
        "%{wks.location}/Engine/src/Nuim/Renderer/*.hpp",
        "%{wks.location}/Engine/src/Nuim/Renderer/*.c",

        "%{wks.location}/Engine/src/Nuim/ImGui/*.cpp",
        "%{wks.location}/Engine/src/Nuim/ImGui/*.h",
        "%{wks.location}/Engine/src/Nuim/ImGui/*.hpp",
        "%{wks.location}/Engine/src/Nuim/ImGui/*.c",

        "%{wks.location}/Engine/src/Nuim/Platform/*.cpp",
        "%{wks.location}/Engine/src/Nuim/Platform/*.h",
        "%{wks.location}/Engine/src/Nuim/Platform/*.hpp",
        "%{wks.location}/Engine/src/Nuim/Platform/*.c",

        "%{wks.location}/Engine/src/Nuim/Platform/glad/*.cpp",
        "%{wks.location}/Engine/src/Nuim/Platform/glad/*.h",
        "%{wks.location}/Engine/src/Nuim/Platform/glad/*.hpp",
        "%{wks.location}/Engine/src/Nuim/Platform/glad/*.c",
    }