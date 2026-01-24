#ifndef LOGGER__H
#define LOGGER__H

#include "defines.h"

#ifdef NDEBUG
#    define LOGE(fmt, ...)
#    define LOGW(fmt, ...)
#    define LOGI(fmt, ...)
#    define LOGD(fmt, ...)
#    define LOGT(fmt, ...)
#    define ASSERT(exp)
#    define STATIC_ASSERT(exp)
#else
#    define LOGE(fmt, ...) log_stdout("\033[31m" fmt "\033[0m\n", ##__VA_ARGS__);
#    define LOGW(fmt, ...) log_stdout("\033[33m" fmt "\033[0m\n", ##__VA_ARGS__);
#    define LOGI(fmt, ...) log_stdout("\033[37m" fmt "\033[0m\n", ##__VA_ARGS__);
#    define LOGD(fmt, ...) log_stdout("\033[34m" fmt "\033[0m\n", ##__VA_ARGS__);
#    define LOGT(fmt, ...) log_stdout("\033[32m" fmt "\033[0m\n", ##__VA_ARGS__);

#    define ASSERT(exp)                                                 \
        do {                                                            \
            if (exp) {                                                  \
            } else {                                                    \
                LOGE("[assert]:%s -> %s:%d", #exp, __FILE__, __LINE__); \
                __builtin_trap();                                       \
            }                                                           \
        } while (0)

#    define GLUE_(a, b) a##b
#    define GLUE(a, b) GLUE_(a, b)
#    define STATIC_ASSERT(exp)                                   \
        enum {                                                   \
            GLUE(static_assert__, __LINE__) = 1 / (int)(!!(exp)) \
        }
#endif

void log_stdout(const char* fmt, ...);

void log_stderr(const char* fmt, ...);

void log_file(FILE* file, const char* fmt, ...);

u32 log_buffer(void* buffer, u32 size, const char* fmt, ...);

FILE* file_open(const char* filename);

void file_close(FILE* file);

#endif