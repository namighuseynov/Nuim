project "Nuim"
    -- kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetdir "%{wks.location}/bin"
    objdir "%{wks.location}/bin-obj"
    links {
        -- "G_Engine",
        -- "V_Engine",
        "D_Engine"
    }
    includedirs {
        "%{wks.location}/Linking/include",
        "%{wks.location}/V_Engine/src/",
        "%{wks.location}/G_Engine/src/",
    }

    files {
        "%{wks.location}/Nuim/src/*.cpp",
        "%{wks.location}/Nuim/src/*.h",
        "%{wks.location}/Nuim/src/*.c",
    }

    filter "system:windows"
        kind "WindowedApp"