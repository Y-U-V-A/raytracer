#include "logger.h"
#include <stdarg.h>
#include <stdio.h>

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

void log_stdout(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);
}

void log_stderr(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

void log_file(FILE* file, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(file, fmt, args);
    va_end(args);
}

u32 log_buffer(void* buffer, u32 size, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    i32 written = vsnprintf(buffer, size, fmt, args);
    va_end(args);
    return (u32)written;
}

FILE* file_open(const char* filename) {
    FILE* file;
    fopen_s(&file, filename, "w+");
    return file;
}

void file_close(FILE* file) {
    fclose(file);
}