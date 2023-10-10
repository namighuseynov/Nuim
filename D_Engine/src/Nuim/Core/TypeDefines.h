#pragma once

#ifdef NUIM_PLATFORM_WINDOWS
#include <iostream>
#include <cstdint>
#include <string>

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


#ifndef STRING 
typedef const char* STRING;
#else	
#undef STRING
typedef const char* STRING;
#endif // STRING

typedef STRING* PSTRING;

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

#ifdef NUIM_CHARACTERSET_UNICODE

#ifndef CHAR16

typedef char16_t CHAR16;
typedef const CHAR16* USTRING;
typedef USTRING* PUSTRING;

#endif // !CHAR16

#endif // NUIM_CHARSET_UNICODE

#ifndef SIZEOF_CHAR16
#define SIZEOF_CHAR16 sizeof(CHAR16)
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

#else 
#error					"Nuim supports only Windows platform!"
#endif

