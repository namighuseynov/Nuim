workspace "Nuim" 
    system "windows"
    architecture "x64"
    configurations "Release"
    systemversion "latest"
    location "Nuim"
    startproject "Nuim"
    targetdir "bin/"
    objdir "bin-obj/" 

project "Engine"
    kind "StaticLib"
    language "C++"  
    location "Engine"

    files { "**.h", "**.c" }

    filter "system:windows" 
        cppdialect "C++17"

project "Nuim"   
    kind "ConsoleApp"  
    language "C++"   
    location "Nuim"
    links "Engine"
    
    files { "**.h", "**.c" }

    filter "system:windows" 
        cppdialect "C++17"
        links {"kernel32", "user32", "gdi32", "winspool", "comdlg32", "advapi32", "shell32", "ole32", "oleaut32", "uuid", "odbc32", "odbccp32", "opengl32", "GLFW/glfw3.lib"}
    filter "configurations:Release" 
        defines { "NDEBUG" }      
        optimize "On" 
    includedirs {"Linking/include"}
    libdirs {"Linking/libs/GLFW"}