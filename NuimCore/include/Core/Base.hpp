#pragma once
#include <cstdint>
#include <cstdlib>

// ---------------- Platform ----------------
#if !defined(NUIM_PLATFORM_WINDOWS)
	#if defined(_WIN32) || defined(_WIN64)
		#define NUIM_PLATFORM_WINDOWS 1
	#else
		#error Nuim: Unsupported platform
	#endif
#endif

// ---------------- Build config ----------------
#if !defined(NUIM_DEBUG) && !defined(NUIM_RELEASE)
	#if defined(_DEBUG) 
		#define NUIM_DEBUG 1
	#else
		#define NUIM_RELEASE 1
	#endif
#endif

// ---------------- DLL export/import (for future) ----------------
#if defined(NUIM_BUILD_DLL)
	#define NUIM_API __declspec(dllexport)
#elif defined (NUIM_USE_DLL)
	#define NUIM_API __declspec(dllimport)
#else
	#define NUIM_API
#endif

// ---------------- helpers ----------------
#define NUIM_BIT(x) (1u << (x))

namespace Nuim {
	using U8 = uint8_t;
	using U16 = uint16_t;
	using U32 = uint32_t;
	using U64 = uint64_t;

	using I8 = int8_t;
	using I16 = int16_t;
	using I32 = int32_t;
	using I64 = int64_t;
}