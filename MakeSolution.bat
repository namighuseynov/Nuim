DEL /S *.sln
DEL /S /Q *.vcxproj
DEL /S /Q *.vcxproj.filters
DEL /S /Q *.vcxproj.user
rmdir /s /q bin
rmdir /s /q bin-obj
rmdir /s /q .vs

vendor\premake\premake5.exe --file=premake5.lua vs2022
