#ifndef ASSERTS__H
#define ASSERTS__H

#include "defines.h"
#include "logger.h"

#ifdef CONFIG_DEBUG

#    define BEBUG_BREAK() __builtin_trap()

#    define _ASSERT_GLUE(a, b) a##b
#    define ASSERT_GLUE(a, b) _ASSERT_GLUE(a, b)
#    define STATIC_ASSERT(exp) enum {                       \
        ASSERT_GLUE(assert_, __LINE__) = 1 / (int)(!!(exp)) \
    }

STATIC_ASSERT(sizeof(u8) == 1);
STATIC_ASSERT(sizeof(u16) == 2);
STATIC_ASSERT(sizeof(u32) == 4);
STATIC_ASSERT(sizeof(u64) == 8);

STATIC_ASSERT(sizeof(i8) == 1);
STATIC_ASSERT(sizeof(i16) == 2);
STATIC_ASSERT(sizeof(i32) == 4);
STATIC_ASSERT(sizeof(i64) == 8);

STATIC_ASSERT(sizeof(f32) == 4);
STATIC_ASSERT(sizeof(f64) == 8);

#    define ASSERT(exp)                                           \
        if (exp) {                                                \
                                                                  \
        } else {                                                  \
            LOGE("%s failed at %s:%d", #exp, __FILE__, __LINE__); \
            BEBUG_BREAK();                                        \
        }

#else

#    define STATIC_ASSERT(exp)
#    define ASSERT(exp)

#endif

#endif