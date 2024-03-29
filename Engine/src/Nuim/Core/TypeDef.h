#ifndef TYPE_DEF_H
#include "PlatformDef.h"

#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <functional>
#include <stdio.h>
#include <sstream>

#ifndef NM_TRUE
#define NM_TRUE			1
#else 
#undef NM_TRUE
#define NM_TRUE			1
#endif

#ifndef NM_FALSE
#define NM_FALSE		0
#else 
#undef NM_FALSE
#define NM_FALSE		0
#endif					// !NM_FALSE

#ifndef NUIM_SUCCESS
#define NUIM_SUCCESS 0
#endif // !NUIM_SUCCESS

#ifndef NUIM_FAILURE
#define NUIM_FAILURE 1
#endif // !NUIM_FAILURE

#ifndef U8 
typedef uint8_t			U8;
#else 
#undef U8
typedef uint8_t			U8;
#endif

#ifndef U16 
typedef uint16_t		U16;
#else
#undef U16
typedef uint16_t		U16;
#endif

#ifndef U32 
typedef uint32_t		U32;
#else
#undef U32
typedef uint32_t		U32;
#endif

#ifndef U64 
typedef uint64_t		U64;
#else
#undef U64
typedef uint64_t		U64;
#endif

#ifndef I8 
typedef int8_t			I8;
#else 
#undef I8
typedef int8_t			I8;
#endif

#ifndef I16 
typedef int16_t			I16;
#else 
#undef I16
typedef int16_t			I16;
#endif

#ifndef I32
typedef int32_t			I32;
#else
#undef I32
typedef int32_t			I32;
#endif

#ifndef I64
typedef int64_t			I64;
#else
#undef I64
typedef int64_t			I64;
#endif

#ifndef SIZEOF_U8
#define SIZEOF_U8 sizeof(U8)
#endif

#ifndef SIZEOF_U16
#define SIZEOF_U16 sizeof(U16)
#endif

#ifndef SIZEOF_U32
#define SIZEOF_U32 sizeof(U32)
#endif

#ifndef SIZEOF_U64
#define SIZEOF_U64 sizeof(U64)
#endif

#ifndef SIZEOF_I8
#define SIZEOF_I8 sizeof(I8)
#endif

#ifndef SIZEOF_I16
#define SIZEOF_I16 sizeof(I16)
#endif

#ifndef SIZEOF_I32
#define SIZEOF_I32 sizeof(I32)
#endif

#ifndef SIZEOF_I64
#define SIZEOF_I64 sizeof(I64)
#endif

#ifdef NUIM_PLATFORM_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>



#ifndef STRING 
typedef const char* STRING;
#else	
#undef STRING
typedef const char* STRING;
#endif // STRING

typedef STRING* PSTRING;



#ifdef NUIM_CHARACTERSET_UNICODE

#ifndef CHAR16

typedef char16_t CHAR16;
typedef const CHAR16* USTRING;
typedef USTRING* PUSTRING;

#ifdef _UNICODE
#define UNICODE_TEXT(x) (LPCWSTR)u##x
#else
#define UNICODE_TEXT(x) x
#endif

#endif // !CHAR16

#endif // NUIM_CHARSET_UNICODE

#ifndef SIZEOF_CHAR16
#define SIZEOF_CHAR16 sizeof(CHAR16)
#endif

#elif defined(NUIM_PLATFORM_LINUX)



#endif
#define TYPE_DEF_H
#endif
