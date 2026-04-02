#ifndef LOGGER__H
#define LOGGER__H

#include "defines.h"

#define LOGE(msg_fmt, ...) log_stdout("\033[31m" msg_fmt "\033[0m\n", ##__VA_ARGS__)
#define LOGW(msg_fmt, ...) log_stdout("\033[33m" msg_fmt "\033[0m\n", ##__VA_ARGS__)
#define LOGI(msg_fmt, ...) log_stdout("\033[37m" msg_fmt "\033[0m\n", ##__VA_ARGS__)
#define LOGD(msg_fmt, ...) log_stdout("\033[34m" msg_fmt "\033[0m\n", ##__VA_ARGS__)
#define LOGT(msg_fmt, ...) log_stdout("\033[32m" msg_fmt "\033[0m\n", ##__VA_ARGS__)

#define LOG_STDOUT(msg_fmt, ...) log_stdout(msg_fmt, ##__VA_ARGS__)
#define LOG_STDERR(msg_fmt, ...) log_stderr(msg_fmt, ##__VA_ARGS__)
#define LOG_FILE(file, msg_fmt, ...) log_file(file, msg_fmt, ##__VA_ARGS__)
#define LOG_BUFFER(buffer, size, msg_fmt, ...) log_buffer(buffer, size, msg_fmt, ##__VA_ARGS__)

typedef enum file_modes {
    FILE_MODE_READ,
    FILE_MODE_WRITE,
    FILE_MODE_APPEND,
    FILE_MODE_READ_AND_WRITE,
    FILE_MODE_MAX,
} file_modes;

void log_stdout(const char* msg_fmt, ...);

void log_stderr(const char* msg_fmt, ...);

void log_file(FILE* file, const char* msg_fmt, ...);

u32 log_buffer(char* buffer, u64 size, const char* msg_fmt, ...);

FILE* file_open(const char* file_name, bool binary, file_modes mode);

void file_close(FILE* file);

u64 file_read(void* buffer, u64 stride, u64 count, FILE* file);

u64 file_write(const void* buffer, u64 stride, u64 count, FILE* file);

u32 file_position(FILE* file);

i32 file_seek_end(FILE* file);

void convert_ppm_to_bmp(const char* ppm_file, const char* bmp_file);

#endif