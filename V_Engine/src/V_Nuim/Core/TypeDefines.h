#pragma once
#include <cstdint>

#ifndef NM_TRUE
#define TRUE 1
#else 
#undef NM_TRUE
#define NM_TRUE 1
#endif

#ifndef NM_FALSE
#define NM_FALSE 0
#else 
#undef NM_FALSE
#define NM_FALSE 0
#endif // !NM_FALSE


#ifndef U8 
typedef uint8_t U8;
#else 
#undef U8
typedef uint8_t U8;
#endif

#ifndef U16 
typedef uint16_t U16;
#else
#undef U16
typedef uint16_t U16;
#endif

#ifndef U32 
typedef uint32_t U32;
#else
#undef U32
typedef uint32_t U32;
#endif

#ifndef U64 
typedef uint64_t U64;
#else
#undef U64
typedef uint64_t U64;
#endif

#ifndef I8 
typedef int8_t I8;
#else 
#undef I8
typedef int8_t I8;
#endif

#ifndef I16 
typedef int16_t I16;
#else 
#undef I16
typedef int16_t I16;
#endif

#ifndef I32
typedef int32_t I32;
#else
#undef I32
typedef int32_t I32;
#endif

#ifndef I64
typedef int64_t I64;
#else
#undef I64
typedef int64_t I64;
#endif