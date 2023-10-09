project "D_Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"

--     -- Includes
    includedirs {
        "%{wks.location}/Linking/include",
    }

    files {
        "%{wks.location}/D_Engine/src/*.cpp",
        "%{wks.location}/D_Engine/src/*.h",
        "%{wks.location}/D_Engine/src/*.hpp",
        "%{wks.location}/D_Engine/src/*.c",

        "%{wks.location}/D_Engine/src/Nuim/Core/*.cpp",
        "%{wks.location}/D_Engine/src/Nuim/Core/*.h",
        "%{wks.location}/D_Engine/src/Nuim/Core/*.hpp",
        "%{wks.location}/D_Engine/src/Nuim/Core/*.c",

        "%{wks.location}/D_Engine/src/Nuim/Renderer/*.cpp",
        "%{wks.location}/D_Engine/src/Nuim/Renderer/*.h",
        "%{wks.location}/D_Engine/src/Nuim/Renderer/*.hpp",
        "%{wks.location}/D_Engine/src/Nuim/Renderer/*.c",

        "%{wks.location}/D_Engine/src/Nuim/ImGui/*.cpp",
        "%{wks.location}/D_Engine/src/Nuim/ImGui/*.h",
        "%{wks.location}/D_Engine/src/Nuim/ImGui/*.hpp",
        "%{wks.location}/D_Engine/src/Nuim/ImGui/*.c",
    }