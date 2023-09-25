#pragma once

#ifndef TRUE
#define TRUE 1
#else 
#undef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#else 
#undef FALSE
#define FALSE 0
#endif // !FALSE


#ifndef U8 
typedef unsigned __int8 U8;
#else 
#undef U8
typedef unsigned __int8 U8;
#endif

#ifndef U16 
typedef unsigned __int16 U16;
#else
#undef U16
typedef unsigned __int16 U16;
#endif

#ifndef U32 
typedef unsigned __int32 U32;
#else
#undef U32
typedef unsigned __int32 U32;
#endif

#ifndef U64 
typedef unsigned __int64 U64;
#else
#undef U64
typedef unsigned __int64 U64;
#endif

#ifndef I8 
typedef signed __int8 I8;
#else 
#undef I8
typedef signed __int8 I8;
#endif

#ifndef I16 
typedef signed __int16 I16;
#else 
#undef I16
typedef signed __int16 I16;
#endif

#ifndef I32
typedef signed __int32 I32;
#else
#undef I32
typedef signed __int32 I32;
#endif

#ifndef I64
typedef signed __int64 I64;
#else
#undef I64
typedef signed __int64 I64;
#endif