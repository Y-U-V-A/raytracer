#ifndef DEFINES__H
#define DEFINES__H

#define CONFIG_DEBUG

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

#define true 1
#define false 0

#define SET_BIT(index) (1 << (index))
#define CHECK_BIT(val, index) ((1 << (index)) & (val)) != 0

#if defined(_WIN32)
#    define PLATFORM_WINDOWS
#elif defined(__linux__)
#    define PLATFORM_LINUX
#else
#    error "Unknown platform"
#endif

#define INLINE __attribute__((always_inline)) static inline
#define NOINLINE __attribute__((noinline))

#define PI 3.14159265359
#define INFINITY ((float)(1e+300 * 1e+300))
#define DBL_EPSILON 2.2204460492503131e-16
#define FLT_EPSILON 1.19209290e-7F

#define CLAMP(min, max, val) ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))
#define DEG_TO_RAD(val) ((val) * PI / 180)
#define IS_POWER_OF_2(val) ((bool)(((val) != 0) && ((val) & ((val) - 1)) == 0))
#define DOUBLE_COMPARE(val_1, val_2) ((bool)(zfabs((val_1) - (val_2)) < DBL_EPSILON))
#define NDC_TO_UNIT(val) (((val) + 1.0) * 0.5)

#define _GLUE(a, b) a##b
#define GLUE(a, b) _GLUE(a, b)

#ifdef PLATFORM_WINDOWS
typedef struct _iobuf FILE;
#else
typedef struct _IO_FILE FILE;
#endif

#endif