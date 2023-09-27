#pragma once

#ifdef _WIN32

#ifdef _WIN64

#define NUIM_PLATFORM_WINDOWS
#include <Windows.h>

#else 
#error "Nuim Engine does not support x32 architecture"
#endif 


#elif __linux__

#error "Linux platform does not supporting by Nuim Engine yet"

#elif __APPLE__

#error "Mac platform does not supporting by Nuim Engine yet"

#endif