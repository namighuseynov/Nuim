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
        "%{wks.location}/D_Engine/src/*.c",

        "%{wks.location}/D_Engine/src/V_Nuim/Core/*.cpp",
        "%{wks.location}/D_Engine/src/V_Nuim/Core/*.h",
        "%{wks.location}/D_Engine/src/V_Nuim/Core/*.c",
    }