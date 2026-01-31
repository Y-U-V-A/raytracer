#ifndef DEFINES__H
#define DEFINES__H

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef float f32;
typedef double f64;
typedef _Bool bool;

#define TRUE 1
#define FALSE 0

#define INLINE static inline

#if defined(_WIN32) || defined(_WIN64)
#    define PLATFORM_WINDOWS
#elif defined(__linux__) || defined(__gnu_linux__) || defined(__linux) || defined(linux)
#    define PLATFORM_LINUX
#else
#    error "platform not supported"
#endif

#ifdef PLATFORM_WINDOWS
typedef struct _iobuf FILE;
#else
typedef struct _IO_FILE FILE;
#endif

extern f32 infinity;
extern u64 memory;

#define PI 3.1415926535897932384626433832795
#define PI_OVER_4 0.78539816339744830961566084581988

#endif