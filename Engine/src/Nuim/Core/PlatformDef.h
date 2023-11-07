#ifndef PLATFORM_DEF_H

#ifdef _WIN32
#ifdef _WIN64

#define NUIM_PLATFORM_WINDOWS
#else 
#error "Nuim Engine does not support x32 architecture"
#endif 

#elif defined(__linux__) 

#define NUIM_PLATFORM_LINUX

#elif defined(__APPLE__)

#error "Mac platform does not supporting by Nuim Engine yet"

#endif

#define PLATFORM_DEF_H
#endif
