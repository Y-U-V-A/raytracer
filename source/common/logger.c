#include "logger.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include "zmemory.h"

void log_stdout(const char* msg_fmt, ...) {
    va_list args;
    va_start(args, msg_fmt);
    vfprintf(stdout, msg_fmt, args);
    va_end(args);
}

void log_stderr(const char* msg_fmt, ...) {
    va_list args;
    va_start(args, msg_fmt);
    vfprintf(stderr, msg_fmt, args);
    va_end(args);
}

void log_file(FILE* file, const char* msg_fmt, ...) {
    va_list args;
    va_start(args, msg_fmt);
    vfprintf(file, msg_fmt, args);
    va_end(args);
}

u32 log_buffer(char* buffer, u64 size, const char* msg_fmt, ...) {
    va_list args;
    va_start(args, msg_fmt);
    u32 written = vsnprintf(buffer, size, msg_fmt, args);
    va_end(args);
    return written;
}

FILE* file_open(const char* file_name, bool binary, file_modes mode) {
#ifdef PLATFORM_LINUX
    FILE* file;
    if (mode == FILE_MODE_READ) {
        if (binary) {
            file = fopen(file_name, "rb");
        } else {
            file = fopen(file_name, "r");
        }
    } else if (mode == FILE_MODE_WRITE || mode == FILE_MODE_READ_AND_WRITE) {
        if (binary) {
            file = fopen(file_name, "w+b");
        } else {
            file = fopen(file_name, "w+");
        }
    } else if (mode == FILE_MODE_APPEND) {
        if (binary) {
            file = fopen(file_name, "a+b");
        } else {
            file = fopen(file_name, "a+");
        }
    } else {
        LOGE("file_open : invalid file mode");
        return 0;
    }
#else
    FILE* file;
    if (mode == FILE_MODE_READ) {
        if (binary) {
            fopen_s(&file, file_name, "rb");
        } else {
            fopen_s(&file, file_name, "r");
        }
    } else if (mode == FILE_MODE_WRITE || mode == FILE_MODE_READ_AND_WRITE) {
        if (binary) {
            fopen_s(&file, file_name, "w+b");
        } else {
            fopen_s(&file, file_name, "w+");
        }
    } else if (mode == FILE_MODE_APPEND) {
        if (binary) {
            fopen_s(&file, file_name, "a+b");
        } else {
            fopen_s(&file, file_name, "a+");
        }
    } else {
        LOGE("file_open : invalid file mode");
        return 0;
    }
#endif
    return file;
}

void file_close(FILE* file) {
    if (fclose(file) != 0) {
        LOGE("file_close : failed to close the file");
    }
}

u64 file_read(void* buffer, u64 stride, u64 count, FILE* file) {
    return fread(buffer, stride, count, file);
}

u64 file_write(const void* buffer, u64 stride, u64 count, FILE* file) {
    return fwrite(buffer, stride, count, file);
}

u32 file_position(FILE* file) {
    return ftell(file);
}

i32 file_seek_end(FILE* file) {
    return fseek(file, 0, SEEK_END);
}

typedef struct {
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
} __attribute__((packed)) BMPFileHeader;

typedef struct {
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
} __attribute__((packed)) BMPDIBHeader;

#ifdef PLATFORM_WINDOWS

void convert_ppm_to_bmp(const char* ppm_file, const char* bmp_file) {
    FILE *bmp, *ppm;
    fopen_s(&bmp, bmp_file, "w+b"); // Note: "w+b" for binary writing
    if (!bmp) {
        perror("Failed to open BMP file");
        return;
    }
    fopen_s(&ppm, ppm_file, "r");
    if (!ppm) {
        perror("Failed to open PPM file");
        fclose(bmp);
        return;
    }

    // Read PPM header
    char format[3];
    int width, height, max_color;
    if (fscanf_s(ppm, "%2s\n", format) != 1) {
        fprintf(stderr, "Failed to read PPM format\n");
        return;
    }

    // Skip comments
    int c = getc(ppm);
    while (c == '#') {
        while (getc(ppm) != '\n')
            ;
        c = getc(ppm);
    }
    ungetc(c, ppm);

    if (fscanf_s(ppm, "%d %d\n%d\n", &width, &height, &max_color) != 3) {
        fprintf(stderr, "Failed to read PPM dimensions\n");
        return;
    }

    // Calculate padding and row size
    int padding = (4 - (width * 3) % 4) % 4;
    int row_size = width * 3 + padding;

    // Prepare and write BMP headers
    BMPFileHeader file_header = {
        .bfType = 0x4D42, // "BM"
        .bfSize = sizeof(BMPFileHeader) + sizeof(BMPDIBHeader) + height * row_size,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPDIBHeader)};

    BMPDIBHeader dib_header = {
        .biSize = sizeof(BMPDIBHeader),
        .biWidth = width,
        .biHeight = -height, // Positive for bottom-up image
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = height * row_size,
        .biXPelsPerMeter = 0, // 0 DPI
        .biYPelsPerMeter = 0, // 0 DPI
        .biClrUsed = 0,
        .biClrImportant = 0};

    // Write headers
    fwrite(&file_header, sizeof(BMPFileHeader), 1, bmp);
    fwrite(&dib_header, sizeof(BMPDIBHeader), 1, bmp);

    // Allocate buffer for one row
    uint8_t* row_buffer = zmemory_allocate(row_size);
    if (!row_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Read and write pixel data (bottom-up for BMP)
    for (int y = height - 1; y >= 0; y--) {
        int pos = 0;
        for (int x = 0; x < width; x++) {
            int r, g, b;
            if (fscanf_s(ppm, "%d %d %d", &r, &g, &b) != 3) {
                fprintf(stderr, "Failed to read pixel data\n");
                zmemory_free(row_buffer, row_size);
                return;
            }
            row_buffer[pos++] = (uint8_t)b; // BGR format for BMP
            row_buffer[pos++] = (uint8_t)g;
            row_buffer[pos++] = (uint8_t)r;
        }
        // Add padding
        for (int p = 0; p < padding; p++) {
            row_buffer[pos++] = 0;
        }
        fwrite(row_buffer, 1, row_size, bmp);
    }

    zmemory_free(row_buffer, row_size);
    fclose(bmp);
    fclose(ppm);
    printf("ppm to bmp Conversion successful: %s\n", bmp_file);
}

#else

void convert_ppm_to_bmp(const char* ppm_file, const char* bmp_file) {
    FILE* ppm = fopen(ppm_file, "r");
    if (!ppm) {
        perror("Failed to open PPM file");
        return;
    }
    FILE* bmp = fopen(bmp_file, "w+b"); // Note: "w+b" for binary writing
    if (!bmp) {
        perror("Failed to open BMP file");
        fclose(ppm);
        return;
    }

    // Read PPM header
    char format[3];
    int width, height, max_color;
    if (fscanf(ppm, "%2s\n", format) != 1) {
        fprintf(stderr, "Failed to read PPM format\n");
        return;
    }

    // Skip comments
    int c = getc(ppm);
    while (c == '#') {
        while (getc(ppm) != '\n')
            ;
        c = getc(ppm);
    }
    ungetc(c, ppm);

    if (fscanf(ppm, "%d %d\n%d\n", &width, &height, &max_color) != 3) {
        fprintf(stderr, "Failed to read PPM dimensions\n");
        return;
    }

    // Calculate padding and row size
    int padding = (4 - (width * 3) % 4) % 4;
    int row_size = width * 3 + padding;

    // Prepare and write BMP headers
    BMPFileHeader file_header = {
        .bfType = 0x4D42, // "BM"
        .bfSize = sizeof(BMPFileHeader) + sizeof(BMPDIBHeader) + height * row_size,
        .bfReserved1 = 0,
        .bfReserved2 = 0,
        .bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPDIBHeader)};

    BMPDIBHeader dib_header = {
        .biSize = sizeof(BMPDIBHeader),
        .biWidth = width,
        .biHeight = -height, // Positive for bottom-up image
        .biPlanes = 1,
        .biBitCount = 24,
        .biCompression = 0,
        .biSizeImage = height * row_size,
        .biXPelsPerMeter = 0, // 0 DPI
        .biYPelsPerMeter = 0, // 0 DPI
        .biClrUsed = 0,
        .biClrImportant = 0};

    // Write headers
    fwrite(&file_header, sizeof(BMPFileHeader), 1, bmp);
    fwrite(&dib_header, sizeof(BMPDIBHeader), 1, bmp);

    // Allocate buffer for one row
    uint8_t* row_buffer = zmemory_allocate(row_size);
    if (!row_buffer) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    // Read and write pixel data (bottom-up for BMP)
    for (int y = height - 1; y >= 0; y--) {
        int pos = 0;
        for (int x = 0; x < width; x++) {
            int r, g, b;
            if (fscanf(ppm, "%d %d %d", &r, &g, &b) != 3) {
                fprintf(stderr, "Failed to read pixel data\n");
                zmemory_free(row_buffer, row_size);
                return;
            }
            row_buffer[pos++] = (uint8_t)b; // BGR format for BMP
            row_buffer[pos++] = (uint8_t)g;
            row_buffer[pos++] = (uint8_t)r;
        }
        // Add padding
        for (int p = 0; p < padding; p++) {
            row_buffer[pos++] = 0;
        }
        fwrite(row_buffer, 1, row_size, bmp);
    }

    zmemory_free(row_buffer, row_size);
    fclose(bmp);
    fclose(ppm);
    printf("ppm to bmp Conversion successful: %s\n", bmp_file);
}

#endif
