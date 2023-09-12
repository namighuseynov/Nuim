#pragma once
#ifdef _WIN32
	#ifdef _WIN64
		#define NUIM_PLATFORM_WINDOWS
	#else 
		#error "x86 is not supported"
	#endif // _WIN64
#endif