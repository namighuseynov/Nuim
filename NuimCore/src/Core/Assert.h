#pragma once
#include "Base.hpp"
#include "Log.hpp"

#if NUIM_PLATFORM_WINDOWS
#include <intrin.h>
#define NUIM_DEBUGBREAK() __debugbreak()
#else
#define NUIM_DEBUGBREAK() ((void)0)
#endif

#if defined(NUIM_DEBUG)
#define NUIM_ASSERT(cond, ...)                                                     \
        do {                                                                           \
            if (!(cond)) {                                                             \
                ::Nuim::Log::Error("ASSERT FAILED: {}", ::Nuim::Log::Format(__VA_ARGS__)); \
                NUIM_DEBUGBREAK();                                                     \
            }                                                                          \
        } while (0)
#else

#define NUIM_ASSERT(cond, ...) ((void)0)
#endif
